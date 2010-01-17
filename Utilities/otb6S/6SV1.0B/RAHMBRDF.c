/* RAHMBRDF.f -- translated by f2c (version 19970805).
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

/*<       Subroutine RAHMBRDF (rho0,af,xk,mu,np,rm,rp,brdfint) >*/
/* Subroutine */ int rahmbrdf_(doublereal *rho0, doublereal *af, doublereal *
	xk, integer *mu, integer *np, doublereal *rm, doublereal *rp, 
	doublereal *brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double acos(doublereal), sqrt(doublereal), cos(doublereal), pow_dd(
	    doublereal *, doublereal *);

    /* Local variables */
    integer j, k;
    doublereal fi, pi, mu1, mu2, coef1, coef2, tante1, tante2, geofac, phaang,
	     cospha, phafun;

/* ***********************************************************************
 */

/*           A semi-empirical bidirectional reflectance model */

/*  Purpose: */

/*  To generate a single bidirectional reflectance factor value for a */
/*  semi-infinite medium, given the illumination and viewing geometry, */
/*  the optical and the structural properties of the scatterers. */

/*  Definitions: */
/*   geometrical conditions */
/*     mu1          : Illumination zenith angle, in radians */
/*     mu2          : Observation zenith angle, in radians */
/*     fi           : Relative azimuth angle, in radians */
/*   optical characteristics of the scatterers: */
/*     Rho0         : Intensity of the reflectance of the surface cover. 
*/
/*                    N/D value greater or equal to 0.0 (Rho_0) */
/*     af           : Phase function parameter: */
/*                    Asymmetry factor, N/D value between -1.0 and 1.0 */
/*     xk           : Structural parameter of the medium */

/*  References: */

/*        [1] R. Rahman, Verstraete M., and Pinty B., 1993, `A coupled */
/*            surface-atmosphere reflectance (CSAR) model. Part 1: Model 
*/
/*            Description and Inversion on Synthetic Data', submitted to 
*/
/*            JGR. */
/*        [2] R. Rahman, Pinty B., and Verstraete M., 1993, `A coupled */
/*            surface-atmosphere reflectance (CSAR) model. Part 2: a */
/*            semi-empirical model usable with NOAA/AVHRR data', */
/*            submitted to JGR. */

/* ***********************************************************************
 */

/*<       integer mu,np,j,k >*/
/*<       real xk,af,rho0 >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real coef1,coef2,cospha,geofac >*/
/*<       real fi,mu1,mu2,phaang,phafun,pi,tante1,tante2 >*/

/*<       pi =acos(-1.0) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
    pi = acos(-1.);
/*<       mu1=rm(0) >*/
    mu1 = rm[0];
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       mu2=rm(j) >*/
	    mu2 = rm[j];
/*<       if (j.eq.mu)then >*/
	    if (j == *mu) {
/*< 	 fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*< 	 else >*/
	    } else {
/*< 	 fi=rp(k)+rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<       endif >*/
	    }
/* Compute various trigonometric expressions: */
/*<       cospha=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi) >*/
	    cospha = mu1 * mu2 + sqrt(1. - mu1 * mu1) * sqrt(1. - mu2 * mu2) *
		     cos(fi);
/*<       phaang=acos(cospha) >*/
	    phaang = acos(cospha);
/*<       tante1=sqrt(1.-mu1*mu1)/mu1 >*/
	    tante1 = sqrt(1. - mu1 * mu1) / mu1;
/*<       tante2=sqrt(1.-mu2*mu2)/mu2 >*/
	    tante2 = sqrt(1. - mu2 * mu2) / mu2;
/*<       geofac=sqrt(tante1*tante1+tante2*tante2-2.0*tante1*tante2*cos(fi)) >*/
	    geofac = sqrt(tante1 * tante1 + tante2 * tante2 - tante1 * 2. * 
		    tante2 * cos(fi));
/* Compute the first term */
/*<       coef1=(mu1**(xk-1.))*(mu2**(xk-1.))/((mu1+mu2)**(1.-xk)) >*/
	    d__1 = *xk - 1.;
	    d__2 = *xk - 1.;
	    d__3 = mu1 + mu2;
	    d__4 = 1. - *xk;
	    coef1 = pow_dd(&mu1, &d__1) * pow_dd(&mu2, &d__2) / pow_dd(&d__3, 
		    &d__4);
/* Compute the phase function: */
/*<       phafun=(1.0-af*af)/((1.0+af*af-2.0*af*cos(pi-phaang))**1.5) >*/
	    d__1 = *af * *af + 1. - *af * 2. * cos(pi - phaang);
	    phafun = (1. - *af * *af) / pow_dd(&d__1, &c_b4);
/* Compute the opposition (hot spot) function: */
/*<       coef2=1.+(1.-rho0)/(1.+geofac) >*/
	    coef2 = (1. - *rho0) / (geofac + 1.) + 1.;
/* Compute the bidirectional reflectance factor: */
/*<       brdfint(j,k)=rho0*coef1*phafun*coef2 >*/
	    brdfint[j + k * brdfint_dim1] = *rho0 * coef1 * phafun * coef2;
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* rahmbrdf_ */

#ifdef __cplusplus
	}
#endif
