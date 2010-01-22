/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal gr, gt, g, g1, th, sth, cth, th1, sth1, cth1, phi, sp, cp, 
	    th22, st, ct, st1, ct1, t10, t11, e1, e2, s2, s3, ctg, ctg1, ctt1,
	     stt1, calph, alp2, salp2, calp2, alph, salph, alpp, difmy, 
	    difsig;
} ggg_;

#ifdef __cplusplus
}
#endif
