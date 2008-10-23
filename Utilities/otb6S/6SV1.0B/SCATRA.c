/* SCATRA.f -- translated by f2c (version 19970805).
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

static doublereal c_b3 = 999.;

/*<    >*/
/* Subroutine */ int scatra_(integer *iaer_prof__, doublereal *taer, 
	doublereal *taerp, doublereal *tray, doublereal *trayp, doublereal *
	piza, doublereal *palt, integer *nt, integer *mu, doublereal *rm, 
	doublereal *gb, doublereal *xmus, doublereal *xmuv, doublereal *
	ddirtt, doublereal *ddiftt, doublereal *udirtt, doublereal *udiftt, 
	doublereal *sphalbt, doublereal *ddirtr, doublereal *ddiftr, 
	doublereal *udirtr, doublereal *udiftr, doublereal *sphalbr, 
	doublereal *ddirta, doublereal *ddifta, doublereal *udirta, 
	doublereal *udifta, doublereal *sphalba)
{
    /* System generated locals */
    integer rm_offset, gb_offset;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    integer it;
    extern /* Subroutine */ int iso_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, doublereal *, doublereal *, doublereal *);
    doublereal tamol;
    extern /* Subroutine */ int csalbr_(doublereal *, doublereal *);
    doublereal tamolp, xtrans[3];

/*<       integer mu >*/
/*<       real rm(-mu:mu),gb(-mu:mu) >*/
/*     computations of the direct and diffuse transmittances */
/*     for downward and upward paths , and spherical albedo */
/*<       real xtrans(-1:1) >*/
/*<       real taer,taerp,tray,trayp,piza,palt,xmus,xmuv >*/
/*<       real udiftt,sphalbt,ddirtr,ddiftr,udirtr,udiftr,sphalbr >*/
/*<       real ddirtt,ddiftt,udirtt,ddirta,ddifta,udirta,udifta >*/
/*<       real sphalba,tamol,tamolp >*/
/*<       integer nt,it,iaer_prof >*/

/*<       ddirtt=1. >*/
    /* Parameter adjustments */
    gb_offset = -(*mu);
    gb -= gb_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;

    /* Function Body */
    *ddirtt = 1.;
/*<       ddiftt=0. >*/
    *ddiftt = 0.;
/*<       udirtt=1. >*/
    *udirtt = 1.;
/*<       udiftt=0. >*/
    *udiftt = 0.;
/*<       sphalbt=0. >*/
    *sphalbt = 0.;
/*<       ddirtr=1. >*/
    *ddirtr = 1.;
/*<       ddiftr=0. >*/
    *ddiftr = 0.;
/*<       udirtr=1. >*/
    *udirtr = 1.;
/*<       udiftr=0. >*/
    *udiftr = 0.;
/*<       sphalbr=0. >*/
    *sphalbr = 0.;
/*<       ddirta=1. >*/
    *ddirta = 1.;
/*<       ddifta=0. >*/
    *ddifta = 0.;
/*<       udirta=1. >*/
    *udirta = 1.;
/*<       udifta=0. >*/
    *udifta = 0.;
/*<       sphalba=0. >*/
    *sphalba = 0.;
