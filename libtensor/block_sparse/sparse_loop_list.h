/*
 * sparse_loop_list.h
 *
 *  Created on: Nov 13, 2013
 *      Author: smanzer
 */

#ifndef SPARSE_LOOP_LIST_H_
#define SPARSE_LOOP_LIST_H_

#include <vector>
#include "block_loop.h"
#include "sparse_bispace.h"
#include "block_kernel_i.h"

//TODO REMOVE
#include <iostream>

namespace libtensor
{

typedef std::map<size_t, std::vector<size_t> > fixed_block_map;
typedef std::map<size_t,size_t> loop_fixed_block_map;

class sparse_loop_list
{
private:
    static const char* k_clazz; //!< Class name
    std::vector<block_loop> m_loops;
    std::vector<sparse_bispace_any_order> m_bispaces;

	template<typename T>
    void _run_internal(block_kernel_i<T>& kernel,
    				   std::vector<T*>& ptrs,
                       std::vector<T*>& block_ptrs,
    				   std::vector<offset_list>& bispace_grp_offsets,
    				   std::vector<dim_list>& bispace_block_dims,
    				   size_t loop_grp_idx);
    std::vector<std::vector<off_dim_pair_list> > m_offsets_and_sizes; //Each entry contains offsets and sizes of the blocks for each a given loop group
    std::vector<idx_pair_list> m_bispaces_and_index_groups; //Each entry contains the bispaces and index groups touched by a given loop group
    std::vector<idx_pair_list> m_bispaces_and_subspaces; //Each entry contains the bispaces and subspaces touched by a given loop group
    std::vector<std::vector<dim_list> > m_block_dims; //The block sizes of each tensor 
    std::vector<std::vector<offset_list> > m_cur_bispace_grp_offsets; //Used as 'scratch paper' for the offset calculations; pre-alloc for speed
    std::vector<idx_list> m_loops_for_groups;
    idx_pair_list m_loop_bounds; //Used to make some loops iterate over a restricted range for direct code
public:
	sparse_loop_list(const std::vector<block_loop>& loops,const idx_list& direct_tensors = idx_list());

	template<typename T>
	void run(block_kernel_i<T>& kernel,std::vector<T*>& ptrs,const std::map<size_t,idx_pair>& batches = (std::map<size_t,idx_pair>()));

	const std::vector< sparse_bispace_any_order >& get_bispaces() const { return m_bispaces; }
	const std::vector< block_loop >& get_loops() const { return m_loops; }

	//Returns the indices of the loops that access any subspace of the specified bispace
	std::vector<size_t> get_loops_that_access_bispace(size_t bispace_idx) const;

