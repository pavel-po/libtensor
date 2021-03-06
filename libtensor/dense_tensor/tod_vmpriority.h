#ifndef LIBTENSOR_TOD_VMPRIORITY_H
#define LIBTENSOR_TOD_VMPRIORITY_H

#include <libtensor/core/noncopyable.h>
#include "dense_tensor_i.h"

namespace libtensor {


/** \brief Sets/unsets in-core virtual memory priority on tensor data
    \tparam N Tensor order

    \ingroup libtensor_tod
 **/
template<size_t N>
class tod_vmpriority : public noncopyable {
private:
    dense_tensor_base_i<N, double> &m_t; //!< Tensor

public:
    /** \brief Initializes the operation
     **/
    tod_vmpriority(dense_tensor_base_i<N, double> &t) : m_t(t) { }

    /** \brief Sets in-core priority on tensor data
     **/
    void set_priority();

    /** \brief Unsets in-core priority on tensor data
     **/
    void unset_priority();
};


} // namespace libtensor

#endif // LIBTENSOR_TOD_VMPRIORITY_H

