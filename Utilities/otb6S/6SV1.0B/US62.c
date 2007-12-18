/* US62.f -- translated by f2c (version 19970805).
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

/*<       subroutine   us62 >*/
/* Subroutine */ int us62_()
{
    /* Initialized data */

    static doublereal z6[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p6[34] = { 1013.,898.6,795.,701.2,616.6,540.5,472.2,
	    411.1,356.5,308.,265.,227.,194.,165.8,141.7,121.1,103.5,88.5,
	    75.65,64.67,55.29,47.29,40.47,34.67,29.72,25.49,11.97,5.746,2.871,
	    1.491,.7978,.0552,3.008e-4,0. };
    static doublereal t6[34] = { 288.1,281.6,275.1,268.7,262.2,255.7,249.2,
	    242.7,236.2,229.7,223.2,216.8,216.6,216.6,216.6,216.6,216.6,216.6,
	    216.6,216.6,216.6,217.6,218.6,219.6,220.6,221.6,226.5,236.5,253.4,
	    264.2,270.6,219.7,210.,210. };
    static doublereal wh6[34] = { 5.9,4.2,2.9,1.8,1.1,.64,.38,.21,.12,.046,
	    .018,.0082,.0037,.0018,8.4e-4,7.2e-4,6.1e-4,5.2e-4,4.4e-4,4.4e-4,
	    4.4e-4,4.8e-4,5.2e-4,5.7e-4,6.1e-4,6.6e-4,3.8e-4,1.6e-4,6.7e-5,
	    3.2e-5,1.2e-5,1.5e-7,1e-9,0. };
    static doublereal wo6[34] = { 5.4e-5,5.4e-5,5.4e-5,5e-5,4.6e-5,4.6e-5,
	    4.5e-5,4.9e-5,5.2e-5,7.1e-5,9e-5,1.3e-4,1.6e-4,1.7e-4,1.9e-4,
	    2.1e-4,2.4e-4,2.8e-4,3.2e-4,3.5e-4,3.8e-4,3.8e-4,3.9e-4,3.8e-4,
	    3.6e-4,3.4e-4,2e-4,1.1e-4,4.9e-5,1.7e-5,4e-6,8.6e-8,4.3e-11,0. };

    integer i__;

/*<       integer i >*/
/*<       real z6(34),p6(34),t6(34),wh6(34),wo6(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/

/*     model: us standard 62 mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z6(i) >*/
	sixs_atm__1.z__[i__ - 1] = z6[i__ - 1];
/*<       p(i)=p6(i) >*/
	sixs_atm__1.p[i__ - 1] = p6[i__ - 1];
/*<       t(i)=t6(i) >*/
	sixs_atm__1.t[i__ - 1] = t6[i__ - 1];
/*<       wh(i)=wh6(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh6[i__ - 1];
/*<       wo(i)=wo6(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo6[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* us62_ */

#ifdef __cplusplus
	}
#endif
