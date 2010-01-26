
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
VOID r_cnjg(r, z) complex *r, *z;
#else
VOID r_cnjg(complex *r, complex *z)
#endif
{
	real zi = z->i;
	r->r = z->r;
	r->i = -zi;
	}
#ifdef __cplusplus
}
#endif
