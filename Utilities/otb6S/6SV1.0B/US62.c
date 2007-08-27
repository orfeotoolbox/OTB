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
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*<       subroutine   us62 >*/
/* Subroutine */ int us62_()
{
    /* Initialized data */

    static real z6[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p6[34] = { (float)1013.,(float)898.6,(float)795.,(float)701.2,
	    (float)616.6,(float)540.5,(float)472.2,(float)411.1,(float)356.5,(
	    float)308.,(float)265.,(float)227.,(float)194.,(float)165.8,(
	    float)141.7,(float)121.1,(float)103.5,(float)88.5,(float)75.65,(
	    float)64.67,(float)55.29,(float)47.29,(float)40.47,(float)34.67,(
	    float)29.72,(float)25.49,(float)11.97,(float)5.746,(float)2.871,(
	    float)1.491,(float).7978,(float).0552,(float)3.008e-4,(float)0. };
    static real t6[34] = { (float)288.1,(float)281.6,(float)275.1,(float)
	    268.7,(float)262.2,(float)255.7,(float)249.2,(float)242.7,(float)
	    236.2,(float)229.7,(float)223.2,(float)216.8,(float)216.6,(float)
	    216.6,(float)216.6,(float)216.6,(float)216.6,(float)216.6,(float)
	    216.6,(float)216.6,(float)216.6,(float)217.6,(float)218.6,(float)
	    219.6,(float)220.6,(float)221.6,(float)226.5,(float)236.5,(float)
	    253.4,(float)264.2,(float)270.6,(float)219.7,(float)210.,(float)
	    210. };
    static real wh6[34] = { (float)5.9,(float)4.2,(float)2.9,(float)1.8,(
	    float)1.1,(float).64,(float).38,(float).21,(float).12,(float).046,
	    (float).018,(float).0082,(float).0037,(float).0018,(float)8.4e-4,(
	    float)7.2e-4,(float)6.1e-4,(float)5.2e-4,(float)4.4e-4,(float)
	    4.4e-4,(float)4.4e-4,(float)4.8e-4,(float)5.2e-4,(float)5.7e-4,(
	    float)6.1e-4,(float)6.6e-4,(float)3.8e-4,(float)1.6e-4,(float)
	    6.7e-5,(float)3.2e-5,(float)1.2e-5,(float)1.5e-7,(float)1e-9,(
	    float)0. };
    static real wo6[34] = { (float)5.4e-5,(float)5.4e-5,(float)5.4e-5,(float)
	    5e-5,(float)4.6e-5,(float)4.6e-5,(float)4.5e-5,(float)4.9e-5,(
	    float)5.2e-5,(float)7.1e-5,(float)9e-5,(float)1.3e-4,(float)
	    1.6e-4,(float)1.7e-4,(float)1.9e-4,(float)2.1e-4,(float)2.4e-4,(
	    float)2.8e-4,(float)3.2e-4,(float)3.5e-4,(float)3.8e-4,(float)
	    3.8e-4,(float)3.9e-4,(float)3.8e-4,(float)3.6e-4,(float)3.4e-4,(
	    float)2e-4,(float)1.1e-4,(float)4.9e-5,(float)1.7e-5,(float)4e-6,(
	    float)8.6e-8,(float)4.3e-11,(float)0. };

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
