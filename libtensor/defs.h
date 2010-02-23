#ifndef LIBTENSOR_DEFS_H
#define LIBTENSOR_DEFS_H

#include <cstddef>

/**	\brief Tensor library
	\ingroup libtensor
 **/
namespace libtensor {

/**	\brief Namespace name
 **/
extern const char *g_ns;

}

#ifdef DEBUG_CHECK
#define LIBTENSOR_DEBUG
#endif

#ifdef USE_MKL
#include <mkl.h>
#undef USE_CBLAS
#endif
#ifdef USE_CBLAS
#include <cblas.h>
#endif

#ifdef __MINGW32__
#include <cstdlib>
inline void srand48(long seed) { srand(seed); }
inline double drand48() { return (double(rand())/RAND_MAX); }
inline long lrand48() { return rand(); }
#endif

/**	\defgroup libtensor Tensor library
 **/

/**	\defgroup libtensor_core Core components
	\ingroup libtensor
 **/

/**	\defgroup libtensor_tests Tests
	\brief Unit tests of individual classes
	\ingroup libtensor
 **/

/**	\defgroup libtensor_tod Tensor operations (double)
	\brief Operations on tensors with real double precision elements
	\ingroup libtensor
 **/

#endif // LIBTENSOR_DEFS_H

