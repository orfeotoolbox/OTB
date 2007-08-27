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
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*<       subroutine   midwin >*/
/* Subroutine */ int midwin_()
{
    /* Initialized data */

    static real z3[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p3[34] = { (float)1018.,(float)897.3,(float)789.7,(float)
	    693.8,(float)608.1,(float)531.3,(float)462.7,(float)401.6,(float)
	    347.3,(float)299.2,(float)256.8,(float)219.9,(float)188.2,(float)
	    161.,(float)137.8,(float)117.8,(float)100.7,(float)86.1,(float)
	    73.5,(float)62.8,(float)53.7,(float)45.8,(float)39.1,(float)33.4,(
	    float)28.6,(float)24.3,(float)11.1,(float)5.18,(float)2.53,(float)
	    1.29,(float).682,(float).0467,(float)3e-4,(float)0. };
    static real t3[34] = { (float)272.2,(float)268.7,(float)265.2,(float)
	    261.7,(float)255.7,(float)249.7,(float)243.7,(float)237.7,(float)
	    231.7,(float)225.7,(float)219.7,(float)219.2,(float)218.7,(float)
	    218.2,(float)217.7,(float)217.2,(float)216.7,(float)216.2,(float)
	    215.7,(float)215.2,(float)215.2,(float)215.2,(float)215.2,(float)
	    215.2,(float)215.2,(float)215.2,(float)217.4,(float)227.8,(float)
	    243.2,(float)258.5,(float)265.7,(float)230.7,(float)210.2,(float)
	    210. };
    static real wh3[34] = { (float)3.5,(float)2.5,(float)1.8,(float)1.2,(
	    float).66,(float).38,(float).21,(float).085,(float).035,(float)
	    .016,(float).0075,(float).0069,(float).006,(float).0018,(float)
	    .001,(float)7.6e-4,(float)6.4e-4,(float)5.6e-4,(float)5e-4,(float)
	    4.9e-4,(float)4.5e-4,(float)5.1e-4,(float)5.1e-4,(float)5.4e-4,(
	    float)6e-4,(float)6.7e-4,(float)3.6e-4,(float)1.1e-4,(float)
	    4.3e-5,(float)1.9e-5,(float)6.3e-6,(float)1.4e-7,(float)1e-9,(
	    float)0. };
    static real wo3[34] = { (float)6e-5,(float)5.4e-5,(float)4.9e-5,(float)
	    4.9e-5,(float)4.9e-5,(float)5.8e-5,(float)6.4e-5,(float)7.7e-5,(
	    float)9e-5,(float)1.2e-4,(float)1.6e-4,(float)2.1e-4,(float)
	    2.6e-4,(float)3e-4,(float)3.2e-4,(float)3.4e-4,(float)3.6e-4,(
	    float)3.9e-4,(float)4.1e-4,(float)4.3e-4,(float)4.5e-4,(float)
	    4.3e-4,(float)4.3e-4,(float)3.9e-4,(float)3.6e-4,(float)3.4e-4,(
	    float)1.9e-4,(float)9.2e-5,(float)4.1e-5,(float)1.3e-5,(float)
	    4.3e-6,(float)8.6e-8,(float)4.3e-11,(float)0. };

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
