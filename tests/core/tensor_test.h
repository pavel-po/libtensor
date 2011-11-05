#ifndef LIBTENSOR_TENSOR_TEST_H
#define LIBTENSOR_TENSOR_TEST_H

#include <libtest/unit_test.h>
#include <libtensor/core/tensor.h>

namespace libtensor {

/**	\brief Tests the libtensor::tensor class

	\ingroup libtensor_tests_core
**/
class tensor_test : public libtest::unit_test {
public:
	virtual void perform() throw(libtest::test_exception);

private:
	//!	Tests the constructor
	void test_ctor() throw(libtest::test_exception);

	//!	Tests immutability
	void test_immutable() throw(libtest::test_exception);

	//!	Tests operations
	void test_operation() throw(libtest::test_exception);

	void test_1() throw(libtest::test_exception);
	void test_2() throw(libtest::test_exception);

};

} // namespace libtensor

#endif // LIBTENSOR_TENSOR_TEST_H

