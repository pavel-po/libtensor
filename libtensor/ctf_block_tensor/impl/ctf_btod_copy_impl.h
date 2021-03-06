#ifndef LIBTENSOR_CTF_BTOD_COPY_IMPL_H
#define LIBTENSOR_CTF_BTOD_COPY_IMPL_H

#include <libtensor/ctf_dense_tensor/ctf_tod_copy.h>
#include <libtensor/ctf_dense_tensor/ctf_tod_set.h>
#include <libtensor/gen_block_tensor/gen_bto_aux_add.h>
#include <libtensor/gen_block_tensor/gen_bto_aux_copy.h>
#include <libtensor/block_tensor/block_tensor_ctrl.h>
#include "ctf_btod_set_symmetry.h"
#include "../ctf_btod_copy.h"

namespace libtensor {


template<size_t N>
const char ctf_btod_copy<N>::k_clazz[] = "ctf_btod_copy<N>";


template<size_t N>
void ctf_btod_copy<N>::perform(gen_block_tensor_i<N, bti_traits> &btb) {

    gen_bto_aux_copy<N, ctf_btod_traits> out(get_symmetry(), btb);
    out.open();
    ctf_btod_set_symmetry<N>().perform(get_schedule(), btb);
    perform(out);
    out.close();
}


template<size_t N>
void ctf_btod_copy<N>::perform(gen_block_tensor_i<N, bti_traits> &btb,
    const scalar_transf<double> &c) {

    gen_block_tensor_rd_ctrl<N, bti_traits> cb(btb);
    std::vector<size_t> nzblkb;
    cb.req_nonzero_blocks(nzblkb);
    addition_schedule<N, ctf_btod_traits> asch(get_symmetry(),
        cb.req_const_symmetry());
    asch.build(get_schedule(), nzblkb);

    gen_bto_aux_add<N, ctf_btod_traits> out(get_symmetry(), asch, btb, c);
    out.open();
    ctf_btod_set_symmetry<N>().perform(asch, btb);
    perform(out);
    out.close();
}


} // namespace libtensor

#endif // LIBTENSOR_CTF_BTOD_COPY_IMPL_H
