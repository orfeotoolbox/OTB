
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef KR_headers
double tanh();
double d_tanh(x) doublereal *x;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
double d_tanh(doublereal *x)
#endif
{
return( tanh(*x) );
}
#ifdef __cplusplus
}
#endif
