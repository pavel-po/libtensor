#ifndef LIBTENSOR_MAGIC_DIMENSIONS_H
#define LIBTENSOR_MAGIC_DIMENSIONS_H

#include "dimensions.h"
#include "out_of_bounds.h"

namespace libtensor {


/** \brief Tensor dimensions class with some magic for fast index transformation

    Index transformations from the regular to the absolute form can become
    a bottleneck in calculations with small blocks. The hotspot is integer
    divisions. This class uses libdivide to alleviate the problem.

    Magic dimensions are created based on regular dimensions with one parameter:
     - incs = true: use tensor modes' linear increments (can be used to convert
       absolute indexes to multi-indexes via abs_index<N>::get_index).
     - incs = false: use tensor dimensions (can be used to find to which block
       an index belongs).

    \sa abs_index, dimensions, index

    \ingroup libtensor_core
 **/
template<size_t N>
class magic_dimensions {
private:
    dimensions<N> m_dims;
    bool m_incs;
    void *m_magic;

public:
    /** \brief Constructs magic dimensions from regular dimensions
        \param dims Dimensions.
        \param incs Use linear increments (true) or dimensions (false).
     **/
    magic_dimensions(const dimensions<N> &dims, bool incs);

    /** \brief Copy constructor
     **/
    magic_dimensions(const magic_dimensions<N> &mdims);

    /** \brief Destructor
     **/
    ~magic_dimensions();

    /** \brief Returns the dimensions
     **/
    const dimensions<N> &get_dims() const {
        return m_dims;
    }

    /** \brief Permutes the %imensions
     **/
    void permute(const permutation<N> &p);

    /** \brief Divides each element of index i1 by the dimensions and puts
            the result in i2.
     **/
    void divide(const index<N> &i1, index<N> &i2) const;

    /** \brief Divides number n by ith dimension
     **/
    size_t divide(size_t n, size_t i) const;

private:
    void make_magic();

private:
    const magic_dimensions<N> &operator=(const magic_dimensions<N> &mdims);

};


template<>
class magic_dimensions<0> {
private:
    dimensions<0> m_dims;

public:
    magic_dimensions(const dimensions<0> &dims, bool incs) : m_dims(dims) {

    }

    const dimensions<0> &get_dims() const {
        return m_dims;
    }

    void permute(const permutation<0> &p) {

    }

    void divide(const index<0> &i1, index<0> &i2) const {

    }

    size_t divide(size_t n, size_t i) const {
        throw out_of_bounds(g_ns, "magic_dimensions<0>",
            "divide(size_t, size_t)", __FILE__, __LINE__, "i");
        return 0;
    }

};


} // namespace libtensor

#endif // LIBTENSOR_MAGIC_DIMENSIONS_H
