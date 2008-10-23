
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
shortint h_mod(a,b) short *a, *b;
#else
shortint h_mod(short *a, short *b)
#endif
{
return( *a % *b);
}
#ifdef __cplusplus
}
#endif
