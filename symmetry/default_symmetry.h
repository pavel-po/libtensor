#ifndef LIBTENSOR_DEFAULT_SYMMETRY_H
#define LIBTENSOR_DEFAULT_SYMMETRY_H

#include "defs.h"
#include "exception.h"
#include "symmetry_base.h"

namespace libtensor {

/**	\brief Default symmetry in block tensors (no symmetry)

	Simple implementation of empty symmetry. It provides no relationships
	among the blocks of a block %tensor making each block unique. Useful
	for testing and debugging.

	\ingroup libtensor
 **/
template<size_t N, typename T>
class default_symmetry : public symmetry_base< N, T, default_symmetry<N, T> > {
private:
	class handler : public orbit_iterator_handler_i<N, T> {
	public:
		virtual void on_begin(index<N> &idx,
			const dimensions<N> &dims) const;
		virtual bool on_end(const index<N> &idx,
			const dimensions<N> &dims) const;
		virtual void on_next(index<N> &idx,
			const dimensions<N> &dims) const;
	};

private:
	handler m_handler; //!< Iterator handler

public:
	//!	\name Implementation of symmetry_i<N, T>
	//@{

	virtual void disable_symmetry();
	virtual void enable_symmetry();
	virtual orbit_iterator<N, T> get_orbits(const dimensions<N> &dims)
		const;

	//@}

};

template<size_t N, typename T>
void default_symmetry<N, T>::disable_symmetry() {

}

template<size_t N, typename T>
void default_symmetry<N, T>::enable_symmetry() {

}

template<size_t N, typename T>
orbit_iterator<N, T> default_symmetry<N, T>::get_orbits(
	const dimensions<N> &dims) const {

	return orbit_iterator<N, T>(m_handler, dims);
}

template<size_t N, typename T>
void default_symmetry<N, T>::handler::on_begin(index<N> &idx,
	const dimensions<N> &dims) const {

}

template<size_t N, typename T>
bool default_symmetry<N, T>::handler::on_end(index<N> &idx,
	const dimensions<N> &dims) const {

	return false;
}

template<size_t N, typename T>
void default_symmetry<N, T>::handler::on_next(index<N> &idx,
	const dimensions<N> &dims) const {

}

} // namespace libtensor

#endif // LIBTENSOR_DEFAULT_SYMMETRY_H