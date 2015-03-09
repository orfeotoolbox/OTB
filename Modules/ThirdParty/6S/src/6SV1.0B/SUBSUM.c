/* SUBSUM.f -- translated by f2c (version 19970805).
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

/*<       subroutine   subsum >*/
/* Subroutine */ int subsum_()
{
    /* Initialized data */

    static doublereal z4[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p4[34] = { 1010.,896.,792.9,700.,616.,541.,473.,413.,
	    359.,310.7,267.7,230.,197.7,170.,146.,125.,108.,92.8,79.8,68.6,
	    58.9,50.7,43.6,37.5,32.27,27.8,13.4,6.61,3.4,1.81,.987,.0707,3e-4,
	    0. };
    static doublereal t4[34] = { 287.,282.,276.,271.,266.,260.,253.,246.,239.,
	    232.,225.,225.,225.,225.,225.,225.,225.,225.,225.,225.,225.,225.,
	    225.,225.,226.,228.,235.,247.,262.,274.,277.,216.,210.,210. };
    static doublereal wh4[34] = { 9.1,6.,4.2,2.7,1.7,1.,.54,.29,.13,.042,.015,
	    .0094,.006,.0018,.001,7.6e-4,6.4e-4,5.6e-4,5e-4,4.9e-4,4.5e-4,
	    5.1e-4,5.1e-4,5.4e-4,6e-4,6.7e-4,3.6e-4,1.1e-4,4.3e-5,1.9e-5,
	    6.3e-6,1.4e-7,1e-9,0. };
    static doublereal wo4[34] = { 4.9e-5,5.4e-5,5.6e-5,5.8e-5,6e-5,6.4e-5,
	    7.1e-5,7.5e-5,7.9e-5,1.1e-4,1.3e-4,1.8e-4,2.1e-4,2.6e-4,2.8e-4,
	    3.2e-4,3.4e-4,3.9e-4,4.1e-4,4.1e-4,3.9e-4,3.6e-4,3.2e-4,3e-4,
	    2.8e-4,2.6e-4,1.4e-4,9.2e-5,4.1e-5,1.3e-5,4.3e-6,8.6e-8,4.3e-11,
	    0. };

    integer i__;

/*<       integer i >*/
/*<       real z4(34),p4(34),t4(34),wh4(34),wo4(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/

/*     model: subarctique summer mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z4(i) >*/
	sixs_atm__1.z__[i__ - 1] = z4[i__ - 1];
/*<       p(i)=p4(i) >*/
	sixs_atm__1.p[i__ - 1] = p4[i__ - 1];
/*<       t(i)=t4(i) >*/
	sixs_atm__1.t[i__ - 1] = t4[i__ - 1];
/*<       wh(i)=wh4(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh4[i__ - 1];
/*<       wo(i)=wo4(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo4[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* subsum_ */

#ifdef __cplusplus
	}
#endif
