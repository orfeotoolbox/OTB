/* POSSPO.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Common Block Declarations */

Extern struct {
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/*<    >*/
/* Subroutine */ int posspo_(integer *month, integer *jday, doublereal *tu, 
	doublereal *xlon, doublereal *xlat, doublereal *asol, doublereal *
	phi0, doublereal *avis, doublereal *phiv)
{
    extern /* Subroutine */ int possol_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);

/*<       logical ier >*/
/*<       real tu,xlon,xlat,asol,phi0,avis,phiv >*/
/*<       integer month,jday,iwr >*/
/*<       common/sixs_ier/iwr,ier >*/
/*     spot definition */
/*     warning !!! */
/*     xlon and xlat are the coordinates of the scene center. */
/*<       avis=0. >*/
    *avis = 0.;
/*<       phiv=0. >*/
    *phiv = 0.;
/*<    >*/
    possol_(month, jday, tu, xlon, xlat, asol, phi0);
/*<       if(ier)return >*/
    if (sixs_ier__1.ier) {
	return 0;
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* posspo_ */

#ifdef __cplusplus
	}
#endif
