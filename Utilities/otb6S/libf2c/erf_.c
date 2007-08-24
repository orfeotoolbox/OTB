
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef REAL
#define REAL double
#endif

#ifdef KR_headers
double erf();
REAL erf_(x) real *x;
#else
extern double erf(double);
REAL erf_(real *x)
#endif
{
return( erf((double)*x) );
}
#ifdef __cplusplus
}
#endif
