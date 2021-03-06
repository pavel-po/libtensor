#include <libtensor/core/allocator.h>
#include <libtensor/core/scalar_transf_double.h>
#include <libtensor/block_tensor/block_tensor.h>
#include <libtensor/block_tensor/btod_vmpriority.h>
#include <libtensor/block_tensor/btod_random.h>
#include <libtensor/dense_tensor/tod_btconv.h>
#include "btod_vmpriority_test.h"
#include "../compare_ref.h"

namespace libtensor {


void btod_vmpriority_test::perform() throw(libtest::test_exception) {

    allocator<double>::init(4, 16, 65536, 65536);
    try {

    test_1();

    } catch (...) {
        allocator<double>::shutdown();
        throw;
    }
    allocator<double>::shutdown();
}


void btod_vmpriority_test::test_1() throw(libtest::test_exception) {

    static const char *testname = "btod_vmpriority_test::test_1()";

    typedef allocator<double> allocator_t;

    try {

    index<2> i1, i2;
    i2[0] = 9; i2[1] = 9;
    dimensions<2> dims(index_range<2>(i1, i2));
    block_index_space<2> bis(dims);
    dimensions<2> bidims(bis.get_block_index_dims());

    block_tensor<2, double, allocator_t> bta(bis);
    dense_tensor<2, double, allocator_t> ta(dims), ta_ref(dims);

    btod_random<2>().perform(bta);

    //    Prepare the reference

    tod_btconv<2>(bta).perform(ta_ref);

    //    Invoke the operation

    btod_vmpriority<2>(bta).set_priority();
    tod_btconv<2>(bta).perform(ta);

    //    Compare against the reference

    compare_ref<2>::compare(testname, ta, ta_ref, 1e-15);

    } catch(exception &e) {
        fail_test(testname, __FILE__, __LINE__, e.what());
    }
}


} // namespace libtensor
