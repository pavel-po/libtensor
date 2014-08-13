#ifndef NODE_UNBLOCK_H
#define NODE_UNBLOCK_H

#include "node.h"

namespace libtensor {
namespace expr {


/** \brief Tensor expression node: unblocking

    \ingroup libtensor_expr_dag
 **/
class node_unblock : public node {
public:
    static const char k_op_type[]; //!< Operation type

private:
    size_t m_subspace;
public:
    /** \brief Creates an unblock node
        \param n Order of result.
     **/
    node_unblock(size_t n,size_t subspace) :
        node(k_op_type, n),
        m_subspace(subspace)
    { }

    /** \brief Virtual destructor
     **/
    virtual ~node_unblock() { }

    /** \brief Creates a copy of the node via new
     **/
    virtual node *clone() const {
        return new node_unblock(*this);
    }

};


} // namespace expr
} // namespace libtensor


#endif /* NODE_UNBLOCK_H */
