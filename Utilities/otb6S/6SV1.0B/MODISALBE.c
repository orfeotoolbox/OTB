/* MODISALBE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<    >*/
/* Subroutine */ int modisalbe_(doublereal *p1, doublereal *p2, doublereal *
	p3, doublereal *brdfalb)
{
/*<       real p1,p2,p3,brdfalb >*/
/*<       brdfalb=p1+p2*0.189184-p3*1.377622 >*/
    *brdfalb = *p1 + *p2 * .189184 - *p3 * 1.377622;
/*<       return >*/
    return 0;
/*<       end >*/
} /* modisalbe_ */

#ifdef __cplusplus
	}
#endif
