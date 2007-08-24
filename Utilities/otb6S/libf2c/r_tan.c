
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef KR_headers
double tan();
double r_tan(x) real *x;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
double r_tan(real *x)
#endif
{
return( tan(*x) );
}
#ifdef __cplusplus
}
#endif
