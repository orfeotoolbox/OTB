/* TROPIC.f -- translated by f2c (version 19970805).
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

/*<       subroutine   tropic >*/
/* Subroutine */ int tropic_()
{
    /* Initialized data */

    static doublereal z1[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p1[34] = { 1013.,904.,805.,715.,633.,559.,492.,432.,
	    378.,329.,286.,247.,213.,182.,156.,132.,111.,93.7,78.9,66.6,56.5,
	    48.,40.9,35.,30.,25.7,12.2,6.,3.05,1.59,.854,.0579,3e-4,0. };
    static doublereal t1[34] = { 300.,294.,288.,284.,277.,270.,264.,257.,250.,
	    244.,237.,230.,224.,217.,210.,204.,197.,195.,199.,203.,207.,211.,
	    215.,217.,219.,221.,232.,243.,254.,265.,270.,219.,210.,210. };
    static doublereal wh1[34] = { 19.,13.,9.3,4.7,2.2,1.5,.85,.47,.25,.12,.05,
	    .017,.006,.0018,.001,7.6e-4,6.4e-4,5.6e-4,5e-4,4.9e-4,4.5e-4,
	    5.1e-4,5.1e-4,5.4e-4,6e-4,6.7e-4,3.6e-4,1.1e-4,4.3e-5,1.9e-5,
	    6.3e-6,1.4e-7,1e-9,0. };
    static doublereal wo1[34] = { 5.6e-5,5.6e-5,5.4e-5,5.1e-5,4.7e-5,4.5e-5,
	    4.3e-5,4.1e-5,3.9e-5,3.9e-5,3.9e-5,4.1e-5,4.3e-5,4.5e-5,4.5e-5,
	    4.7e-5,4.7e-5,6.9e-5,9e-5,1.4e-4,1.9e-4,2.4e-4,2.8e-4,3.2e-4,
	    3.4e-4,3.4e-4,2.4e-4,9.2e-5,4.1e-5,1.3e-5,4.3e-6,8.6e-8,4.3e-11,
	    0. };

    integer i__;

/*<       integer i >*/
/*<       real z1(34),p1(34),t1(34),wh1(34),wo1(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/

/*     model: tropical mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z1(i) >*/
	sixs_atm__1.z__[i__ - 1] = z1[i__ - 1];
/*<       p(i)=p1(i) >*/
	sixs_atm__1.p[i__ - 1] = p1[i__ - 1];
/*<       t(i)=t1(i) >*/
	sixs_atm__1.t[i__ - 1] = t1[i__ - 1];
/*<       wh(i)=wh1(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh1[i__ - 1];
/*<       wo(i)=wo1(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo1[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* tropic_ */

#ifdef __cplusplus
	}
#endif
