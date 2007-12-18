/* MIDSUM.f -- translated by f2c (version 19970805).
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
    doublereal z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*<       subroutine   midsum >*/
/* Subroutine */ int midsum_()
{
    /* Initialized data */

    static doublereal z2[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p2[34] = { 1013.,902.,802.,710.,628.,554.,487.,426.,
	    372.,324.,281.,243.,209.,179.,153.,130.,111.,95.,81.2,69.5,59.5,
	    51.,43.7,37.6,32.2,27.7,13.2,6.52,3.33,1.76,.951,.0671,3e-4,0. };
    static doublereal t2[34] = { 294.,290.,285.,279.,273.,267.,261.,255.,248.,
	    242.,235.,229.,222.,216.,216.,216.,216.,216.,216.,217.,218.,219.,
	    220.,222.,223.,224.,234.,245.,258.,270.,276.,218.,210.,210. };
    static doublereal wh2[34] = { 14.,9.3,5.9,3.3,1.9,1.,.61,.37,.21,.12,.064,
	    .022,.006,.0018,.001,7.6e-4,6.4e-4,5.6e-4,5e-4,4.9e-4,4.5e-4,
	    5.1e-4,5.1e-4,5.4e-4,6e-4,6.7e-4,3.6e-4,1.1e-4,4.3e-5,1.9e-5,
	    1.3e-6,1.4e-7,1e-9,0. };
    static doublereal wo2[34] = { 6e-5,6e-5,6e-5,6.2e-5,6.4e-5,6.6e-5,6.9e-5,
	    7.5e-5,7.9e-5,8.6e-5,9e-5,1.1e-4,1.2e-4,1.5e-4,1.8e-4,1.9e-4,
	    2.1e-4,2.4e-4,2.8e-4,3.2e-4,3.4e-4,3.6e-4,3.6e-4,3.4e-4,3.2e-4,
	    3e-4,2e-4,9.2e-5,4.1e-5,1.3e-5,4.3e-6,8.6e-8,4.3e-11,0. };

    integer i__;

/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       real z2(34),p2(34),t2(34),wh2(34),wo2(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       integer i >*/

/*     model: midlatitude summer mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z2(i) >*/
	sixs_atm__1.z__[i__ - 1] = z2[i__ - 1];
/*<       p(i)=p2(i) >*/
	sixs_atm__1.p[i__ - 1] = p2[i__ - 1];
/*<       t(i)=t2(i) >*/
	sixs_atm__1.t[i__ - 1] = t2[i__ - 1];
/*<       wh(i)=wh2(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh2[i__ - 1];
/*<       wo(i)=wo2(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo2[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* midsum_ */

#ifdef __cplusplus
	}
#endif
