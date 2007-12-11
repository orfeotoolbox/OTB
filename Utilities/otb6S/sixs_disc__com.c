#include "f2c.h"

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
