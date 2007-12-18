/* WALTALBE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Table of constant values */

static integer c__24 = 24;
static integer c__48 = 48;

/*<    >*/
/* Subroutine */ int waltalbe_(doublereal *a, doublereal *ap, doublereal *b, 
	doublereal *c__, doublereal *brdfalb)
{
    /* Builtin functions */
    double atan(doublereal), sin(doublereal), cos(doublereal);

    /* Local variables */
    integer j, k, l;
    doublereal fa[48], ta[24], pi, ts, tv, si1, si2, mu1, mu2, wfa[48], phi, 
	    wta[24], phi1, phi2, pond, summ, teta1, teta2;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    doublereal brdfint;

/*<       parameter (nta=24,nfa=48) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real a,ap,b,c >*/
/*<       real pi >*/
/*<       real brdfalb,summ,si2,si1,ts,tv,phi,pond,brdfint,mu1,mu2 >*/
/*<       integer k,j,l >*/
/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       teta1=0. >*/
    teta1 = 0.;
/*<       teta2=pi/2. >*/
    teta2 = pi / 2.;
/*<       call gauss(teta1,teta2,ta,wta,nta) >*/
    gauss_(&teta1, &teta2, ta, wta, &c__24);
/*<       phi1=0. >*/
    phi1 = 0.;
/*<       phi2=2.*pi >*/
    phi2 = pi * 2.;
/*<       call gauss(phi1,phi2,fa,wfa,nfa) >*/
    gauss_(&phi1, &phi2, fa, wfa, &c__48);
/*<       brdfalb=0. >*/
    *brdfalb = 0.;
/*<       summ=0. >*/
    summ = 0.;
/*<       do 1 k=1,nfa >*/
    for (k = 1; k <= 48; ++k) {
/*<       do 2 j=1,nta >*/
	for (j = 1; j <= 24; ++j) {
/*<       do 3 l=1,nta >*/
	    for (l = 1; l <= 24; ++l) {
/*<       si2=sin(ta(j)) >*/
		si2 = sin(ta[j - 1]);
/*<       si1=sin(ta(l)) >*/
		si1 = sin(ta[l - 1]);
/*<       mu2=cos(ta(j)) >*/
		mu2 = cos(ta[j - 1]);
/*<       mu1=cos(ta(l)) >*/
		mu1 = cos(ta[l - 1]);
/*<       ts=ta(j) >*/
		ts = ta[j - 1];
/*<       tv=ta(l) >*/
		tv = ta[l - 1];
/*<       phi=fa(k) >*/
		phi = fa[k - 1];
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       brdfint=a*ts*ts*tv*tv+ap*(ts*ts+tv*tv)+b*ts*tv*cos(phi)+c >*/
		brdfint = *a * ts * ts * tv * tv + *ap * (ts * ts + tv * tv) 
			+ *b * ts * tv * cos(phi) + *c__;
/*<       brdfalb=brdfalb+brdfint*pond >*/
		*brdfalb += brdfint * pond;
/*<       summ=summ+pond >*/
		summ += pond;
/*<   3   continue >*/
/* L3: */
	    }
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       brdfalb=brdfalb/summ >*/
    *brdfalb /= summ;
/*<       return >*/
    return 0;
/*<       end >*/
} /* waltalbe_ */

#ifdef __cplusplus
	}
#endif
