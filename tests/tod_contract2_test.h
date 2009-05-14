#ifndef LIBTENSOR_TOD_CONTRACT2_TEST_H
#define LIBTENSOR_TOD_CONTRACT2_TEST_H

#include <sstream>
#include <libtest.h>
#include "tod_contract2.h"

namespace libtensor {

/**	\brief Tests the libtensor::tod_contract2 class

	\ingroup libtensor_tests
**/
class tod_contract2_test : public libtest::unit_test {
public:
	virtual void perform() throw(libtest::test_exception);

private:
	// c_{ij} = \sum_{pq} a_{pq} b_{ijpq}
	void test_ij_pq_ijpq(size_t ni, size_t nj, size_t np, size_t nq)
		throw(libtest::test_exception);

	// c_{ij} = \sum_{pqr} a_{ipqr} b_{jpqr}
	void test_ij_ipqr_jpqr(size_t ni, size_t nj, size_t np, size_t nq,
		size_t nr) throw(libtest::test_exception);

	// c_{ij} = \sum_{pqr} a_{pqir} b_{pqjr}
	void test_ij_pqir_pqjr(size_t ni, size_t nj, size_t np, size_t nq,
		size_t nr) throw(libtest::test_exception);

	// c_{jikl} = \sum_{p} a_{pi} b_{jpkl}
	void test_jikl_pi_jpkl(size_t ni, size_t nj, size_t nk, size_t nl,
		size_t np) throw(libtest::test_exception);

	// c_{ijkl} = \sum_{p} a_{ijp} b_{klp}
	void test_ijkl_ijp_klp(size_t ni, size_t nj, size_t nk, size_t nl,
		size_t np) throw(libtest::test_exception);

	template<size_t N>
	void compare_ref(const char *test, tensor_i<N, double> &t,
		tensor_i<N, double> &t_ref, double thresh)
		throw(libtest::test_exception);
};

template<size_t N>
inline void tod_contract2_test::compare_ref(const char *test,
	tensor_i<N, double> &t, tensor_i<N, double> &t_ref, double thresh)
	throw(libtest::test_exception) {

	tod_compare<N> cmp(t, t_ref, thresh);
	if(!cmp.compare()) {
		std::ostringstream ss1, ss2;
		ss2 << "Result does not match reference at element "
			<< cmp.get_diff_index() << ": "
			<< cmp.get_diff_elem_1() << " (act) vs. "
			<< cmp.get_diff_elem_2() << " (ref), "
			<< cmp.get_diff_elem_1() - cmp.get_diff_elem_2()
			<< " (diff) in " << test;
		fail_test("tod_contract2_test::compare_ref()",
			__FILE__, __LINE__, ss2.str().c_str());
	}

}

} // namespace libtensor

#endif // LIBTENSOR_TOD_CONTRACT2_TEST_H

