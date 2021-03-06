#ifndef LIBTENSOR_GEN_BTO_AUX_COPY_IMPL_H
#define LIBTENSOR_GEN_BTO_AUX_COPY_IMPL_H

#include <libutil/threads/auto_lock.h>
#include <libtensor/core/abs_index.h>
#include <libtensor/symmetry/so_copy.h>
#include "../block_stream_exception.h"
#include "../gen_bto_aux_copy.h"

namespace libtensor {


template<size_t N, typename Traits>
const char *gen_bto_aux_copy<N, Traits>::k_clazz =
    "gen_bto_aux_copy<N, Traits>";


template<size_t N, typename Traits>
gen_bto_aux_copy<N, Traits>::gen_bto_aux_copy(
    const symmetry<N, element_type> &sym,
    gen_block_tensor_wr_i<N, bti_traits> &bt,
    bool sync) :

    m_sym(sym.get_bis()), m_bt(bt), m_ctrl(m_bt),
    m_bidims(m_bt.get_bis().get_block_index_dims()), m_open(false),
    m_sync(sync) {

    so_copy<N, element_type>(sym).perform(m_sym);
}


template<size_t N, typename Traits>
gen_bto_aux_copy<N, Traits>::~gen_bto_aux_copy() {

    if(m_open) close();
}


template<size_t N, typename Traits>
void gen_bto_aux_copy<N, Traits>::open() {

    if(m_open) {
        throw block_stream_exception(g_ns, k_clazz, "open()",
            __FILE__, __LINE__, "Stream is already open.");
    }

    m_ctrl.req_zero_all_blocks();
    so_copy<N, element_type>(m_sym).perform(m_ctrl.req_symmetry());
    m_open = true;
}


template<size_t N, typename Traits>
void gen_bto_aux_copy<N, Traits>::close() {

    if(!m_open) {
        throw block_stream_exception(g_ns, k_clazz, "close()",
            __FILE__, __LINE__, "Stream is already closed.");
    }

    m_open = false;
    for(typename std::map<size_t, block_status>::iterator i = m_blkstat.begin();
        i != m_blkstat.end(); ++i) delete i->second.mtx;
    m_blkstat.clear();
}


template<size_t N, typename Traits>
void gen_bto_aux_copy<N, Traits>::put(
    const index<N> &idx,
    rd_block_type &blk,
    const tensor_transf<N, element_type> &tr) {

    typedef typename Traits::template to_copy_type<N>::type to_copy_type;

    if(!m_open) {
        throw block_stream_exception(g_ns, k_clazz, "put()",
            __FILE__, __LINE__, "Stream is not ready.");
    }

    size_t aidx = abs_index<N>::get_abs_index(idx, m_bidims);
    typename std::map<size_t, block_status>::iterator ibs = m_blkstat.end();

    if(m_sync) {
        libutil::auto_lock<libutil::mutex> lock(m_mtx);
        ibs = m_blkstat.find(aidx);
        if(ibs == m_blkstat.end()) {
            block_status bs;
            bs.mtx = new libutil::mutex;
            bs.touched = false;
            ibs = m_blkstat.insert(std::make_pair(aidx, bs)).first;
        }
    } else {
        ibs = m_blkstat.find(aidx);
        if(ibs == m_blkstat.end()) {
            block_status bs;
            bs.mtx = 0;
            bs.touched = false;
            ibs = m_blkstat.insert(std::make_pair(aidx, bs)).first;
        }
    }

    if(m_sync) {
        libutil::auto_lock<libutil::mutex> lock(*ibs->second.mtx);
        wr_block_type &blk_tgt = m_ctrl.req_block(idx);
        to_copy_type(blk, tr).perform(!ibs->second.touched, blk_tgt);
        m_ctrl.ret_block(idx);
        ibs->second.touched = true;
    } else {
        wr_block_type &blk_tgt = m_ctrl.req_block(idx);
        to_copy_type(blk, tr).perform(!ibs->second.touched, blk_tgt);
        m_ctrl.ret_block(idx);
        ibs->second.touched = true;
    }
}


} // namespace libtensor

#endif // LIBTENSOR_GEN_BTO_AUX_COPY_IMPL_H
