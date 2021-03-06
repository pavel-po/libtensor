#include "libtensor_core_suite.h"

namespace libtensor {


libtensor_core_suite::libtensor_core_suite() :
    libtest::test_suite("libtensor_core") {

    add_test("abs_index", m_utf_abs_index);
    add_test("block_index_space", m_utf_block_index_space);
    add_test("block_index_subspace_builder",
        m_utf_block_index_subspace_builder);
    add_test("block_map", m_utf_block_map);
    add_test("combined_orbits", m_utf_combined_orbits);
    add_test("contraction2", m_utf_contraction2);
    add_test("contraction2_list_builder", m_utf_contraction2_list_builder);
    add_test("dimensions", m_utf_dimensions);
    add_test("immutable", m_utf_immutable);
    add_test("index", m_utf_index);
    add_test("index_range", m_utf_index_range);
    add_test("magic_dimensions", m_utf_magic_dimensions);
    add_test("mask", m_utf_mask);
    add_test("orbit", m_utf_orbit);
    add_test("orbit_list", m_utf_orbit_list);
    add_test("permutation", m_utf_permutation);
    add_test("permutation_builder", m_utf_permutation_builder);
    add_test("permutation_generator", m_utf_permutation_generator);
    add_test("sequence", m_utf_sequence);
    add_test("sequence_generator", m_utf_sequence_generator);
    add_test("short_orbit", m_utf_short_orbit);
    add_test("subgroup_orbits", m_utf_subgroup_orbits);
    add_test("symmetry", m_utf_symmetry);
    add_test("symmetry_element_set", m_utf_symmetry_element_set);
    add_test("transf_list", m_utf_transf_list);
    add_test("version", m_utf_version);
}


}
