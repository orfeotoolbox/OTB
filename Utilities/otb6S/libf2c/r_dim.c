
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
double r_dim(a,b) real *a, *b;
#else
double r_dim(real *a, real *b)
#endif
{
return( *a > *b ? *a - *b : 0);
}
#ifdef __cplusplus
}
#endif
