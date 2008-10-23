
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
double r_abs(x) real *x;
#else
double r_abs(real *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
#ifdef __cplusplus
}
#endif
