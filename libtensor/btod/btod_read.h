#ifndef LIBTENSOR_BTOD_READ_H
#define LIBTENSOR_BTOD_READ_H

#include <istream>
#include <sstream>
#include "../defs.h"
#include "../exception.h"
#include "../timings.h"
#include "../btod/transf_double.h"
#include "../core/block_tensor_i.h"
#include "../core/block_tensor_ctrl.h"
#include "../core/orbit.h"
#include "../core/orbit_list.h"
#include "../core/tensor_i.h"
#include "../core/tensor_ctrl.h"
#include "../symmetry/so_copy.h"
#include "../symmetry/bad_symmetry.h"
#include "../tod/tod_compare.h"
#include "../tod/tod_set.h"

namespace libtensor {


/**	\brief Reads a block %tensor from a stream
	\tparam N Tensor order.

	\ingroup libtensor_btod
 **/
template<size_t N>
class btod_read : public timings< btod_read<N> > {
public:
	static const char *k_clazz; //!< Class name

private:
	std::istream &m_stream; //!< Input stream
	double m_thresh; //!< Zero threshold

public:
	//!	\name Construction and destruction
	//!{

	btod_read(std::istream &stream, double thresh = 0.0) :
		m_stream(stream), m_thresh(thresh) { }

	//!}

	//!	\name Operation
	//!{

	void perform(block_tensor_i<N, double> &bt) throw(exception);

