
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
double d_prod(x,y) real *x, *y;
#else
double d_prod(real *x, real *y)
#endif
{
return( (*x) * (*y) );
}
#ifdef __cplusplus
}
#endif
