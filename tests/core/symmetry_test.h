#ifndef LIBTENSOR_SYMMETRY_TEST_H
#define LIBTENSOR_SYMMETRY_TEST_H

#include <libtest/unit_test.h>

namespace libtensor {

/** \brief Tests the libtensor::symmetry class

    \ingroup libtensor_tests_core
**/
class symmetry_test : public libtest::unit_test {
public:
    virtual void perform() throw(libtest::test_exception);

private:
    void test_1() throw(libtest::test_exception);
    void test_2() throw(libtest::test_exception);
    void test_3() throw(libtest::test_exception);
    void test_4() throw(libtest::test_exception);
    void test_5() throw(libtest::test_exception);
};

} // namespace libtensor

#endif // LIBTENSOR_SYMMETRY_TEST_H
