/* OCEABRDF.f -- translated by f2c (version 19970805).
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

static doublereal c_b2 = 3.52;
static integer c__24 = 24;
static integer c__48 = 48;
static doublereal c_b6 = 0.;

/*<    >*/
/* Subroutine */ int oceabrdf_(doublereal *pws, doublereal *paw, doublereal *
	xsal, doublereal *pcl, doublereal *pwl, doublereal *rfoam, doublereal 
	*rwat, doublereal *rglit, integer *mu, integer *np, doublereal *rm, 
	doublereal *rp, doublereal *brdfint)
{
    /* Initialized data */

    static doublereal ref[39] = { .22,.22,.22,.22,.22,.22,.215,.21,.2,.19,
	    .175,.155,.13,.08,.1,.105,.1,.08,.045,.055,.065,.06,.055,.04,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0. };

    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double atan(doublereal), acos(doublereal), sqrt(doublereal), pow_dd(
	    doublereal *, doublereal *), cos(doublereal), sin(doublereal), 
	    asin(doublereal);

    /* Local variables */
    extern /* Subroutine */ int sunglint_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal a, c__;
    integer j, k, m, n;
    doublereal w, fa[48], fi, n12, ta[24], ni, pi, nr, wl, tp, rw, tw;
    extern /* Subroutine */ int morcasiwat_(doublereal *, doublereal *, 
	    doublereal *);
    doublereal fac, wfa[48], fip, rog, tds;
    integer iwl;
    doublereal wta[24], rwc, tdv, rwb, azw, wlp, phi1, phi2, pond, rogp, wspd,
	     summ, teta1, teta2, ref_i__, tetas;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    doublereal tetav;
    extern /* Subroutine */ int indwat_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);


/* INPUT:  pws=wind speed (in m/s) */
/*         paw= azim. of sun - azim. of wind (in deg.) */
/* 	  xsal=salinity (in ppt) */
/* 	  pcl=pigment concentration (in mg.m-3) */
/*         pwl=wavelength of the computation (in micrometer) */
/*         mu=number of zenith angle */
/*         np=number of azimuth */
/*        rm=cosine of Gauss's angles for angles between -PI/2 and PI/2 de
g*/
/*         rp=Gauss's angles for angles between 0 and 2*PI */
/* OUTPUT: brdfint(j,k)=the total reflectance of the sea water */
/*        rfoam= the effective reflectance of the foam (foam coverage x fo
am reflectance)*/
/* 	  rwat= the reflectance of the water (just above the surface) */
/*         rglit= the reflectance of the sunglint */

/*<       parameter (nta=24,nfa=48) >*/
/*<       integer np,mu,k,j,m,n,iwl >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real Ref(39) >*/
/*<       real pwl,paw,pcl,pws,wl,wspd,C,azw,xsal >*/
/*<       real rfoam,rwat,rglit >*/
/*<       real pi,fac,nr,ni,n12 >*/
/*<       real tetas,w,wlp,ref_i,rwc,rw,tds,summ,tp,fip >*/
/*<       real rogp,pond,tetav,tw,tdv,fi,rog,a,rwb >*/
/* effective reflectance of the whitecaps (Koepke, 1984) */
/*<    >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
/* conversion of parameter */
/*<       C=pcl >*/
    c__ = *pcl;
/*<       wspd=pws >*/
    wspd = *pws;
/*<       azw=paw >*/
    azw = *paw;
/*<       wl=pwl >*/
    wl = *pwl;
/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       fac=pi/180. >*/
    fac = pi / 180.;
/*<       tetas=acos(rm(0))/fac >*/
    tetas = acos(rm[0]) / fac;
/* COMPUTE INDEX OF WATER */
/*<       call indwat(wl,xsal,nr,ni) >*/
    indwat_(&wl, xsal, &nr, &ni);
/*      write(6,'(1A,1X,2(F10.5,1X))') "nr,ni",nr,ni */
/*<       n12=sqrt(nr*nr+ni*ni) >*/
    n12 = sqrt(nr * nr + ni * ni);
/* COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN) */
/*<       W=2.95e-06*(wspd**3.52) >*/
    w = pow_dd(&wspd, &c_b2) * 2.95e-6;
/*<       iwl=1+int((wl-0.2)/0.1) >*/
    iwl = (integer) ((wl - .2) / .1) + 1;
/*<       wlp=0.5+(iwl-1)*0.1 >*/
    wlp = (iwl - 1) * .1 + .5;
/*<       Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1)) >*/
    ref_i__ = ref[iwl] + (wl - wlp) / .1 * (ref[iwl - 1] - ref[iwl]);
/*<       Rwc=W*Ref_i >*/
    rwc = w * ref_i__;
/* COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN) */
/*  water reflectance below the sea surface */
/*<       call MORCASIWAT(wl,C,Rw) >*/
    morcasiwat_(&wl, &c__, &rw);
/* call gauss quadrature */
/*<       tds=1.0 >*/
    tds = 1.;
/*<       tdv=1.0 >*/
    tdv = 1.;