	//!}

};


template<size_t N>
const char *btod_read<N>::k_clazz = "btod_read<N>";


template<size_t N>
void btod_read<N>::perform(block_tensor_i<N, double> &bt) throw(exception) {

	static const char *method = "perform(block_tensor_i<N, double>&)";

	btod_read<N>::start_timer();

	//
	//	Read the first line: order, dimensions
	//

	if(!m_stream.good()) {
		throw_exc(k_clazz, method, "Input stream is empty.");
	}

	int order;
	index<N> i1, i2;
	size_t k = 0;
	m_stream >> order;
	if(order != N) {
		throw_exc(k_clazz, method, "Incorrect tensor order.");
	}
	while(m_stream.good() && k < N) {
		int dim;
		m_stream >> dim;
		if(dim <= 0) {
			throw_exc(k_clazz, method,
				"Incorrect tensor dimension.");
		}
		i2[k] = dim - 1;
		k++;
	}
	if(k < N) {
		throw_exc(k_clazz, method, "Unexpected end of stream.");
	}

	const block_index_space<N> &bis = bt.get_bis();
	dimensions<N> dims(index_range<N>(i1, i2));
	dimensions<N> bdims(bis.get_block_index_dims());
	if(!dims.equals(bis.get_dims())) {
		throw_exc(k_clazz, method, "Incompatible tensor dimensions.");
	}

	//
	//	Set up the tensor
	//

	block_tensor_ctrl<N, double> ctrl(bt);
	symmetry<N, double> sym(bis);
	so_copy<N, double>(ctrl.req_const_symmetry()).perform(sym);
	ctrl.req_symmetry().clear();
	ctrl.req_zero_all_blocks();

	//
	//	Read tensor elements from file into buffer
	//

	double *buf = new double[dims.get_size()];

	for(size_t i = 0; i < dims.get_size(); i++) {
		if(!m_stream.good()) {
			throw_exc(k_clazz, method, "Unexpected end of stream.");
		}
		m_stream >> buf[i];
	}

	//
	//	Transfer data into the block tensor
	//

	abs_index<N> bi(bdims);
	do {
		tensor_i<N, double> &blk = ctrl.req_block(bi.get_index());
		bool zero = true;
		{
		tensor_ctrl<N, double> blk_ctrl(blk);
		const dimensions<N> &blk_dims = blk.get_dims();
		double *p = blk_ctrl.req_dataptr();

		index<N> blk_start_idx(bis.get_block_start(bi.get_index()));
		abs_index<N> blk_offs_aidx(blk_dims);
		size_t nj = blk_dims[N - 1];
		do {
			index<N> idx(blk_start_idx);
			const index<N> &offs(blk_offs_aidx.get_index());
			for(size_t i = 0; i < N; i++) idx[i] += offs[i];
			abs_index<N> aidx(idx, bis.get_dims());
			size_t blk_offs = blk_offs_aidx.get_abs_index();
			size_t buf_offs = aidx.get_abs_index();
#ifdef LIBTENSOR_DEBUG
			if(buf_offs + nj > dims.get_size()) {
				throw out_of_bounds(g_ns, k_clazz, method,
					__FILE__, __LINE__,
					"buf_offs");
			}
#endif // LIBTENSOR_DEBUG
			for(size_t j = 0; j < nj; j++) {
				register double d = buf[buf_offs + j];
				if(fabs(d) <= m_thresh) d = 0.0;
				else zero = false;
				p[blk_offs + j] = d;
			}
			for(size_t j = 1; j < nj; j++) blk_offs_aidx.inc();
		} while(blk_offs_aidx.inc());

		blk_ctrl.ret_dataptr(p);
		}
		ctrl.ret_block(bi.get_index());
		if(zero) ctrl.req_zero_block(bi.get_index());

	} while(bi.inc());

	delete [] buf;

	//
	// Check block tensor for its symmetry
	//

	orbit_list<N, double>  ol(sym);
	// loop over all orbits
	for (typename orbit_list<N, double>::iterator it = ol.begin();
			it != ol.end(); it++) {

		orbit<N, double> orb(sym, ol.get_index(it));

		// get canonical block
		size_t ac = orb.get_abs_canonical_index();
		abs_index<N> acidx(ac, bdims);

		bool zero = ctrl.req_is_zero_block(acidx.get_index());

		if (zero) {
			// loop over all blocks within an orbit
			for (typename orbit<N, double>::iterator ito = orb.begin();
					ito != orb.end(); ito++) {

				// do nothing for the canonical block
				if (ac == orb.get_abs_index(ito)) continue;

				abs_index<N> aidx(orb.get_abs_index(ito), bdims);

				if (ctrl.req_is_zero_block(aidx.get_index())) continue;

				std::ostringstream oss;
				oss << "Read block tensor does not match symmetry in block "
						<< aidx.get_index() << "(non-zero block).";
				throw bad_symmetry(g_ns, k_clazz, method, __FILE__, __LINE__,
						oss.str().c_str());
			}
		}
		else {
			// apply transformation to the canonical block
			tensor_i<N, double> &cblk = ctrl.req_block(acidx.get_index());

			// loop over all blocks within an orbit
			for (typename orbit<N, double>::iterator ito = orb.begin();
					ito != orb.end(); ito++) {

				// do nothing for the canonical block
				if (ac == orb.get_abs_index(ito)) continue;

				// get block transformation and index
				abs_index<N> aidx(orb.get_abs_index(ito), bdims);
				const transf<N, double> &tr = orb.get_transf(ito);

				// compare real and expected block
				tensor_i<N, double> &blk = ctrl.req_block(aidx.get_index());

				typedef libvmm::std_allocator<double> allocator;
				tensor<N, double, allocator> tmp_blk(blk.get_dims());
				tod_copy<N>(cblk, tr.get_perm(), tr.get_coeff()).perform(tmp_blk);

				tod_compare<N> cmp(blk, tmp_blk, 1e-13);
				if (! cmp.compare()) {
					ctrl.ret_block(aidx.get_index());
					ctrl.ret_block(acidx.get_index());

					std::ostringstream oss;
					oss << "Read block tensor does not match symmetry in block "
							<< aidx.get_index() << " at " << cmp.get_diff_index()
							<< "(expected: " << cmp.get_diff_elem_1() << ", found: "
							<< cmp.get_diff_elem_2() << ", diff: "
							<< cmp.get_diff_elem_1()-cmp.get_diff_elem_2() << ").";

					throw bad_symmetry(g_ns, k_clazz, method, __FILE__, __LINE__,
							oss.str().c_str());
				}

				ctrl.ret_block(aidx.get_index());

				// if real and expected block match zero the non-canonical block
				ctrl.req_zero_block(aidx.get_index());
			}
			ctrl.ret_block(acidx.get_index());
		}
	}

	// copy symmetry back to block tensor
	so_copy<N, double>(sym).perform(ctrl.req_symmetry());

	btod_read<N>::stop_timer();
}


} // namespace libtensor

#endif // LIBTENSOR_BTOD_READ_H
