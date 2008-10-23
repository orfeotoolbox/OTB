
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
extern integer s_cmp();
logical l_ge(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
logical l_ge(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) >= 0);
}
#ifdef __cplusplus
}
#endif