/*<       do 1 it=1,3 >*/
    for (it = 1; it <= 3; ++it) {
/* it=1 rayleigh only, it=2 aerosol only, it=3 rayleigh+aerosol */
/*<         if (it.eq.2.and.taer.le.0.) goto 1 >*/
	if (it == 2 && *taer <= 0.) {
	    goto L1;
	}
/*     compute upward,downward diffuse transmittance for rayleigh,aero
sol */
/*<         if (it.eq.1) then >*/
	if (it == 1) {
/*<           if (palt.gt.900) then >*/
	    if (*palt > 900.) {
/*<             udiftt=(2./3.+xmuv)+(2./3.-xmuv)*exp(-tray/xmuv) >*/
		*udiftt = *xmuv + .66666666666666663 + (.66666666666666663 - *
			xmuv) * exp(-(*tray) / *xmuv);
/*<             udiftt=udiftt/((4./3.)+tray)-exp(-tray/xmuv) >*/
		*udiftt = *udiftt / (*tray + 1.3333333333333333) - exp(-(*
			tray) / *xmuv);
/*<             ddiftt=(2./3.+xmus)+(2./3.-xmus)*exp(-tray/xmus) >*/
		*ddiftt = *xmus + .66666666666666663 + (.66666666666666663 - *
			xmus) * exp(-(*tray) / *xmus);
/*<             ddiftt=ddiftt/((4./3.)+tray)-exp(-tray/xmus) >*/
		*ddiftt = *ddiftt / (*tray + 1.3333333333333333) - exp(-(*
			tray) / *xmus);
/*<             ddirtt=exp(-tray/xmus) >*/
		*ddirtt = exp(-(*tray) / *xmus);
/*<             udirtt=exp(-tray/xmuv) >*/
		*udirtt = exp(-(*tray) / *xmuv);
/*<             call csalbr(tray,sphalbt) >*/
		csalbr_(tray, sphalbt);
/*<           endif >*/
	    }
/*<           if (palt.lt.900) then >*/
	    if (*palt < 900.) {
/*<             tamol=0. >*/
		tamol = 0.;
/*<             tamolp=0. >*/
		tamolp = 0.;
/*<             rm(-mu)=-xmuv >*/
		rm[-(*mu)] = -(*xmuv);
/*<             rm(mu)=xmuv >*/
		rm[*mu] = *xmuv;
/*<             rm(0)=xmus >*/
		rm[0] = *xmus;
/*<    >*/
		iso_(iaer_prof__, &tamol, tray, piza, &tamolp, trayp, palt, 
			nt, mu, &rm[rm_offset], &gb[gb_offset], xtrans);
/*<             udiftt=xtrans(-1)-exp(-trayp/xmuv) >*/
		*udiftt = xtrans[0] - exp(-(*trayp) / *xmuv);
/*<             udirtt=exp(-trayp/xmuv) >*/
		*udirtt = exp(-(*trayp) / *xmuv);
/*<             rm(-mu)=-xmus >*/
		rm[-(*mu)] = -(*xmus);
/*<             rm(mu)=xmus >*/
		rm[*mu] = *xmus;
/*<             rm(0)=xmus >*/
		rm[0] = *xmus;
/*<             ddiftt=(2./3.+xmus)+(2./3.-xmus)*exp(-tray/xmus) >*/
		*ddiftt = *xmus + .66666666666666663 + (.66666666666666663 - *
			xmus) * exp(-(*tray) / *xmus);
/*<             ddiftt=ddiftt/((4./3.)+tray)-exp(-tray/xmus) >*/
		*ddiftt = *ddiftt / (*tray + 1.3333333333333333) - exp(-(*
			tray) / *xmus);
/*<             ddirtt=exp(-tray/xmus) >*/
		*ddirtt = exp(-(*tray) / *xmus);
/*<             udirtt=exp(-tray/xmuv) >*/
		*udirtt = exp(-(*tray) / *xmuv);
/*<             call csalbr(tray,sphalbt) >*/
		csalbr_(tray, sphalbt);
/*<           endif >*/
	    }
/*<           if (palt.le.0.) then >*/
	    if (*palt <= 0.) {
/*<             udiftt=0. >*/
		*udiftt = 0.;
/*<             udirtt=1. >*/
		*udirtt = 1.;
/*<           endif >*/
	    }
/*<         endif >*/
	}
/*<       if (it.eq.2) then >*/
	if (it == 2) {
/*<         tamol=0. >*/
	    tamol = 0.;
/*<         tamolp=0. >*/
	    tamolp = 0.;
/*<         rm(-mu)=-xmuv >*/
	    rm[-(*mu)] = -(*xmuv);
/*<         rm(mu)=xmuv >*/
	    rm[*mu] = *xmuv;
/*<         rm(0)=xmus >*/
	    rm[0] = *xmus;
/*<    >*/
	    iso_(iaer_prof__, taer, &tamol, piza, taerp, &tamolp, palt, nt, 
		    mu, &rm[rm_offset], &gb[gb_offset], xtrans);
/*<         udiftt=xtrans(-1)-exp(-taerp/xmuv) >*/
	    *udiftt = xtrans[0] - exp(-(*taerp) / *xmuv);
/*<         udirtt=exp(-taerp/xmuv) >*/
	    *udirtt = exp(-(*taerp) / *xmuv);
/*<         rm(-mu)=-xmus >*/
	    rm[-(*mu)] = -(*xmus);
/*<         rm(mu)=xmus >*/
	    rm[*mu] = *xmus;
/*<         rm(0)=xmus >*/
	    rm[0] = *xmus;
/*<    >*/
	    iso_(iaer_prof__, taer, &tamol, piza, taerp, &tamolp, &c_b3, nt, 
		    mu, &rm[rm_offset], &gb[gb_offset], xtrans);
/*<         ddirtt=exp(-taer/xmus) >*/
	    *ddirtt = exp(-(*taer) / *xmus);
/*<         ddiftt=xtrans(1)-exp(-taer/xmus) >*/
	    *ddiftt = xtrans[2] - exp(-(*taer) / *xmus);
/*<         sphalbt=xtrans(0)*2. >*/
	    *sphalbt = xtrans[1] * 2.;
/*<         if (palt.le.0.) then >*/
	    if (*palt <= 0.) {
/*<           udiftt=0. >*/
		*udiftt = 0.;
/*<           udirtt=1. >*/
		*udirtt = 1.;
/*<         endif >*/
	    }
/*<       endif >*/
	}
/*<       if (it.eq.3) then >*/
	if (it == 3) {
/*<         rm(-mu)=-xmuv >*/
	    rm[-(*mu)] = -(*xmuv);
/*<         rm(mu)=xmuv >*/
	    rm[*mu] = *xmuv;
/*<         rm(0)=xmus >*/
	    rm[0] = *xmus;
/*<    >*/
	    iso_(iaer_prof__, taer, tray, piza, taerp, trayp, palt, nt, mu, &
		    rm[rm_offset], &gb[gb_offset], xtrans);
/*<         udirtt=exp(-(taerp+trayp)/xmuv) >*/
	    *udirtt = exp(-(*taerp + *trayp) / *xmuv);
/*<         udiftt=xtrans(-1)-exp(-(taerp+trayp)/xmuv) >*/
	    *udiftt = xtrans[0] - exp(-(*taerp + *trayp) / *xmuv);
/*<         rm(-mu)=-xmus >*/
	    rm[-(*mu)] = -(*xmus);
/*<         rm(mu)=xmus >*/
	    rm[*mu] = *xmus;
/*<         rm(0)=xmus >*/
	    rm[0] = *xmus;
/*<    >*/
	    iso_(iaer_prof__, taer, tray, piza, taerp, trayp, &c_b3, nt, mu, &
		    rm[rm_offset], &gb[gb_offset], xtrans);
/*<         ddiftt=xtrans(1)-exp(-(taer+tray)/xmus) >*/
	    *ddiftt = xtrans[2] - exp(-(*taer + *tray) / *xmus);
/*<         ddirtt=exp(-(taer+tray)/xmus) >*/
	    *ddirtt = exp(-(*taer + *tray) / *xmus);
/*<         sphalbt=xtrans(0)*2. >*/
	    *sphalbt = xtrans[1] * 2.;
/*<         if (palt.le.0.) then >*/
	    if (*palt <= 0.) {
/*<           udiftt=0. >*/
		*udiftt = 0.;
/*<           udirtt=1. >*/
		*udirtt = 1.;
/*<         endif >*/
	    }
/*<       endif >*/
	}
/*     write(6,*) ddirtt,ddiftt,it,tray,taer,trayp,taerp */
/*<       if (it.eq.2) goto 2 >*/
	if (it == 2) {
	    goto L2;
	}
/*<       if (it.eq.3) goto 1 >*/
	if (it == 3) {
	    goto L1;
	}
/*<       ddirtr=ddirtt >*/
	*ddirtr = *ddirtt;
/*<       ddiftr=ddiftt >*/
	*ddiftr = *ddiftt;
/*<       udirtr=udirtt >*/
	*udirtr = *udirtt;
/*<       udiftr=udiftt >*/
	*udiftr = *udiftt;
/*<       sphalbr=sphalbt >*/
	*sphalbr = *sphalbt;
/*<       goto 1 >*/
	goto L1;
/*<     2 ddirta=ddirtt >*/
L2:
	*ddirta = *ddirtt;
/*<       ddifta=ddiftt >*/
	*ddifta = *ddiftt;
/*<       udirta=udirtt >*/
	*udirta = *udirtt;
/*<       udifta=udiftt >*/
	*udifta = *udiftt;
/*<       sphalba=sphalbt >*/
	*sphalba = *sphalbt;
/*<     1 continue >*/
L1:
	;
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* scatra_ */

#ifdef __cplusplus
	}
#endif
