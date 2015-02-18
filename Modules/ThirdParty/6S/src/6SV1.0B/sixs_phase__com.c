/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal phasel[20000]	/* was [20][1000] */, qhasel[20000]	/* 
	    was [20][1000] */, uhasel[20000]	/* was [20][1000] */;
} sixs_phase__;

#ifdef __cplusplus
}
#endif
