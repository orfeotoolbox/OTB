#include "stdio.h"

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef KR_headers
extern VOID sig_die();

int abort_()
#else
/* OTB Modification */
/* extern void sig_die(char*,int); */
extern void sig_die(register char*,int);

int abort_(void)
#endif
{
sig_die("Fortran abort routine called", 1);
return 0;	/* not reached */
}

#ifdef __cplusplus
}
#endif


