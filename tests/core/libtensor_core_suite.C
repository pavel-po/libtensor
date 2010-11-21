#include <libtensor/libtensor.h>
#include "libtensor_core_suite.h"

namespace libtensor {

libtensor_core_suite::libtensor_core_suite() :
	libtest::test_suite("libtensor_core") {

	add_test("abs_index", m_utf_abs_index);
	add_test("block_index_map", m_utf_block_index_map);
	add_test("block_index_space", m_utf_block_index_space);
	add_test("block_index_subspace_builder",
		m_utf_block_index_subspace_builder);
	add_test("block_map", m_utf_block_map);
	add_test("block_tensor", m_utf_block_tensor);
	add_test("dimensions", m_utf_dimensions);
	add_test("direct_block_tensor", m_utf_direct_block_tensor);
	add_test("global_timings", m_utf_global_timings);
	add_test("immutable", m_utf_immutable);
	add_test("index", m_utf_index);
	add_test("index_range", m_utf_index_range);
	add_test("mapped_block_tensor", m_utf_mapped_block_tensor);
	add_test("mask", m_utf_mask);
	add_test("mp_safe_tensor", m_utf_mp_safe_tensor);
	add_test("orbit", m_utf_orbit);
	add_test("orbit_list", m_utf_orbit_list);
	add_test("permutation", m_utf_permutation);
	add_test("permutation_builder", m_utf_permutation_builder);
	add_test("sequence", m_utf_sequence);
	add_test("symmetry", m_utf_symmetry);
	add_test("symmetry_element_set", m_utf_symmetry_element_set);
	add_test("task_batch", m_utf_task_batch);
	add_test("tensor", m_utf_tensor);
	add_test("timer", m_utf_timer);
	add_test("timings", m_utf_timings);
	add_test("transf_list", m_utf_transf_list);
	add_test("version", m_utf_version);
}

}
