/* HAPKALBE.f -- translated by f2c (version 19970805).
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

/*<    >*/
/* Subroutine */ int hapkalbe_(doublereal *om, doublereal *af, doublereal *s0,
	 doublereal *h__, doublereal *brdfalb)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal), sqrt(
	    doublereal), pow_dd(doublereal *, doublereal *), acos(doublereal),
	     tan(doublereal);

    /* Local variables */
    doublereal f, g;
    integer j, k, l;
    doublereal h1, h2, p0, fa[48], bg, cg, fi, ta[24], pg, pi, si1, si2, mu1, 
	    mu2, h1h2, wfa[48], wta[24], phi1, phi2, pond, summ, teta1, teta2;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);

/*<       parameter (nta=24,nfa=48) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real om,af,s0,h,mu1,mu2 >*/
/*<       real brdfalb,summ,si2,si1 >*/
/*<       real fi,f,cg,h1,h2,h1h2,pg,p0,g,bg,pond >*/
/*<       real pi >*/
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
/*<       f=om/4./(mu2+mu1) >*/
		f = *om / 4. / (mu2 + mu1);
/*<       cg=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi) >*/
		cg = mu1 * mu2 + sqrt(1. - mu1 * mu1) * sqrt(1. - mu2 * mu2) *
			 cos(fi);
/*<       h1=(1.+2*mu1)/(1.+2.*sqrt(1.-om)*mu1) >*/
		h1 = (mu1 * 2 + 1.) / (sqrt(1. - *om) * 2. * mu1 + 1.);
/*<       h2=(1.+2*mu2)/(1.+2.*sqrt(1.-om)*mu2) >*/
		h2 = (mu2 * 2 + 1.) / (sqrt(1. - *om) * 2. * mu2 + 1.);
/*<       h1h2=h1*h2 >*/
		h1h2 = h1 * h2;
/*<       pg=(1-af*af)/((1+af*af+2*af*cg)**1.5) >*/
		d__1 = *af * *af + 1 + *af * 2 * cg;
		pg = (1 - *af * *af) / pow_dd(&d__1, &c_b7);
/*<       p0=(1-af*af)/((1+af*af+2*af)**1.5) >*/
		d__1 = *af * *af + 1 + *af * 2;
		p0 = (1 - *af * *af) / pow_dd(&d__1, &c_b7);
/*<       g=acos(cg) >*/
		g = acos(cg);
/*<       bg=(s0/(om*p0))/(1.+tan(g/2.)/h) >*/
		bg = *s0 / (*om * p0) / (tan(g / 2.) / *h__ + 1.);
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       brdfalb=brdfalb+f*((1.+bg)*pg+h1h2-1.)*pond >*/
		*brdfalb += f * ((bg + 1.) * pg + h1h2 - 1.) * pond;
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
} /* hapkalbe_ */

#ifdef __cplusplus
	}
#endif
