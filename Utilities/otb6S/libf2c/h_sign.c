
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
shortint h_sign(a,b) shortint *a, *b;
#else
shortint h_sign(shortint *a, shortint *b)
#endif
{
shortint x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
#ifdef __cplusplus
}
#endif
