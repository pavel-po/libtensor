#ifndef LIBTENSOR_SO_ADD_IMPL_LABEL_H
#define LIBTENSOR_SO_ADD_IMPL_LABEL_H

#include "../../defs.h"
#include "../../exception.h"
#include "../symmetry_element_set_adapter.h"
#include "../symmetry_operation_impl_base.h"
#include "../so_add.h"
#include "../se_label.h"

namespace libtensor {

/**	\brief Implementation of so_add<N, T> for se_label<N, T>
	\tparam N Tensor order.
	\tparam T Tensor element type.

	\ingroup libtensor_symmetry
 **/
template<size_t N, typename T>
class symmetry_operation_impl< so_add<N, T>, se_label<N, T> > :
public symmetry_operation_impl_base< so_add<N, T>, se_label<N, T> > {

public:
    static const char *k_clazz; //!< Class name

public:
    typedef so_add<N, T> operation_t;
    typedef se_label<N, T> element_t;
    typedef symmetry_operation_params<operation_t>
    symmetry_operation_params_t;

protected:
    virtual void do_perform(symmetry_operation_params_t &params) const;
};


template<size_t N, typename T>
const char *symmetry_operation_impl< so_add<N, T>, se_label<N, T> >::k_clazz =
        "symmetry_operation_impl< so_add<N, T>, se_label<N, T> >";


template<size_t N, typename T>
void symmetry_operation_impl< so_add<N, T>, se_label<N, T> >::do_perform(
        symmetry_operation_params_t &params) const {

    static const char *method =
            "do_perform(const symmetry_operation_params_t&)";

    typedef symmetry_element_set_adapter< N, T, se_label<N, T> > adapter_t;
    adapter_t g1(params.grp1);
    adapter_t g2(params.grp2);
    params.grp3.clear();

    sequence<N, size_t> map(0);
    for (size_t i = 0; i < N; i++) map[i] = i;
    params.perm1.apply(map);

    for (typename adapter_t::iterator it1 = g1.begin();
            it1 != g1.end(); it1++) {

        const se_label<N, T> &e1 = g1.get_elem(it1);

        typename adapter_t::iterator it2 = g2.begin();
        for(; it2 != g2.end(); it2++) {

            if (e1.get_table_id() == g2.get_elem(it2).get_table_id())
                break;
        }

        if (it2 == g2.end()) continue;

        const se_label<N, T> &e2 = g2.get_elem(it2);

        // create new se_label element by copying e2 and permuting
        se_label<N, T> e3(e2);
        e3.permute(params.perm2);

        // check if the labeling of e1 and e2 matches
        for (size_t i = 0; i < N; i++) {
            size_t type1 = e1.get_dim_type(map[i]);
            size_t type = e3.get_dim_type(i);
            if (e1.get_dim(type1) != e3.get_dim(type))
                throw bad_symmetry(g_ns, k_clazz, method,
                        __FILE__, __LINE__, "Incompatible dimensions.");

            for (size_t j = 0; j < e3.get_dim(type); j++) {
                if (e1.get_label(type1, j) != e3.get_label(type, j))
                    throw bad_symmetry(g_ns, k_clazz, method,
                            __FILE__, __LINE__, "Incompatible labeling.");
            }
        }

        e3.delete_target();

        size_t nlabels = e3.get_n_labels();
        // if no target labels are given every block is discarded
        if (e1.get_n_targets() != 0 && e2.get_n_targets() != 0) {

            for (size_t i = 0; i < e1.get_n_targets(); i++)
                e3.add_target(e1.get_target(i));

            for (size_t i = 0; i < e2.get_n_targets(); i++)
                e3.add_target(e2.get_target(i));
        }

        params.grp3.insert(e3);
    }
}


} // namespace libtensor

#endif // LIBTENSOR_SO_ADD_IMPL_LABEL_H
