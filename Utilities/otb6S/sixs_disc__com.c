/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"


#ifdef __cplusplus
extern "C" {
#endif

struct {
    real roatm[60]	/* was [3][20] */, dtdir[60]	/* was [3][20] */, 
	    dtdif[60]	/* was [3][20] */, utdir[60]	/* was [3][20] */, 
	    utdif[60]	/* was [3][20] */, sphal[60]	/* was [3][20] */, 
	    wldis[20], trayl[20], traypl[20], rqatm[60]	/* was [3][20] */, 
	    ruatm[60]	/* was [3][20] */;
} sixs_disc__;

#ifdef __cplusplus
}
#endif