/*<       if (Rw.gt.0.0001) then >*/
    if (rw > 1e-4) {
/*<         teta1=0. >*/
	teta1 = 0.;
/*<         teta2=pi/2. >*/
	teta2 = pi / 2.;
/*<         call gauss(teta1,teta2,ta,wta,nta) >*/
	gauss_(&teta1, &teta2, ta, wta, &c__24);
/*<         phi1=0. >*/
	phi1 = 0.;
/*<         phi2=2.*pi >*/
	phi2 = pi * 2.;
/*<         call gauss(phi1,phi2,fa,wfa,nfa) >*/
	gauss_(&phi1, &phi2, fa, wfa, &c__48);
/* COMPUTE DOWNWARD TRANSMISSION FUNCTION */
/*<         tds=0. >*/
	tds = 0.;
/*<         summ=0. >*/
	summ = 0.;
/*<         do k=1,nfa >*/
	for (k = 1; k <= 48; ++k) {
/*<         do j=1,nta >*/
	    for (j = 1; j <= 24; ++j) {
/*<           tp=ta(j)/fac >*/
		tp = ta[j - 1] / fac;
/*<           fip=fa(k)/fac >*/
		fip = fa[k - 1] / fac;
/*<           call sunglint(wspd,nr,ni,azw,tetas,tp,fip,rogp) >*/
		sunglint_(&wspd, &nr, &ni, &azw, &tetas, &tp, &fip, &rogp);
/*<           pond=cos(ta(j))*sin(ta(j))*wfa(k)*wta(j) >*/
		pond = cos(ta[j - 1]) * sin(ta[j - 1]) * wfa[k - 1] * wta[j - 
			1];
/*<           summ=summ+pond >*/
		summ += pond;
/*<           tds=tds+rogp*pond >*/
		tds += rogp * pond;
/*<         enddo >*/
	    }
/*<         enddo >*/
	}
/*<         tds=1.-tds/summ >*/
	tds = 1. - tds / summ;
/*<       endif >*/
    }
/* Compute glint contribution,transmission terms and total reflectance */
/*<       Do 1 j=1,mu >*/
    i__1 = *mu;
    for (j = 1; j <= i__1; ++j) {
/*<         tetav=acos(rm(j))/fac >*/
	tetav = acos(rm[j]) / fac;
/* compute upward transmission */
/*<         if (Rw.gt.0.0001) then >*/
	if (rw > 1e-4) {
/*<           tw=asin(sin(tetav*fac)/nr)/fac >*/
	    tw = asin(sin(tetav * fac) / nr) / fac;
/*<           tdv=0. >*/
	    tdv = 0.;
/*<           summ=0. >*/
	    summ = 0.;
/*<           do n=1,nfa >*/
	    for (n = 1; n <= 48; ++n) {
/*<           do m=1,nta >*/
		for (m = 1; m <= 24; ++m) {
/*<             tp=ta(m)/fac >*/
		    tp = ta[m - 1] / fac;
/*<             fip=fa(n)/fac >*/
		    fip = fa[n - 1] / fac;
/* here we assume no aborption */
/*<             call sunglint(wspd,1./nr,0.0,azw,tw,tp,fip,rogp) >*/
		    d__1 = 1. / nr;
		    sunglint_(&wspd, &d__1, &c_b6, &azw, &tw, &tp, &fip, &
			    rogp);
/*<             pond=cos(ta(m))*sin(ta(m))*wfa(n)*wta(m) >*/
		    pond = cos(ta[m - 1]) * sin(ta[m - 1]) * wfa[n - 1] * wta[
			    m - 1];
/*<             summ=summ+pond >*/
		    summ += pond;
/*<             tdv=tdv+rogp*pond >*/
		    tdv += rogp * pond;
/*<           enddo >*/
		}
/*<           enddo >*/
	    }
/*<           tdv=1.-tdv/summ >*/
	    tdv = 1. - tdv / summ;
/*<         endif >*/
	}
/*<         Do 2 k=1,np >*/
	i__2 = *np;
	for (k = 1; k <= i__2; ++k) {
/*<            if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<              fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<            else >*/
	    } else {
/*<              fi=(rp(k)+rm(-mu)) >*/
		fi = rp[k] + rm[-(*mu)];
/*<            endif >*/
	    }
/*<            if (fi.lt.0.) fi=fi+2.*pi >*/
	    if (fi < 0.) {
		fi += pi * 2.;
	    }
/*<            if (fi.gt.(2.*pi)) fi=fi-2.*pi >*/
	    if (fi > pi * 2.) {
		fi -= pi * 2.;
	    }
/*<            fi=fi/fac >*/
	    fi /= fac;
/* SUNGLINT REFLECTANCE */
/*<            call sunglint(wspd,nr,ni,azw,tetas,tetav,fi,rog) >*/
	    sunglint_(&wspd, &nr, &ni, &azw, &tetas, &tetav, &fi, &rog);
/*  water reflectance above the sea surface */
/* for explanation on value of a see OCEAALBE.f */
/*<            a=0.485 >*/
	    a = .485;
/* add change in solid angle from under to above to surface */
/* that account for 1/(n12*n12) decrease in sea water directional 
*/
/* reflectance */
/*<            Rwb=(1/(n12*n12))*tds*tdv*Rw/(1-a*Rw) >*/
	    rwb = 1 / (n12 * n12) * tds * tdv * rw / (1 - a * rw);
/* TOTAL REFLECTANCE OF SEA WATER */
/*<            brdfint(j,k)=Rwc+(1-W)*Rog+(1-Rwc)*Rwb >*/
	    brdfint[j + k * brdfint_dim1] = rwc + (1 - w) * rog + (1 - rwc) * 
		    rwb;
/*< 	   rfoam=rwc >*/
	    *rfoam = rwc;
/*< 	   rwat=(1-Rwc)*Rwb >*/
	    *rwat = (1 - rwc) * rwb;
/*< 	   rglit=(1-W)*Rog >*/
	    *rglit = (1 - w) * rog;
/*<  2      continue >*/
/* L2: */
	}
/*<  1    continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* oceabrdf_ */

#ifdef __cplusplus
	}
#endif
