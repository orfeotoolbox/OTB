/* MINNALBE.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int minnalbe_(doublereal *par1, doublereal *par2, doublereal 
	*brdfalb)
{
/*<       real par1,par2,brdfalb >*/
/*<       brdfalb=2.*par2/(par1+1.) >*/
    *brdfalb = *par2 * 2. / (*par1 + 1.);
/*<       return >*/
    return 0;
/*<       end >*/
} /* minnalbe_ */

#ifdef __cplusplus
	}
#endif
