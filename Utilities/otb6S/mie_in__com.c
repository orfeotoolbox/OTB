#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    real rmax, rmin;
    integer icp;
    real rn[80]	/* was [20][4] */, ri[80]	/* was [20][4] */, x1[4], x2[
	    4], x3[4], cij[4];
    integer irsunph;
    real rsunph[50], nrsunph[50];
} mie_in__;

#ifdef __cplusplus
}
#endif