	//Returns the indices of the loops that access any subspace of the specified sparse group
	//within the specified bispace
	std::vector<size_t> get_loops_that_access_group(size_t bispace_idx,size_t group_idx) const;
};

template<typename T>
void sparse_loop_list::run(block_kernel_i<T>& kernel,std::vector<T*>& ptrs,const std::map<size_t,idx_pair>& batches)
{
	if(m_loops.size() == 0)
	{
		throw bad_parameter(g_ns, k_clazz,"run(...)",
				__FILE__, __LINE__, "no loops in loop list");
	}

	//Set up vectors for keeping track of current block indices and dimensions
	size_t n_bispaces = m_bispaces.size();
	std::vector<dim_list> bispace_block_dims(n_bispaces);
	std::vector<dim_list> bispace_grp_offsets(n_bispaces);
	for(size_t bispace_idx = 0; bispace_idx < n_bispaces; ++bispace_idx)
	{
        const sparse_bispace_any_order& bispace = m_bispaces[bispace_idx];
		bispace_block_dims[bispace_idx].resize(bispace.get_order());
		bispace_grp_offsets[bispace_idx].resize(bispace.get_n_index_groups());

        //For offset calculation, each offset entry is initialized to the inner size of its group
        size_t inner_size = 1;
        for(size_t idx_grp_rev_idx = 1; idx_grp_rev_idx <= bispace.get_n_index_groups(); ++idx_grp_rev_idx)
        {
            size_t idx_grp = bispace.get_n_index_groups() - idx_grp_rev_idx;
            bispace_grp_offsets[bispace_idx][idx_grp] = inner_size;
            inner_size *= bispace.get_index_group_dim(idx_grp);
        }
	}

    //Set up loop bounds
    for(size_t grp_idx = 0; grp_idx < m_bispaces_and_subspaces.size(); ++grp_idx)
    {
        m_loop_bounds[grp_idx] = idx_pair(0,m_offsets_and_sizes[grp_idx].size());
    }
    //Restrict the bounds of loops that involve direct tensors
    for(std::map<size_t,idx_pair>::const_iterator it = batches.begin(); it != batches.end(); ++it)
    {
        //Find the loop group containing this loop
        size_t loop_idx = it->first;
        for(size_t grp_idx = 0; grp_idx < m_offsets_and_sizes.size(); ++grp_idx)
        {
            const idx_list& loops_for_group = m_loops_for_groups[grp_idx];
            idx_list::const_iterator loop_pos = find(loops_for_group.begin(),loops_for_group.end(),loop_idx);
            if(loop_pos != loops_for_group.end())
            {
                std::cout << "Group idx: " << grp_idx << "\n";
                break;
            }
        }
    }

    m_cur_bispace_grp_offsets.resize(m_offsets_and_sizes.size(),bispace_grp_offsets);
    std::vector<T*> block_ptrs(ptrs.size());
	_run_internal(kernel,ptrs,block_ptrs,bispace_grp_offsets,bispace_block_dims,0);
}

template<typename T>
void sparse_loop_list::_run_internal(block_kernel_i<T>& kernel,
                                     std::vector<T*>& tensor_ptrs,
                                     std::vector<T*>& block_ptrs,
                                     std::vector<offset_list>& bispace_grp_offsets,
                                     std::vector<dim_list>& bispace_block_dims,
                                     size_t loop_grp_idx)
{
    //Loop over the significant blocks for this loop group
    const std::vector<off_dim_pair_list>& grp_offsets_and_sizes = m_offsets_and_sizes[loop_grp_idx];
    const std::vector<dim_list>& grp_block_dims = m_block_dims[loop_grp_idx];
    const idx_pair_list& grp_baig = m_bispaces_and_index_groups[loop_grp_idx];
    const idx_pair_list& grp_bas = m_bispaces_and_subspaces[loop_grp_idx];
    for(size_t block_set_idx = m_loop_bounds[loop_grp_idx].first; block_set_idx < m_loop_bounds[loop_grp_idx].second; ++block_set_idx)
    {
        const off_dim_pair_list& block_set_offsets_and_sizes = grp_offsets_and_sizes[block_set_idx];
        const dim_list& block_set_block_dims = grp_block_dims[block_set_idx];
        m_cur_bispace_grp_offsets[loop_grp_idx] = bispace_grp_offsets;
        //Fill in/scale offsets appropriately
        for(size_t baig_idx = 0; baig_idx < grp_baig.size(); ++baig_idx)
        {
            size_t bispace_idx = grp_baig[baig_idx].first;
            size_t idx_grp = grp_baig[baig_idx].second;
            size_t offset = block_set_offsets_and_sizes[baig_idx].first;
            size_t size  = block_set_offsets_and_sizes[baig_idx].second;
            m_cur_bispace_grp_offsets[loop_grp_idx][bispace_idx][idx_grp] *= offset;
            for(size_t faster_grp_idx = idx_grp + 1; faster_grp_idx < m_bispaces[bispace_idx].get_n_index_groups(); ++faster_grp_idx)
            {
                m_cur_bispace_grp_offsets[loop_grp_idx][bispace_idx][faster_grp_idx] *= size;
            }
        }

        //Fill in block sizes
        for(size_t bas_idx = 0; bas_idx < grp_bas.size(); ++bas_idx)
        {
            size_t bispace_idx = grp_bas[bas_idx].first;
            size_t subspace = grp_bas[bas_idx].second;
            size_t block_dim = block_set_block_dims[bas_idx];
            bispace_block_dims[bispace_idx][subspace] = block_dim;
        }

        //Inner loop?
        if(loop_grp_idx == m_offsets_and_sizes.size() - 1)
        {
            //Compute all block offsets
            for(size_t bispace_idx = 0; bispace_idx < m_bispaces.size(); ++bispace_idx)
            {
                size_t offset = 0;
                for(size_t idx_grp = 0; idx_grp < m_bispaces[bispace_idx].get_n_index_groups(); ++idx_grp)
                {
                    offset += m_cur_bispace_grp_offsets[loop_grp_idx][bispace_idx][idx_grp];
                }
                block_ptrs[bispace_idx] = tensor_ptrs[bispace_idx] + offset; 
            }

            //Call kernel
            kernel(block_ptrs,bispace_block_dims);
        }
        else
        {
            //Recurse
            _run_internal(kernel,tensor_ptrs,block_ptrs,m_cur_bispace_grp_offsets[loop_grp_idx],bispace_block_dims,loop_grp_idx+1);
        }
    }
}

} /* namespace libtensor */

#endif /* SPARSE_LOOP_LIST_H_ */
