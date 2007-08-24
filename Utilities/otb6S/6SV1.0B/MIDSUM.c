/* MIDSUM.f -- translated by f2c (version 19970805).
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

/*<       subroutine   midsum >*/
/* Subroutine */ int midsum_()
{
    /* Initialized data */

    static real z2[34] = { (float)0.,(float)1.,(float)2.,(float)3.,(float)4.,(
	    float)5.,(float)6.,(float)7.,(float)8.,(float)9.,(float)10.,(
	    float)11.,(float)12.,(float)13.,(float)14.,(float)15.,(float)16.,(
	    float)17.,(float)18.,(float)19.,(float)20.,(float)21.,(float)22.,(
	    float)23.,(float)24.,(float)25.,(float)30.,(float)35.,(float)40.,(
	    float)45.,(float)50.,(float)70.,(float)100.,(float)99999. };
    static real p2[34] = { (float)1013.,(float)902.,(float)802.,(float)710.,(
	    float)628.,(float)554.,(float)487.,(float)426.,(float)372.,(float)
	    324.,(float)281.,(float)243.,(float)209.,(float)179.,(float)153.,(
	    float)130.,(float)111.,(float)95.,(float)81.2,(float)69.5,(float)
	    59.5,(float)51.,(float)43.7,(float)37.6,(float)32.2,(float)27.7,(
	    float)13.2,(float)6.52,(float)3.33,(float)1.76,(float).951,(float)
	    .0671,(float)3e-4,(float)0. };
    static real t2[34] = { (float)294.,(float)290.,(float)285.,(float)279.,(
	    float)273.,(float)267.,(float)261.,(float)255.,(float)248.,(float)
	    242.,(float)235.,(float)229.,(float)222.,(float)216.,(float)216.,(
	    float)216.,(float)216.,(float)216.,(float)216.,(float)217.,(float)
	    218.,(float)219.,(float)220.,(float)222.,(float)223.,(float)224.,(
	    float)234.,(float)245.,(float)258.,(float)270.,(float)276.,(float)
	    218.,(float)210.,(float)210. };
    static real wh2[34] = { (float)14.,(float)9.3,(float)5.9,(float)3.3,(
	    float)1.9,(float)1.,(float).61,(float).37,(float).21,(float).12,(
	    float).064,(float).022,(float).006,(float).0018,(float).001,(
	    float)7.6e-4,(float)6.4e-4,(float)5.6e-4,(float)5e-4,(float)
	    4.9e-4,(float)4.5e-4,(float)5.1e-4,(float)5.1e-4,(float)5.4e-4,(
	    float)6e-4,(float)6.7e-4,(float)3.6e-4,(float)1.1e-4,(float)
	    4.3e-5,(float)1.9e-5,(float)1.3e-6,(float)1.4e-7,(float)1e-9,(
	    float)0. };
    static real wo2[34] = { (float)6e-5,(float)6e-5,(float)6e-5,(float)6.2e-5,
	    (float)6.4e-5,(float)6.6e-5,(float)6.9e-5,(float)7.5e-5,(float)
	    7.9e-5,(float)8.6e-5,(float)9e-5,(float)1.1e-4,(float)1.2e-4,(
	    float)1.5e-4,(float)1.8e-4,(float)1.9e-4,(float)2.1e-4,(float)
	    2.4e-4,(float)2.8e-4,(float)3.2e-4,(float)3.4e-4,(float)3.6e-4,(
	    float)3.6e-4,(float)3.4e-4,(float)3.2e-4,(float)3e-4,(float)2e-4,(
	    float)9.2e-5,(float)4.1e-5,(float)1.3e-5,(float)4.3e-6,(float)
	    8.6e-8,(float)4.3e-11,(float)0. };

    integer i__;

/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       real z2(34),p2(34),t2(34),wh2(34),wo2(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       integer i >*/

/*     model: midlatitude summer mc clatchey */

/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<       z(i)=z2(i) >*/
	sixs_atm__1.z__[i__ - 1] = z2[i__ - 1];
/*<       p(i)=p2(i) >*/
	sixs_atm__1.p[i__ - 1] = p2[i__ - 1];
/*<       t(i)=t2(i) >*/
	sixs_atm__1.t[i__ - 1] = t2[i__ - 1];
/*<       wh(i)=wh2(i) >*/
	sixs_atm__1.wh[i__ - 1] = wh2[i__ - 1];
/*<       wo(i)=wo2(i) >*/
	sixs_atm__1.wo[i__ - 1] = wo2[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* midsum_ */

#ifdef __cplusplus
	}
#endif
