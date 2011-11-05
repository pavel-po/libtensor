#ifndef LIBTENSOR_SO_ADD_IMPL_PERM_H
#define LIBTENSOR_SO_ADD_IMPL_PERM_H

#include "../../defs.h"
#include "../../exception.h"
#include "../symmetry_element_set_adapter.h"
#include "../symmetry_operation_impl_base.h"
#include "../so_add.h"
#include "../se_perm.h"
#include "permutation_group.h"

namespace libtensor {


/**	\brief Implementation of so_add<N, T> for se_perm<N, T>
	\tparam N Tensor order.
	\tparam T Tensor element type.

	\ingroup libtensor_symmetry
 **/
template<size_t N, typename T>
class symmetry_operation_impl< so_add<N, T>, se_perm<N, T> > :
public symmetry_operation_impl_base< so_add<N, T>, se_perm<N, T> > {

public:
    static const char *k_clazz; //!< Class name

public:
    typedef so_add<N, T> operation_t;
    typedef se_perm<N, T> element_t;
    typedef symmetry_operation_params<operation_t>
    symmetry_operation_params_t;

protected:
    virtual void do_perform(symmetry_operation_params_t &params) const;
};


template<size_t N, typename T>
const char *symmetry_operation_impl< so_add<N, T>, se_perm<N, T> >::k_clazz =
        "symmetry_operation_impl< so_add<N, T>, se_perm<N, T> >";


template<size_t N, typename T>
void symmetry_operation_impl< so_add<N, T>, se_perm<N, T> >::do_perform(
        symmetry_operation_params_t &params) const {

    static const char *method =
            "do_perform(const symmetry_operation_params_t&)";

    typedef symmetry_element_set_adapter< N, T, se_perm<N, T> > adapter_t;
    adapter_t adapter1(params.grp1);
    adapter_t adapter2(params.grp2);

    //	G3 = (P1 G1) U (P2 G2)
    //	P2* G3 = (P2* P1 G1) U G2

    permutation<N> perm12(params.perm1), pinv2(params.perm2, true);
    perm12.permute(pinv2);

    permutation_group<N, T> grp1(adapter1);
    permutation_group<N, T> grp3;
    grp1.permute(perm12);

    /*
     * THIS IS A QUICK WORK-AROUND FOR FINDING THE CORRECT INTERSECTION OF TWO
     * PERMUTATION GROUPS! A PROPER ALGORITHM SHOULD BE IMPLEMENTED LATER IN
     * CLASS PERMUTATION_GROUP.
     */
    // Generate a list of all permutations in group 2
    typedef std::pair<permutation<N>, bool> signed_perm_t;
    typedef std::list<signed_perm_t> perm_list_t;

    perm_list_t lst;
    for(typename adapter_t::iterator i = adapter2.begin();
            i != adapter2.end(); i++) {
        const se_perm<N, T> &e = adapter2.get_elem(i);
        lst.push_back(signed_perm_t(e.get_perm(), e.is_symm()));
    }

    bool not_done;
    do {
        not_done = false;
        for(typename adapter_t::iterator i = adapter2.begin();
                i != adapter2.end(); i++) {

            const se_perm<N, T> &e = adapter2.get_elem(i);
            for (typename perm_list_t::iterator j = lst.begin();
                    j != lst.end(); j++) {

                permutation<N> p(j->first);
                p.permute(e.get_perm());

                typename perm_list_t::const_iterator k = lst.begin();
                for (; k != lst.end(); k++)
                    if (p.equals(k->first)) break;

                if (k == lst.end()) {
                    lst.push_back(signed_perm_t(p, j->second == e.is_symm()));
                    not_done = true;
                }
            }
        }
    } while (not_done);

    for (typename perm_list_t::const_iterator i = lst.begin();
            i != lst.end(); i++) {

        if (grp1.is_member(i->second, i->first))
            grp3.add_orbit(i->second, i->first);
    }

    params.grp3.clear();
    grp3.permute(params.perm2);
    grp3.convert(params.grp3);
}


} // namespace libtensor

#endif // LIBTENSOR_SO_ADD_IMPL_PERM_H
