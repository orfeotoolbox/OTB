/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal ex[80]	/* was [4][20] */, sc[80]	/* was [4][20] */, 
	    asy[80]	/* was [4][20] */, vi[4];
} sixs_coef__;

#ifdef __cplusplus
}
#endif
