/* ATMREF.f -- translated by f2c (version 19970805).
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
    doublereal delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

/*<    >*/
/* Subroutine */ int atmref_(integer *iaer, integer *iaer_prof__, doublereal *
	tamoy, doublereal *taer, doublereal *trmoy, doublereal *pizmoy, 
	doublereal *piza, doublereal *tamoyp, doublereal *taerp, doublereal *
	trmoyp, doublereal *palt, doublereal *phi, doublereal *xmus, 
	doublereal *xmuv, doublereal *phirad, integer *nt, integer *mu, 
	integer *np, doublereal *rm, doublereal *gb, doublereal *rp, 
	doublereal *rorayl, doublereal *roaero, doublereal *romix, doublereal 
	*rqrayl, doublereal *rqaero, doublereal *rqmix, doublereal *rurayl, 
	doublereal *ruaero, doublereal *rumix, integer *ipol, doublereal *
	xlm1, doublereal *xlm2, doublereal *rorayl_fi__, doublereal *
	romix_fi__, integer *nfi, integer *nfilut, doublereal *filut, 
	doublereal *rolut, doublereal *rolutq, doublereal *rolutu)
{
    /* System generated locals */
    integer rolut_dim1, rolut_offset, rolutq_dim1, rolutq_offset, rolutu_dim1,
	     rolutu_offset, filut_dim1, filut_offset, rm_offset, gb_offset, 
	    xlm2_dim1, xlm2_offset, xlm1_dim1, xlm1_offset, i__1;

    /* Local variables */
    integer i__, j;
    extern /* Subroutine */ int os_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     doublereal *);
    integer ifi;
    doublereal xqm1[2499]	/* was [51][49] */, xum1[2499]	/* was [51][
	    49] */, tamol;
    extern /* Subroutine */ int ospol_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    doublereal tamolp, xlphim[181], rolutd[1025]	/* was [25][41] */;

/*<       real    rolut(mu,41),rolutq(mu,41),rolutu(mu,41) >*/
/* THOMAS */
/* 			real    rolutd(mu,41) */
/*< 			real    rolutd(25,41) >*/
/*<       real    filut(mu,41) >*/
/*<       integer nfilut(mu) >*/
/*<       integer mu,np,nfi >*/
/*<       real rm(-mu:mu),rp(np),gb(-mu:mu) >*/
/*<       real tamoy,trmoy,pizmoy,tamoyp,trmoyp,palt >*/
/*<       real xlm2(-mu:mu,np) >*/
/* THOMAS */
/*      real xlphim(nfi) */
/*<       real xlphim(181) >*/
/*<       real rorayl_fi(nfi),romix_fi(nfi) >*/
/*<       real phi,xmus,xmuv,phirad >*/
/*<       real delta,sigma,tamol,tamolp >*/
/*<       real rorayl,roaero,romix,rqrayl,rqaero,rqmix >*/
/*<       real rurayl,ruaero,rumix >*/
/*<       real xlm1(-mu:mu,np) >*/
/* THOMAS */
/* 			real xqm1(-mu:mu,np),xum1(-mu:mu,np) */
/*< 			real xqm1(-25:25,49),xum1(-25:25,49) >*/
/*<        integer iaer,nt,ipol,iaer_prof >*/
/*<       common /sixs_del/ delta,sigma >*/

/*     atmospheric reflectances */
/*<       rorayl=0. >*/
    /* Parameter adjustments */
    rolutu_dim1 = *mu;
    rolutu_offset = rolutu_dim1 + 1;
    rolutu -= rolutu_offset;
    rolutq_dim1 = *mu;
    rolutq_offset = rolutq_dim1 + 1;
    rolutq -= rolutq_offset;
    rolut_dim1 = *mu;
    rolut_offset = rolut_dim1 + 1;
    rolut -= rolut_offset;
    filut_dim1 = *mu;
    filut_offset = filut_dim1 + 1;
    filut -= filut_offset;
    --nfilut;
    gb_offset = -(*mu);
    gb -= gb_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;
    xlm2_dim1 = *mu - (-(*mu)) + 1;
    xlm2_offset = -(*mu) + xlm2_dim1;
    xlm2 -= xlm2_offset;
    xlm1_dim1 = *mu - (-(*mu)) + 1;
    xlm1_offset = -(*mu) + xlm1_dim1;
    xlm1 -= xlm1_offset;
    --rp;
    --romix_fi__;
    --rorayl_fi__;

    /* Function Body */
    *rorayl = 0.;
/*<       roaero=0. >*/
    *roaero = 0.;
/*<       romix=0. >*/
    *romix = 0.;
/*<       rqrayl=0. >*/
    *rqrayl = 0.;
/*<       rqaero=0. >*/
    *rqaero = 0.;
