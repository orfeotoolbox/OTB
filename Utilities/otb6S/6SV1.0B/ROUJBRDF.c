/* ROUJBRDF.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine roujbrdf(k0,k1,k2,mu,np,rm,rp,brdfint) >*/
/* Subroutine */ int roujbrdf_(doublereal *k0, doublereal *k1, doublereal *k2,
	 integer *mu, integer *np, doublereal *rm, doublereal *rp, doublereal 
	*brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), acos(doublereal), tan(
	    doublereal), sin(doublereal), sqrt(doublereal);

    /* Local variables */
    integer j, k;
    doublereal f1, f2, fi, fr, pi, psi, tts, ttv, cpsi, xmus, xmuv;

/* model can be found in JGR 1992 paper, Vol. 97,No D18, Page20,445-20,468
 */
/*<       integer mu,np >*/
/*<       dimension rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real k0,k1,k2,pi,psi,cpsi >*/
/*<       real rm,rp,brdfint,xmus,xmuv,fi,fr,tts,ttv,f2,f1 >*/
/*<       integer k,j >*/
/*<       xmus=rm(0) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
    xmus = rm[0];
/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       xmuv=rm(j) >*/
	    xmuv = rm[j];
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
/*<       fr=acos(cos(fi)) >*/
	    fr = acos(cos(fi));
/*<       tts=tan(acos(xmus)) >*/
	    tts = tan(acos(xmus));
/*<       ttv=tan(acos(xmuv)) >*/
	    ttv = tan(acos(xmuv));
/*<       cpsi=xmus*xmuv+sin(acos(xmus))*sin(acos(xmuv))*cos(fi) >*/
	    cpsi = xmus * xmuv + sin(acos(xmus)) * sin(acos(xmuv)) * cos(fi);
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
	    f2 = 4. / (pi * 3. * (xmus + xmuv)) * ((pi / 2 - psi) * cpsi + 
		    sin(psi)) - .33333333333333331;
/*<    >*/
	    f1 = ((pi - fr) * cos(fr) + sin(fr)) * .5 * tts * ttv - tts - ttv 
		    - sqrt(tts * tts + ttv * ttv - tts * 2 * ttv * cos(fr));
/*<       f1=f1/pi >*/
	    f1 /= pi;
/*<       brdfint(j,k)=k0+k1*f1+k2*f2 >*/
	    brdfint[j + k * brdfint_dim1] = *k0 + *k1 * f1 + *k2 * f2;
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* roujbrdf_ */

#ifdef __cplusplus
	}
#endif
