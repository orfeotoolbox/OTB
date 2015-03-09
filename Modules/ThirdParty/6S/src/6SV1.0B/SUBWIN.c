/* SUBWIN.f -- translated by f2c (version 19970805).
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

/*<       subroutine   subwin >*/
/* Subroutine */ int subwin_()
{
    /* Initialized data */

    static doublereal z5[34] = { 0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,
	    13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,30.,35.,40.,
	    45.,50.,70.,100.,99999. };
    static doublereal p5[34] = { 1013.,887.8,777.5,679.8,593.2,515.8,446.7,
	    385.3,330.8,282.9,241.8,206.7,176.6,151.,129.1,110.3,94.31,80.58,
	    68.82,58.75,50.14,42.77,36.47,31.09,26.49,22.56,10.2,4.701,2.243,
	    1.113,.5719,.04016,3e-4,0. };
    static doublereal t5[34] = { 257.1,259.1,255.9,252.7,247.7,240.9,234.1,
	    227.3,220.6,217.2,217.2,217.2,217.2,217.2,217.2,217.2,216.6,216.,
	    215.4,214.8,214.1,213.6,213.,212.4,211.8,211.2,216.,222.2,234.7,
	    247.,259.3,245.7,210.,210. };
    static doublereal wh5[34] = { 1.2,1.2,.94,.68,.41,.2,.098,.054,.011,.0084,
	    .0055,.0038,.0026,.0018,.001,7.6e-4,6.4e-4,5.6e-4,5e-4,4.9e-4,
	    4.5e-4,5.1e-4,5.1e-4,5.4e-4,6e-4,6.7e-4,3.6e-4,1.1e-4,4.3e-5,
	    1.9e-5,6.3e-6,1.4e-7,1e-9,0. };
    static doublereal wo5[34] = { 4.1e-5,4.1e-5,4.1e-5,4.3e-5,4.5e-5,4.7e-5,
	    4.9e-5,7.1e-5,9e-5,1.6e-4,2.4e-4,3.2e-4,4.3e-4,4.7e-4,4.9e-4,
	    5.6e-4,6.2e-4,6.2e-4,6.2e-4,6e-4,5.6e-4,5.1e-4,4.7e-4,4.3e-4,
	    3.6e-4,3.2e-4,1.5e-4,9.2e-5,4.1e-5,1.3e-5,4.3e-6,8.6e-8,4.3e-11,
	    0. };

    integer i__;

/*<       real z5(34),p5(34),t5(34),wh5(34),wo5(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       integer i >*/

/*     model: subarctique winter mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z5(i) >*/
	sixs_atm__1.z__[i__ - 1] = z5[i__ - 1];
/*<       p(i)=p5(i) >*/
	sixs_atm__1.p[i__ - 1] = p5[i__ - 1];
/*<       t(i)=t5(i) >*/
	sixs_atm__1.t[i__ - 1] = t5[i__ - 1];
/*<       wh(i)=wh5(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh5[i__ - 1];
/*<       wo(i)=wo5(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo5[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* subwin_ */

#ifdef __cplusplus
	}
#endif
