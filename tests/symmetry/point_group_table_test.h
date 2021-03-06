#ifndef LIBTENSOR_POINT_GROUP_TABLE_TEST_H
#define LIBTENSOR_POINT_GROUP_TABLE_TEST_H

#include <libtest/unit_test.h>

namespace libtensor {


/** \brief Tests the libtensor::point_group_table class

    \ingroup libtensor_tests_sym
 **/
class point_group_table_test : public libtest::unit_test {
public:
    virtual void perform() throw(libtest::test_exception);

private:
    void test_1() throw(libtest::test_exception);
    void test_2() throw(libtest::test_exception);
    void test_3() throw(libtest::test_exception);
    void test_4() throw(libtest::test_exception);
};


} // namespace libtensor

#endif // LIBTENSOR_POINT_GROUP_TABLE_TEST_H
