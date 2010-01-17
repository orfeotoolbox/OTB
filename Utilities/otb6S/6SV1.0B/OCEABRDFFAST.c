/* OCEABRDFFAST.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<    >*/
/* Subroutine */ int oceabrdffast_(doublereal *pws, doublereal *paw, 
	doublereal *xsal, doublereal *pcl, doublereal *pwl, integer *mu, 
	integer *np, doublereal *rm, doublereal *rp, doublereal *brdfint)
{

    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;

    /* Builtin functions */
    double atan(doublereal), acos(doublereal), sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int sunglint_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal a, c__;
    integer j, k;
    doublereal fi, n12, ni, pi, nr, wl, fac, rog, azw, wspd, tetas, tetav;
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

/*<       parameter (nta=24,nfa=48) >*/
/*<       integer np,mu,k,j,m,n,iwl >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real Ref(39) >*/
/*<       real pwl,paw,pcl,pws,wl,wspd,C,azw,xsal >*/
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
/*<       n12=sqrt(nr*nr+ni*ni) >*/
    n12 = sqrt(nr * nr + ni * ni);
/* COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN) */
/*      W=2.95e-06*(wspd**3.52) */
/*      iwl=1+int((wl-0.2)/0.1) */
/*      wlp=0.5+(iwl-1)*0.1 */
/*      Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1)) */
/*      Rwc=W*Ref_i */
/* COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN) */
/*  water reflectance below the sea surface */
/*      call MORCASIWAT(wl,C,Rw) */
/* call gauss quadrature */
/*      tds=1.0 */
/*      tdv=1.0 */
/*      if (Rw.gt.0.0001) then */
/*        teta1=0. */
/*        teta2=pi/2. */
/*        call gauss(teta1,teta2,ta,wta,nta) */
/*        phi1=0. */
/*        phi2=2.*pi */
/*        call gauss(phi1,phi2,fa,wfa,nfa) */
/* COMPUTE DOWNWARD TRANSMISSION FUNCTION */
/*        tds=0. */
/*        summ=0. */
/*        do k=1,nfa */
/*        do j=1,nta */
/*          tp=ta(j)/fac */
/*          fip=fa(k)/fac */
/*          call sunglint(wspd,nr,ni,azw,tetas,tp,fip,rogp) */
/*          pond=cos(ta(j))*sin(ta(j))*wfa(k)*wta(j) */
/*          summ=summ+pond */
/*          tds=tds+rogp*pond */
/*        enddo */
/*        enddo */
/*        tds=1.-tds/summ */
/*      endif */
/* Compute glint contribution,transmission terms and total reflectance */
/*<       Do 1 j=1,mu >*/
    i__1 = *mu;
    for (j = 1; j <= i__1; ++j) {
/*<         tetav=acos(rm(j))/fac >*/
	tetav = acos(rm[j]) / fac;
/* compute upward transmission */
/*        if (Rw.gt.0.0001) then */
/*          tw=asin(sin(tetav*fac)/nr)/fac */
/*          tdv=0. */
/*          summ=0. */
/*          do n=1,nfa */
/*          do m=1,nta */
/*            tp=ta(m)/fac */
/*            fip=fa(n)/fac */
/* here we assume no aborption */
/*            call sunglint(wspd,1./nr,0.0,azw,tw,tp,fip,rogp) */
/*            pond=cos(ta(m))*sin(ta(m))*wfa(n)*wta(m) */
/*            summ=summ+pond */
/*            tdv=tdv+rogp*pond */
/*          enddo */
/*          enddo */
/*          tdv=1.-tdv/summ */
/*        endif */
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
/*           Rwb=(1/(n12*n12))*tds*tdv*Rw/(1-a*Rw) */
/* TOTAL REFLECTANCE OF SEA WATER */
/*<            brdfint(j,k)=Rog >*/
	    brdfint[j + k * brdfint_dim1] = rog;
/*<  2      continue >*/
/* L2: */
	}
/*<  1    continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* oceabrdffast_ */

#ifdef __cplusplus
	}
#endif
