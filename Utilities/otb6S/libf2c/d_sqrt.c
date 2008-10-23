
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef KR_headers
double sqrt();
double d_sqrt(x) doublereal *x;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
double d_sqrt(doublereal *x)
#endif
{
return( sqrt(*x) );
}
#ifdef __cplusplus
}
#endif
