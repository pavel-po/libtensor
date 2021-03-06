#ifndef LIBTENSOR_CTF_BTOD_TRAITS_H
#define LIBTENSOR_CTF_BTOD_TRAITS_H

#include <libtensor/core/allocator.h>
#include <libtensor/core/scalar_transf_double.h>
#include <libtensor/ctf_dense_tensor/ctf_dense_tensor.h>
#include <libtensor/block_tensor/btod_contract2_clst_optimize.h>
#include "ctf_block_tensor.h"

namespace libtensor {


template<size_t N, size_t M, size_t K> class ctf_tod_contract2_streamed;
template<size_t N> class ctf_tod_copy;
template<size_t N, size_t M> class ctf_tod_diag;
template<size_t N, size_t M> class ctf_tod_dirsum;
template<size_t N> class ctf_tod_dotprod;
template<size_t N, size_t M, size_t K> class ctf_tod_ewmult2;
template<size_t N> class ctf_tod_mult;
template<size_t N> class ctf_tod_mult1;
template<size_t N> class ctf_tod_random;
template<size_t N, size_t M> class ctf_tod_scatter;
template<size_t N> class ctf_tod_scale;
template<size_t N> class ctf_tod_set;
template<size_t N> class ctf_tod_set_diag;
template<size_t N> class ctf_tod_trace;


struct ctf_btod_traits {

    //! Element type
    typedef double element_type;

    //! Block tensor interface traits
    typedef ctf_block_tensor_i_traits<double> bti_traits;

    //! Type of temporary block tensor
    template<size_t N>
    struct temp_block_tensor_type {
        typedef ctf_block_tensor<N, double> type;
    };

    //! Type of temporary block
    template<size_t N>
    struct temp_block_type {
        typedef ctf_dense_tensor<N, double> type;
    };

/*
    template<size_t N, typename Functor>
    struct to_apply_type {
        typedef tod_apply<N, Functor> type;
    };
 */

    template<size_t N, size_t M, size_t K>
    struct to_contract2_type {
        typedef ctf_tod_contract2_streamed<N, M, K> type;
        typedef btod_contract2_clst_optimize<N, M, K> clst_optimize_type;
    };

    template<size_t N>
    struct to_copy_type {
        typedef ctf_tod_copy<N> type;
    };

    template<size_t N, size_t M>
    struct to_diag_type {
        typedef ctf_tod_diag<N, M> type;
    };

    template<size_t N, size_t M>
    struct to_dirsum_type {
        typedef ctf_tod_dirsum<N, M> type;
    };

    template<size_t N>
    struct to_dotprod_type {
        typedef ctf_tod_dotprod<N> type;
    };

    template<size_t N, size_t M, size_t K>
    struct to_ewmult2_type {
        typedef ctf_tod_ewmult2<N, M, K> type;
    };

/*
    template<size_t N, size_t M>
    struct to_extract_type {
        typedef tod_extract<N, M> type;
    };
 */

    template<size_t N>
    struct to_mult_type {
        typedef ctf_tod_mult<N> type;
    };

    template<size_t N>
    struct to_mult1_type {
        typedef ctf_tod_mult1<N> type;
    };

    template<size_t N>
    struct to_random_type {
        typedef ctf_tod_random<N> type;
    };

    template<size_t N>
    struct to_scale_type {
        typedef ctf_tod_scale<N> type;
    };

    template<size_t N, size_t M>
    struct to_scatter_type {
        typedef ctf_tod_scatter<N, M> type;
    };

/*
    template<size_t N, typename ComparePolicy>
    struct to_select_type {
        typedef tod_select<N, ComparePolicy> type;
    };
 */

    template<size_t N>
    struct to_set_diag_type {
        typedef ctf_tod_set_diag<N> type;
    };

/*
    template<size_t N>
    struct to_set_elem_type {
        typedef tod_set_elem<N> type;
    };
 */

    template<size_t N>
    struct to_set_type {
        typedef ctf_tod_set<N> type;
    };

    template<size_t N>
    struct to_trace_type {
        typedef ctf_tod_trace<N> type;
    };

    static bool is_zero(double d) {
        return d == 0.0;
    }

    static bool is_zero(const scalar_transf<double> &d) {
        return is_zero(d.get_coeff());
    }

    static double zero() {
        return 0.0;
    }

    static double identity() {
        return 1.0;
    }

};


} // namespace libtensor

#endif // LIBTENSOR_CTF_BTOD_TRAITS_H
