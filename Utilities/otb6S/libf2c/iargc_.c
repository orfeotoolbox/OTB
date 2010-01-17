
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
ftnint iargc_()
#else
ftnint iargc_(void)
#endif
{
extern int xargc;
return ( xargc - 1 );
}
#ifdef __cplusplus
}
#endif
