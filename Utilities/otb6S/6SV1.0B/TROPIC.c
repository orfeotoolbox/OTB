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
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*<       subroutine   tropic >*/
/* Subroutine */ int tropic_()
{
    /* Initialized data */

    static real z1[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p1[34] = { (float)1013.,(float)904.,(float)805.,(float)715.,(
	    float)633.,(float)559.,(float)492.,(float)432.,(float)378.,(float)
	    329.,(float)286.,(float)247.,(float)213.,(float)182.,(float)156.,(
	    float)132.,(float)111.,(float)93.7,(float)78.9,(float)66.6,(float)
	    56.5,(float)48.,(float)40.9,(float)35.,(float)30.,(float)25.7,(
	    float)12.2,(float)6.,(float)3.05,(float)1.59,(float).854,(float)
	    .0579,(float)3e-4,(float)0. };
    static real t1[34] = { (float)300.,(float)294.,(float)288.,(float)284.,(
	    float)277.,(float)270.,(float)264.,(float)257.,(float)250.,(float)
	    244.,(float)237.,(float)230.,(float)224.,(float)217.,(float)210.,(
	    float)204.,(float)197.,(float)195.,(float)199.,(float)203.,(float)
	    207.,(float)211.,(float)215.,(float)217.,(float)219.,(float)221.,(
	    float)232.,(float)243.,(float)254.,(float)265.,(float)270.,(float)
	    219.,(float)210.,(float)210. };
    static real wh1[34] = { (float)19.,(float)13.,(float)9.3,(float)4.7,(
	    float)2.2,(float)1.5,(float).85,(float).47,(float).25,(float).12,(
	    float).05,(float).017,(float).006,(float).0018,(float).001,(float)
	    7.6e-4,(float)6.4e-4,(float)5.6e-4,(float)5e-4,(float)4.9e-4,(
	    float)4.5e-4,(float)5.1e-4,(float)5.1e-4,(float)5.4e-4,(float)
	    6e-4,(float)6.7e-4,(float)3.6e-4,(float)1.1e-4,(float)4.3e-5,(
	    float)1.9e-5,(float)6.3e-6,(float)1.4e-7,(float)1e-9,(float)0. };
    static real wo1[34] = { (float)5.6e-5,(float)5.6e-5,(float)5.4e-5,(float)
	    5.1e-5,(float)4.7e-5,(float)4.5e-5,(float)4.3e-5,(float)4.1e-5,(
	    float)3.9e-5,(float)3.9e-5,(float)3.9e-5,(float)4.1e-5,(float)
	    4.3e-5,(float)4.5e-5,(float)4.5e-5,(float)4.7e-5,(float)4.7e-5,(
	    float)6.9e-5,(float)9e-5,(float)1.4e-4,(float)1.9e-4,(float)
	    2.4e-4,(float)2.8e-4,(float)3.2e-4,(float)3.4e-4,(float)3.4e-4,(
	    float)2.4e-4,(float)9.2e-5,(float)4.1e-5,(float)1.3e-5,(float)
	    4.3e-6,(float)8.6e-8,(float)4.3e-11,(float)0. };

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
