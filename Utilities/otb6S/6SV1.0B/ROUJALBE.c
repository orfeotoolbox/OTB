/* ROUJALBE.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int roujalbe_(doublereal *k0, doublereal *k1, doublereal *k2,
	 doublereal *brdfalb)
{
    /* Builtin functions */
    double atan(doublereal), sin(doublereal), cos(doublereal), acos(
	    doublereal), tan(doublereal), sqrt(doublereal);

    /* Local variables */
    integer j, k, l;
    doublereal f1, f2, fa[48], ta[24], fr, pi, ft, ts, tv, si1, si2, mu1, mu2,
	     wfa[48], wta[24], psi, tts, ttv, phi1, phi2, cpsi, pond, summ, 
	    xmus, xmuv, teta1, teta2;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);

/*<       parameter (nta=24,nfa=48) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real k0,k1,k2,brdfalb,psi,cpsi,mu1,mu2,si1,si2,ft >*/
/*<       real pi >*/
/*<       real summ,ts,tv,fr,pond,tts,ttv,xmus,xmuv,f2,f1 >*/
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
/*<       fr=acos(cos(fa(k))) >*/
		fr = acos(cos(fa[k - 1]));
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       tts=tan(ts) >*/
		tts = tan(ts);
/*<       ttv=tan(tv) >*/
		ttv = tan(tv);
/*<       xmus=cos(ts) >*/
		xmus = cos(ts);
/*<       xmuv=cos(tv) >*/
		xmuv = cos(tv);
/*<       cpsi=xmus*xmuv+sin(ts)*sin(tv)*cos(fr) >*/
		cpsi = xmus * xmuv + sin(ts) * sin(tv) * cos(fr);
/*<       if (cpsi.lt.1.) then >*/
		if (cpsi < 1.) {
/*<       psi=acos(cpsi) >*/
		    psi = acos(cpsi);
/*<       else >*/
		} else {
/*<       psi=0. >*/
		    psi = 0.;
/*<       endif >*/
		}
/*<       f2=4./(3.*pi*(xmus+xmuv))*((pi/2-psi)*cpsi+sin(psi))-1./3. >*/
		f2 = 4. / (pi * 3. * (xmus + xmuv)) * ((pi / 2 - psi) * cpsi 
			+ sin(psi)) - .33333333333333331;
/*<       ft=tts*tts+ttv*ttv-2*tts*ttv*cos(fr) >*/
		ft = tts * tts + ttv * ttv - tts * 2 * ttv * cos(fr);
/*<       f1=0.5*((pi-fr)*cos(fr)+sin(fr))*tts*ttv-tts-ttv-sqrt(ft) >*/
		f1 = ((pi - fr) * cos(fr) + sin(fr)) * .5 * tts * ttv - tts - 
			ttv - sqrt(ft);
/*<       f1=f1/pi >*/
		f1 /= pi;
/*<       brdfalb=brdfalb+(k0+k1*f1+k2*f2)*pond >*/
		*brdfalb += (*k0 + *k1 * f1 + *k2 * f2) * pond;
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
} /* roujalbe_ */

#ifdef __cplusplus
	}
#endif
