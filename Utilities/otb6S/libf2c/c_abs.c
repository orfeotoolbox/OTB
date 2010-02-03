
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
/* OTB Modifications */
#undef abs
#undef min
#undef max


#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
extern double f__cabs();

double c_abs(z) complex *z;
#else
extern double f__cabs(double, double);

double c_abs(complex *z)
#endif
{
return( f__cabs( z->r, z->i ) );
}
#ifdef __cplusplus
}
#endif
