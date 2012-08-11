#include <libtensor/symmetry/er_merge.h>
#include <libtensor/symmetry/point_group_table.h>
#include "er_merge_test.h"

namespace libtensor {


void er_merge_test::perform() throw(libtest::test_exception) {

    test_1();
    test_2();

}


/** \brief Merge 4 dimensions in 2 steps (merge dims can be simplified)
 **/
void er_merge_test::test_1() throw(libtest::test_exception) {

    static const char *testname = "er_merge_test::test_1()";

    typedef product_table_i::label_set_t label_set_t;
    typedef product_table_i::label_group_t label_group_t;

    try {

    evaluation_rule<4> r1;
    {
        sequence<4, size_t> seq1(1), seq2(0), seq3(0);
        seq2[0] = seq2[2] = 1; seq3[1] = seq3[3] = 1;
        product_rule<4> &pr1 = r1.new_product();
        pr1.add(seq1, 0);
        product_rule<4> &pr2 = r1.new_product();
        pr2.add(seq2, 1);
        pr2.add(seq3, 3);
    }

    sequence<4, size_t> mmap(0);
    mmap[0] = 0; mmap[1] = 0; mmap[2] = 1; mmap[3] = 1;
    mask<2> smsk;
    smsk[0] = smsk[1] = true;

    evaluation_rule<2> r2;
    er_merge<4, 2>(r1, mmap, smsk).perform(r2);

    // Check sequence list
    const eval_sequence_list<2> &sl = r2.get_sequences();
    if (sl.size() != 1) {
        fail_test(testname, __FILE__, __LINE__, "# seq.");
    }
    if (sl[0][0] != 1 || sl[0][1] != 1) {
        fail_test(testname, __FILE__, __LINE__, "seq.");
    }

    // Check product list
    evaluation_rule<2>::const_iterator it = r2.begin();
    if (it == r2.end()) {
        fail_test(testname, __FILE__, __LINE__, "Empty product list.");
    }
    const product_rule<2> &pr1 = r2.get_product(it);
    it++;
    if (it != r2.end()) {
        fail_test(testname, __FILE__, __LINE__, "More than one products.");
    }

    product_rule<2>::iterator ip1 = pr1.begin();
    if (ip1 == pr1.end()) {
        fail_test(testname, __FILE__, __LINE__, "Empty product pr1.");
    }
    if (pr1.get_intrinsic(ip1) != product_table_i::k_invalid)
        fail_test(testname, __FILE__, __LINE__, "Intrinsic label.");
    ip1++;
    if (ip1 != pr1.end()) {
        fail_test(testname, __FILE__, __LINE__, "Too many terms in pr1.");
    }

    } catch(exception &e) {
        fail_test(testname, __FILE__, __LINE__, e.what());
    }
}


/** \brief Merge 4 dimensions in 2 steps (1 merge cannot be simplified)
 **/
void er_merge_test::test_2() throw(libtest::test_exception) {


    static const char *testname = "er_merge_test::test_2()";

    typedef product_table_i::label_set_t label_set_t;
    typedef product_table_i::label_group_t label_group_t;

    try {

    evaluation_rule<5> r1;
    {
        sequence<5, size_t> seq1(1), seq2(0), seq3(0);
        seq1[3] = seq1[4] = 0;
        seq2[3] = seq2[4] = 1;
        seq3[0] = seq3[4] = 1;
        product_rule<5> &pr1 = r1.new_product();
        pr1.add(seq1, 1);
        product_rule<5> &pr2 = r1.new_product();
        pr2.add(seq2, 2);
        pr2.add(seq3, 3);
    }

    sequence<5, size_t> mmap(0);
    mmap[0] = 0; mmap[1] = 0; mmap[2] = 1; mmap[3] = 2; mmap[4] = 2;
    mask<3> smsk;
    smsk[0] = true;

    evaluation_rule<3> r2;
    er_merge<5, 3>(r1, mmap, smsk).perform(r2);

    // Check sequence list
    const eval_sequence_list<3> &sl = r2.get_sequences();
    if (sl.size() != 3) {
        fail_test(testname, __FILE__, __LINE__, "# seq.");
    }
    if (sl[0][0] != 0 || sl[0][1] != 1 || sl[0][2] != 0) {
        fail_test(testname, __FILE__, __LINE__, "seq[0].");
    }
    if (sl[1][0] != 0 || sl[1][1] != 0 || sl[1][2] != 2) {
        fail_test(testname, __FILE__, __LINE__, "seq[1].");
    }
    if (sl[2][0] != 1 || sl[2][1] != 0 || sl[2][2] != 1) {
        fail_test(testname, __FILE__, __LINE__, "seq[2].");
    }

    // Check product list
    evaluation_rule<3>::const_iterator it = r2.begin();
    if (it == r2.end()) {
        fail_test(testname, __FILE__, __LINE__, "Empty product list.");
    }
    const product_rule<3> &pr1 = r2.get_product(it);
    it++;
    if (it == r2.end()) {
        fail_test(testname, __FILE__, __LINE__, "Only one product.");
    }
    const product_rule<3> &pr2 = r2.get_product(it);
    it++;
    if (it != r2.end()) {
        fail_test(testname, __FILE__, __LINE__, "More than one products.");
    }

    product_rule<3>::iterator ip = pr1.begin();
    if (ip == pr1.end()) {
        fail_test(testname, __FILE__, __LINE__, "Empty product pr1.");
    }
    if (pr1.get_intrinsic(ip) != 1)
        fail_test(testname, __FILE__, __LINE__, "Intrinsic label.");
    ip++;
    if (ip != pr1.end()) {
        fail_test(testname, __FILE__, __LINE__, "Too many terms in pr1.");
    }

    ip = pr2.begin();
    if (ip == pr2.end()) {
        fail_test(testname, __FILE__, __LINE__, "Empty product pr2.");
    }
    if (pr2.get_intrinsic(ip) != 2)
        fail_test(testname, __FILE__, __LINE__, "Intrinsic label.");
    ip++;
    if (pr2.get_intrinsic(ip) != 3)
        fail_test(testname, __FILE__, __LINE__, "Intrinsic label.");
    ip++;
    if (ip != pr2.end()) {
        fail_test(testname, __FILE__, __LINE__, "Too many terms in pr2.");
    }


    } catch(exception &e) {
        fail_test(testname, __FILE__, __LINE__, e.what());
    }
}


} // namespace libtensor