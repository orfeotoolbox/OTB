/* MIDWIN.f -- translated by f2c (version 19970805).
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

/*<       subroutine   midwin >*/
/* Subroutine */ int midwin_()
{
    /* Initialized data */

    static doublereal z3[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p3[34] = { 1018.,897.3,789.7,693.8,608.1,531.3,462.7,
	    401.6,347.3,299.2,256.8,219.9,188.2,161.,137.8,117.8,100.7,86.1,
	    73.5,62.8,53.7,45.8,39.1,33.4,28.6,24.3,11.1,5.18,2.53,1.29,.682,
	    .0467,3e-4,0. };
    static doublereal t3[34] = { 272.2,268.7,265.2,261.7,255.7,249.7,243.7,
	    237.7,231.7,225.7,219.7,219.2,218.7,218.2,217.7,217.2,216.7,216.2,
	    215.7,215.2,215.2,215.2,215.2,215.2,215.2,215.2,217.4,227.8,243.2,
	    258.5,265.7,230.7,210.2,210. };
    static doublereal wh3[34] = { 3.5,2.5,1.8,1.2,.66,.38,.21,.085,.035,.016,
	    .0075,.0069,.006,.0018,.001,7.6e-4,6.4e-4,5.6e-4,5e-4,4.9e-4,
	    4.5e-4,5.1e-4,5.1e-4,5.4e-4,6e-4,6.7e-4,3.6e-4,1.1e-4,4.3e-5,
	    1.9e-5,6.3e-6,1.4e-7,1e-9,0. };
    static doublereal wo3[34] = { 6e-5,5.4e-5,4.9e-5,4.9e-5,4.9e-5,5.8e-5,
	    6.4e-5,7.7e-5,9e-5,1.2e-4,1.6e-4,2.1e-4,2.6e-4,3e-4,3.2e-4,3.4e-4,
	    3.6e-4,3.9e-4,4.1e-4,4.3e-4,4.5e-4,4.3e-4,4.3e-4,3.9e-4,3.6e-4,
	    3.4e-4,1.9e-4,9.2e-5,4.1e-5,1.3e-5,4.3e-6,8.6e-8,4.3e-11,0. };

    integer i__;

/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       real z3(34),p3(34),t3(34),wh3(34),wo3(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       integer i >*/

/*     model: midlatitude winter mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z3(i) >*/
	sixs_atm__1.z__[i__ - 1] = z3[i__ - 1];
/*<       p(i)=p3(i) >*/
	sixs_atm__1.p[i__ - 1] = p3[i__ - 1];
/*<       t(i)=t3(i) >*/
	sixs_atm__1.t[i__ - 1] = t3[i__ - 1];
/*<       wh(i)=wh3(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh3[i__ - 1];
/*<       wo(i)=wo3(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo3[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* midwin_ */

#ifdef __cplusplus
	}
#endif
