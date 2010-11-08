#ifndef LIBTENSOR_KERN_MUL_IJK_PJI_PK_H
#define LIBTENSOR_KERN_MUL_IJK_PJI_PK_H

#include "kern_mul_ij_pi_pj.h"

namespace libtensor {


class kern_mul_ijk_pji_pk : public kernel_base<2, 1> {
	friend class kern_mul_ijk_pjqi_qpk;
	friend class kern_mul_ijk_pqji_pqk;

public:
	static const char *k_clazz; //!< Kernel name

private:
	double m_d;
	size_t m_ni, m_nj, m_nk, m_np;
	size_t m_spa, m_sja, m_spb, m_sic, m_sjc;

public:
	virtual ~kern_mul_ijk_pji_pk() { }

	virtual const char *get_name() const {
		return k_clazz;
	}

	virtual void run(const loop_registers<2, 1> &r);

	static kernel_base<2, 1> *match(const kern_mul_ij_pi_pj &z,
		list_t &in, list_t &out);

};


} // namespace libtensor

#endif // LIBTENSOR_KERN_MUL_IJK_PJI_PK_H