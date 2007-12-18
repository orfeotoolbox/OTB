/* EQUIVWL.f -- translated by f2c (version 19970805).
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
    doublereal s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

/*<       subroutine equivwl(iinf,isup,step,wlmoy) >*/
/* Subroutine */ int equivwl_(integer *iinf, integer *isup, doublereal *step, 
	doublereal *wlmoy)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer l;
    doublereal wl, seb, swl, coef, sbor, wlwave;
    extern /* Subroutine */ int solirr_(doublereal *, doublereal *);

/*<       common /sixs_ffu/s(1501),wlinf,wlsup >*/
/*<       real step,wlmoy,s,wlinf,wlsup,seb,wlwave,sbor,wl,swl,coef >*/
/*<       integer iinf,isup,l >*/
/*<       seb=0. >*/
    seb = 0.;
/*<       wlwave=0. >*/
    wlwave = 0.;
/*<       do 50 l=iinf,isup >*/
    i__1 = *isup;
    for (l = *iinf; l <= i__1; ++l) {
/*<         sbor=s(l) >*/
	sbor = sixs_ffu__1.s[l - 1];
/*<         if(l.eq.iinf.or.l.eq.isup) sbor=sbor*0.5 >*/
	if (l == *iinf || l == *isup) {
	    sbor *= .5;
	}
/*<         wl=.25+(l-1)*step >*/
	wl = (l - 1) * *step + .25;
/*<         call solirr(wl,swl) >*/
	solirr_(&wl, &swl);
/*<         coef=sbor*step*swl >*/
	coef = sbor * *step * swl;
/*<         seb=seb+coef >*/
	seb += coef;
/*<         wlwave=wlwave+wl*coef >*/
	wlwave += wl * coef;
/*<   50  continue >*/
/* L50: */
    }
/*<       wlmoy=wlwave/seb >*/
    *wlmoy = wlwave / seb;
/*<       return >*/
    return 0;
/*<       end >*/
} /* equivwl_ */

#ifdef __cplusplus
	}
#endif
