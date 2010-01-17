
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
integer i_abs(x) integer *x;
#else
integer i_abs(integer *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
#ifdef __cplusplus
}
#endif
