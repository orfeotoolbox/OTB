/* HAPKBRDF.f -- translated by f2c (version 19970805).
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

static doublereal c_b4 = 1.5;

/*<    >*/
/* Subroutine */ int hapkbrdf_(doublereal *om, doublereal *af, doublereal *s0,
	 doublereal *h__, integer *mu, integer *np, doublereal *rm, 
	doublereal *rp, doublereal *brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), cos(doublereal), pow_dd(doublereal *, doublereal 
	    *), acos(doublereal), tan(doublereal);

    /* Local variables */
    doublereal f, g;
    integer j, k;
    doublereal h1, h2, p0, bg, cg, fi, pg, mu1, mu2, h1h2;

/*<       integer mu,np,k,j >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real f,h1 >*/
/*<       real h2,h1h2,pg,p0,g,bg >*/
/*<       real mu1,mu2,fi,cg >*/
/*<       real om,af,s0,h >*/
/* here the notation are taken according to j.g.r.,vol 95,no d8,pp 11767, 
*/
/* geometrical conditions: mu1 cosine of sun  zenith angle */
/*                         mu2 cosine of view zenith angle */
/*                         fi: fi1-fi2 [radians] */
/*                         cg: cos(g) */
/* canopy parameters     : om  (albedo) */
/* 			: af assymetry parameter for the phase function */
/* 			: s0 amplitude of hot spot */
/* 			: h width of the hot spot */
/*<       mu1=rm(0) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
    mu1 = rm[0];
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       mu2=rm(j) >*/
	    mu2 = rm[j];
/*<       if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<          fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<          else >*/
	    } else {
/*<          fi=rp(k)+rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<          endif >*/
	    }
/*<       cg=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi) >*/
	    cg = mu1 * mu2 + sqrt(1. - mu1 * mu1) * sqrt(1. - mu2 * mu2) * 
		    cos(fi);
/*<       f=om/4./(mu2+mu1) >*/
	    f = *om / 4. / (mu2 + mu1);
/*<       h1=(1.+2*mu1)/(1.+2.*sqrt(1.-om)*mu1) >*/
	    h1 = (mu1 * 2 + 1.) / (sqrt(1. - *om) * 2. * mu1 + 1.);
/*<       h2=(1.+2*mu2)/(1.+2.*sqrt(1.-om)*mu2) >*/
	    h2 = (mu2 * 2 + 1.) / (sqrt(1. - *om) * 2. * mu2 + 1.);
/*<       h1h2=h1*h2 >*/
	    h1h2 = h1 * h2;
/*<       pg=(1-af*af)/((1+af*af+2*af*cg)**1.5) >*/
	    d__1 = *af * *af + 1 + *af * 2 * cg;
	    pg = (1 - *af * *af) / pow_dd(&d__1, &c_b4);
/*<       p0=(1-af*af)/((1+af*af+2*af)**1.5) >*/
	    d__1 = *af * *af + 1 + *af * 2;
	    p0 = (1 - *af * *af) / pow_dd(&d__1, &c_b4);
/*<       g=acos(cg) >*/
	    g = acos(cg);
/*<       bg=(s0/(om*p0))/(1+tan(g/2.)/h) >*/
	    bg = *s0 / (*om * p0) / (tan(g / 2.) / *h__ + 1);
/*<       brdfint(j,k)=f*((1.+bg)*pg+h1h2-1.) >*/
	    brdfint[j + k * brdfint_dim1] = f * ((bg + 1.) * pg + h1h2 - 1.);
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* hapkbrdf_ */

#ifdef __cplusplus
	}
#endif
