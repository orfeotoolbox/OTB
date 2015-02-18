/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal ph[20000]	/* was [20][1000] */, qh[20000]	/* was [20][
	    1000] */, uh[20000]	/* was [20][1000] */;
} sixs_aerbas__;

#ifdef __cplusplus
}
#endif
