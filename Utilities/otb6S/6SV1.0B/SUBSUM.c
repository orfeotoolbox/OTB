/* SUBSUM.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/* Common Block Declarations */

Extern struct {
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*<       subroutine   subsum >*/
/* Subroutine */ int subsum_()
{
    /* Initialized data */

    static real z4[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p4[34] = { (float)1010.,(float)896.,(float)792.9,(float)700.,(
	    float)616.,(float)541.,(float)473.,(float)413.,(float)359.,(float)
	    310.7,(float)267.7,(float)230.,(float)197.7,(float)170.,(float)
	    146.,(float)125.,(float)108.,(float)92.8,(float)79.8,(float)68.6,(
	    float)58.9,(float)50.7,(float)43.6,(float)37.5,(float)32.27,(
	    float)27.8,(float)13.4,(float)6.61,(float)3.4,(float)1.81,(float)
	    .987,(float).0707,(float)3e-4,(float)0. };
    static real t4[34] = { (float)287.,(float)282.,(float)276.,(float)271.,(
	    float)266.,(float)260.,(float)253.,(float)246.,(float)239.,(float)
	    232.,(float)225.,(float)225.,(float)225.,(float)225.,(float)225.,(
	    float)225.,(float)225.,(float)225.,(float)225.,(float)225.,(float)
	    225.,(float)225.,(float)225.,(float)225.,(float)226.,(float)228.,(
	    float)235.,(float)247.,(float)262.,(float)274.,(float)277.,(float)
	    216.,(float)210.,(float)210. };
    static real wh4[34] = { (float)9.1,(float)6.,(float)4.2,(float)2.7,(float)
	    1.7,(float)1.,(float).54,(float).29,(float).13,(float).042,(float)
	    .015,(float).0094,(float).006,(float).0018,(float).001,(float)
	    7.6e-4,(float)6.4e-4,(float)5.6e-4,(float)5e-4,(float)4.9e-4,(
	    float)4.5e-4,(float)5.1e-4,(float)5.1e-4,(float)5.4e-4,(float)
	    6e-4,(float)6.7e-4,(float)3.6e-4,(float)1.1e-4,(float)4.3e-5,(
	    float)1.9e-5,(float)6.3e-6,(float)1.4e-7,(float)1e-9,(float)0. };
    static real wo4[34] = { (float)4.9e-5,(float)5.4e-5,(float)5.6e-5,(float)
	    5.8e-5,(float)6e-5,(float)6.4e-5,(float)7.1e-5,(float)7.5e-5,(
	    float)7.9e-5,(float)1.1e-4,(float)1.3e-4,(float)1.8e-4,(float)
	    2.1e-4,(float)2.6e-4,(float)2.8e-4,(float)3.2e-4,(float)3.4e-4,(
	    float)3.9e-4,(float)4.1e-4,(float)4.1e-4,(float)3.9e-4,(float)
	    3.6e-4,(float)3.2e-4,(float)3e-4,(float)2.8e-4,(float)2.6e-4,(
	    float)1.4e-4,(float)9.2e-5,(float)4.1e-5,(float)1.3e-5,(float)
	    4.3e-6,(float)8.6e-8,(float)4.3e-11,(float)0. };

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
