/* RAHMALBE.f -- translated by f2c (version 19970805).
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
static doublereal c_b7 = 1.5;

/*<       Subroutine RAHMALBE (rho0,af,xk,brdfalb) >*/
/* Subroutine */ int rahmalbe_(doublereal *rho0, doublereal *af, doublereal *
	xk, doublereal *brdfalb)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double acos(doublereal), cos(doublereal), sin(doublereal), sqrt(
	    doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    integer j, k, l;
    doublereal fa[48], fi, ta[24], pi, si1, si2, mu1, mu2, wfa[48], wta[24], 
	    phi1, phi2, pond, summ, coef1, coef2, teta1, teta2;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    doublereal tante1, tante2, geofac, phaang, cospha, phafun;

/*<       parameter (nta=24,nfa=48) >*/

/* see RAHMBRDF.f for description */

/*<       integer j,k,l >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real xk,af,rho0 >*/
/*<       real brdfalb,summ,si1,si2,pond >*/
/*<       real coef1,coef2,cospha,geofac >*/
/*<       real fi,mu1,mu2,phaang,phafun,pi,tante1,tante2 >*/

/*<       pi =acos(-1.0) >*/
    pi = acos(-1.);
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
/*<       mu2=cos(ta(j)) >*/
		mu2 = cos(ta[j - 1]);
/*<       mu1=cos(ta(l)) >*/
		mu1 = cos(ta[l - 1]);
/*<       si2=sin(ta(j)) >*/
		si2 = sin(ta[j - 1]);
/*<       si1=sin(ta(l)) >*/
		si1 = sin(ta[l - 1]);
/*<       fi=fa(k) >*/
		fi = fa[k - 1];
/* Compute various trigonometric expressions: */
/*<       cospha=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi) >*/
		cospha = mu1 * mu2 + sqrt(1. - mu1 * mu1) * sqrt(1. - mu2 * 
			mu2) * cos(fi);
/*<       phaang=acos(cospha) >*/
		phaang = acos(cospha);
/*<       tante1=sqrt(1.-mu1*mu1)/mu1 >*/
		tante1 = sqrt(1. - mu1 * mu1) / mu1;
/*<       tante2=sqrt(1.-mu2*mu2)/mu2 >*/
		tante2 = sqrt(1. - mu2 * mu2) / mu2;
/*<       geofac=sqrt(tante1*tante1+tante2*tante2-2.0*tante1*tante2*cos(fi)) >*/
		geofac = sqrt(tante1 * tante1 + tante2 * tante2 - tante1 * 2. 
			* tante2 * cos(fi));
/* Compute the first term */
/*<       coef1=(mu1**(xk-1.))*(mu2**(xk-1.))/((mu1+mu2)**(1.-xk)) >*/
		d__1 = *xk - 1.;
		d__2 = *xk - 1.;
		d__3 = mu1 + mu2;
		d__4 = 1. - *xk;
		coef1 = pow_dd(&mu1, &d__1) * pow_dd(&mu2, &d__2) / pow_dd(&
			d__3, &d__4);
/* Compute the phase function: */
/*<       phafun=(1.0-af*af)/((1.0+af*af-2.0*af*cos(pi-phaang))**1.5) >*/
		d__1 = *af * *af + 1. - *af * 2. * cos(pi - phaang);
		phafun = (1. - *af * *af) / pow_dd(&d__1, &c_b7);
/* Compute the opposition (hot spot) function: */
/*<       coef2=1.+(1.-rho0)/(1.+geofac) >*/
		coef2 = (1. - *rho0) / (geofac + 1.) + 1.;
/* Compute the bidirectional reflectance factor: */
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       brdfalb=brdfalb+rho0*coef1*phafun*coef2*pond >*/
		*brdfalb += *rho0 * coef1 * phafun * coef2 * pond;
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
} /* rahmalbe_ */

#ifdef __cplusplus
	}
#endif