/*<       rqmix=0. >*/
    *rqmix = 0.;
/*<       rurayl=999. >*/
    *rurayl = 999.;
/*<       ruaero=999. >*/
    *ruaero = 999.;
/*<       rumix=999. >*/
    *rumix = 999.;

/* 3 possible cases (satellite,plane,ground) */
/*<       if(palt.gt.0.0)then >*/
    if (*palt > 0.) {
/*<         rm(-mu)=-xmuv >*/
	rm[-(*mu)] = -(*xmuv);
/*<         rm(mu)=xmuv >*/
	rm[*mu] = *xmuv;
/*<         rm(0)=-xmus >*/
	rm[0] = -(*xmus);
/*  -----rayleigh reflectance = rorayl,rprayl */
/*<         tamol=0. >*/
	tamol = 0.;
/*<         tamolp=0. >*/
	tamolp = 0.;
/*<  	 >*/
	ospol_(iaer_prof__, &tamol, trmoy, piza, &tamolp, trmoyp, palt, 
		phirad, nt, mu, np, &rm[rm_offset], &gb[gb_offset], &rp[1], &
		xlm1[xlm1_offset], xqm1, xum1, xlphim, nfi, &nfilut[1], &
		filut[filut_offset], rolutd, rolutd, rolutd);
/*< 	if (ipol.ne.1)then >*/
	if (*ipol != 1) {
/*<           rorayl=xlm1(-mu,1)/xmus >*/
	    *rorayl = xlm1[-(*mu) + xlm1_dim1] / *xmus;
/*<           romix=rorayl >*/
	    *romix = *rorayl;
/*< 	do ifi=1,nfi >*/
	    i__1 = *nfi;
	    for (ifi = 1; ifi <= i__1; ++ifi) {
/*<            rorayl_fi(ifi)=(xlphim(ifi)/xmus) >*/
		rorayl_fi__[ifi] = xlphim[ifi - 1] / *xmus;
/*<            romix_fi(ifi)=(xlphim(ifi)/xmus) >*/
		romix_fi__[ifi] = xlphim[ifi - 1] / *xmus;
/*<         enddo >*/
	    }
/*< 	endif >*/
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*              -> here we define 2 reflectances from Stockes' par
ameters */
/*                but they don't have any physical interpretations
, this is*/
/*                 just to be coherent with the reflectance rorayl
 */
/*              -> parameters rorayl2,roaero2,romix2 have been int
roduced */
/*                 to compute the degrees of polarization. */
/*< 	  rorayl=xlm1(-mu,1)/xmus >*/
	    *rorayl = xlm1[-(*mu) + xlm1_dim1] / *xmus;
/*< 	  rqrayl=xqm1(-mu,1)/xmus >*/
	    *rqrayl = xqm1[-(*mu) + 25] / *xmus;
/*         WRITE(6,*) "TAURAYL=",trmoy," RORAYL = ",rorayl */
/*<           rqmix=rqrayl >*/
	    *rqmix = *rqrayl;
/*< 	  rurayl=xum1(-mu,1)/xmus >*/
	    *rurayl = xum1[-(*mu) + 25] / *xmus;
/*<           rumix=rurayl >*/
	    *rumix = *rurayl;
/*< 	do ifi=1,nfi >*/
	    i__1 = *nfi;
	    for (ifi = 1; ifi <= i__1; ++ifi) {
/*<            rorayl_fi(ifi)=(xlphim(ifi)/xmus) >*/
		rorayl_fi__[ifi] = xlphim[ifi - 1] / *xmus;
/*<         enddo >*/
	    }
/*< 	endif >*/
	}
/*       write(6,*)'ATMREF- rayleigh',rorayl,rqrayl,rurayl */
/*<         if (iaer.eq.0) then >*/
	if (*iaer == 0) {
/*< 	 romix=rorayl >*/
	    *romix = *rorayl;
/*< 	 rqmix=rqrayl >*/
	    *rqmix = *rqrayl;
/*< 	 rumix=rurayl >*/
	    *rumix = *rurayl;
/*< 	 roaero=0.0 >*/
	    *roaero = 0.;
/*< 	 rqaero=0.0 >*/
	    *rqaero = 0.;
/*< 	 ruaero=0.0 >*/
	    *ruaero = 0.;
/*<         return >*/
	    return 0;
/*< 	endif >*/
	}
/*  -----aerosol reflectance = roaero,rpaero */
/*<         tamol=0. >*/
	tamol = 0.;
/*<         tamolp=0. >*/
	tamolp = 0.;
/*< 	if (ipol.ne.1)then >*/
	if (*ipol != 1) {
/*<    >*/
	    os_(iaer_prof__, tamoy, &tamol, pizmoy, tamoyp, &tamolp, palt, 
		    phirad, nt, mu, np, &rm[rm_offset], &gb[gb_offset], &rp[1]
		    , &xlm1[xlm1_offset], xlphim, nfi, rolutd);
/*<           roaero=(xlm1(-mu,1)/xmus) >*/
	    *roaero = xlm1[-(*mu) + xlm1_dim1] / *xmus;
/*< 	endif >*/
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*<  	 >*/
	    ospol_(iaer_prof__, taer, &tamol, piza, taerp, &tamolp, palt, 
		    phirad, nt, mu, np, &rm[rm_offset], &gb[gb_offset], &rp[1]
		    , &xlm1[xlm1_offset], xqm1, xum1, xlphim, nfi, &nfilut[1],
		     &filut[filut_offset], rolutd, rolutd, rolutd);
/*< 	  rqaero=xqm1(-mu,1)/xmus >*/
	    *rqaero = xqm1[-(*mu) + 25] / *xmus;
/*< 	  ruaero=xum1(-mu,1)/xmus >*/
	    *ruaero = xum1[-(*mu) + 25] / *xmus;
/*< 	  if (ipol.eq.1)roaero=xlm1(-mu,1)/xmus >*/
	    if (*ipol == 1) {
		*roaero = xlm1[-(*mu) + xlm1_dim1] / *xmus;
	    }
/*< 	endif >*/
	}
/*       write(6,*)'ATMREF - aero', roaero,rqaero,ruaero */
/*  -----rayleigh+aerosol reflectance = romix,rpmix */
/*<         if (ipol.ne.1)then >*/
	if (*ipol != 1) {
/*<  	 >*/
	    os_(iaer_prof__, tamoy, trmoy, pizmoy, tamoyp, trmoyp, palt, 
		    phirad, nt, mu, np, &rm[rm_offset], &gb[gb_offset], &rp[1]
		    , &xlm1[xlm1_offset], xlphim, nfi, rolutd);
/*<           romix=(xlm1(-mu,1)/xmus) >*/
	    *romix = xlm1[-(*mu) + xlm1_dim1] / *xmus;
/*< 	do ifi=1,nfi >*/
	    i__1 = *nfi;
	    for (ifi = 1; ifi <= i__1; ++ifi) {
/*<            romix_fi(ifi)=(xlphim(ifi)/xmus) >*/
		romix_fi__[ifi] = xlphim[ifi - 1] / *xmus;
/*<         enddo >*/
	    }
/*< 	endif >*/
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	  >*/
	    ospol_(iaer_prof__, taer, trmoy, piza, taerp, trmoyp, palt, 
		    phirad, nt, mu, np, &rm[rm_offset], &gb[gb_offset], &rp[1]
		    , &xlm1[xlm1_offset], xqm1, xum1, xlphim, nfi, &nfilut[1],
		     &filut[filut_offset], &rolut[rolut_offset], &rolutq[
		    rolutq_offset], &rolutu[rolutu_offset]);
/*< 	  rqmix=xqm1(-mu,1)/xmus >*/
	    *rqmix = xqm1[-(*mu) + 25] / *xmus;
/*< 	  rumix=xum1(-mu,1)/xmus >*/
	    *rumix = xum1[-(*mu) + 25] / *xmus;
/*< 	  if (ipol.eq.1)then  >*/
	    if (*ipol == 1) {
/*< 	  romix=xlm1(-mu,1)/xmus >*/
		*romix = xlm1[-(*mu) + xlm1_dim1] / *xmus;
/*<           do ifi=1,nfi >*/
		i__1 = *nfi;
		for (ifi = 1; ifi <= i__1; ++ifi) {
/*<           romix_fi(ifi)=(xlphim(ifi)/xmus) >*/
		    romix_fi__[ifi] = xlphim[ifi - 1] / *xmus;
/*<           enddo >*/
		}
/*< 	  endif >*/
	    }
/*< 	endif >*/
	}
/*< 	do i=1,mu >*/
	i__1 = *mu;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*< 	do j=1,41 >*/
	    for (j = 1; j <= 41; ++j) {
/*< 	rolut(i,j)=rolut(i,j)/xmus >*/
		rolut[i__ + j * rolut_dim1] /= *xmus;
/*< 	rolutq(i,j)=rolutq(i,j)/xmus >*/
		rolutq[i__ + j * rolutq_dim1] /= *xmus;
/*< 	rolutu(i,j)=rolutu(i,j)/xmus >*/
		rolutu[i__ + j * rolutu_dim1] /= *xmus;
/*< 	enddo >*/
	    }
/*< 	enddo >*/
	}
/*       write(6,*)'ATMREF - mix',romix,rqmix,rumix */
/*<       endif >*/
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* atmref_ */

#ifdef __cplusplus
	}
#endif
