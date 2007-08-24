/* SUBWIN.f -- translated by f2c (version 19970805).
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

/*<       subroutine   subwin >*/
/* Subroutine */ int subwin_()
{
    /* Initialized data */

    static real z5[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p5[34] = { (float)1013.,(float)887.8,(float)777.5,(float)
	    679.8,(float)593.2,(float)515.8,(float)446.7,(float)385.3,(float)
	    330.8,(float)282.9,(float)241.8,(float)206.7,(float)176.6,(float)
	    151.,(float)129.1,(float)110.3,(float)94.31,(float)80.58,(float)
	    68.82,(float)58.75,(float)50.14,(float)42.77,(float)36.47,(float)
	    31.09,(float)26.49,(float)22.56,(float)10.2,(float)4.701,(float)
	    2.243,(float)1.113,(float).5719,(float).04016,(float)3e-4,(float)
	    0. };
    static real t5[34] = { (float)257.1,(float)259.1,(float)255.9,(float)
	    252.7,(float)247.7,(float)240.9,(float)234.1,(float)227.3,(float)
	    220.6,(float)217.2,(float)217.2,(float)217.2,(float)217.2,(float)
	    217.2,(float)217.2,(float)217.2,(float)216.6,(float)216.,(float)
	    215.4,(float)214.8,(float)214.1,(float)213.6,(float)213.,(float)
	    212.4,(float)211.8,(float)211.2,(float)216.,(float)222.2,(float)
	    234.7,(float)247.,(float)259.3,(float)245.7,(float)210.,(float)
	    210. };
    static real wh5[34] = { (float)1.2,(float)1.2,(float).94,(float).68,(
	    float).41,(float).2,(float).098,(float).054,(float).011,(float)
	    .0084,(float).0055,(float).0038,(float).0026,(float).0018,(float)
	    .001,(float)7.6e-4,(float)6.4e-4,(float)5.6e-4,(float)5e-4,(float)
	    4.9e-4,(float)4.5e-4,(float)5.1e-4,(float)5.1e-4,(float)5.4e-4,(
	    float)6e-4,(float)6.7e-4,(float)3.6e-4,(float)1.1e-4,(float)
	    4.3e-5,(float)1.9e-5,(float)6.3e-6,(float)1.4e-7,(float)1e-9,(
	    float)0. };
    static real wo5[34] = { (float)4.1e-5,(float)4.1e-5,(float)4.1e-5,(float)
	    4.3e-5,(float)4.5e-5,(float)4.7e-5,(float)4.9e-5,(float)7.1e-5,(
	    float)9e-5,(float)1.6e-4,(float)2.4e-4,(float)3.2e-4,(float)
	    4.3e-4,(float)4.7e-4,(float)4.9e-4,(float)5.6e-4,(float)6.2e-4,(
	    float)6.2e-4,(float)6.2e-4,(float)6e-4,(float)5.6e-4,(float)
	    5.1e-4,(float)4.7e-4,(float)4.3e-4,(float)3.6e-4,(float)3.2e-4,(
	    float)1.5e-4,(float)9.2e-5,(float)4.1e-5,(float)1.3e-5,(float)
	    4.3e-6,(float)8.6e-8,(float)4.3e-11,(float)0. };

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
