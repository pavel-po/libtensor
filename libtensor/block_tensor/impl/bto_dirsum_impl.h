#ifndef LIBTENSOR_BTO_DIRSUM_IMPL_H
#define LIBTENSOR_BTO_DIRSUM_IMPL_H

#include <libtensor/gen_block_tensor/gen_bto_aux_add.h>
#include <libtensor/gen_block_tensor/gen_bto_aux_copy.h>
#include "../bto_dirsum.h"

namespace libtensor {


template<size_t N, size_t M, typename T>
const char bto_dirsum<N, M, T>::k_clazz[] = "bto_dirsum<N, M, T>";


template<size_t N, size_t M, typename T>
void bto_dirsum<N, M, T>::perform(gen_block_tensor_i<N + M, bti_traits> &btb) {

    gen_bto_aux_copy<N + M, bto_traits<T> > out(get_symmetry(), btb);
    out.open();
    perform(out);
    out.close();
}


template<size_t N, size_t M, typename T>
void bto_dirsum<N, M, T>::perform(gen_block_tensor_i<N + M, bti_traits> &btb,
    const scalar_transf<T> &c) {

    typedef block_tensor_i_traits<T> bti_traits;

    gen_block_tensor_rd_ctrl<N + M, bti_traits> cb(btb);
    std::vector<size_t> nzblkb;
    cb.req_nonzero_blocks(nzblkb);
    addition_schedule<N + M, bto_traits<T> > asch(get_symmetry(),
        cb.req_const_symmetry());
    asch.build(get_schedule(), nzblkb);

    gen_bto_aux_add<N + M, bto_traits<T> > out(get_symmetry(), asch, btb, c);
    out.open();
    perform(out);
    out.close();
}


template<size_t N, size_t M, typename T>
void bto_dirsum<N, M, T>::perform(block_tensor_i<N + M, T> &btb, T c) {

    perform(btb, scalar_transf<T>(c));
}


template<size_t N, size_t M, typename T>
void bto_dirsum<N, M, T>::compute_block(
        bool zero,
        const index<N + M> &ic,
        const tensor_transf<N + M, T> &trc,
        dense_tensor_wr_i<N + M, T> &blkc) {

    m_gbto.compute_block(zero, ic, trc, blkc);
}


} // namespace libtensor


#endif // LIBTENOSR_BTO_DIRSUM_IMPL_H