/* INTERP.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/* Common Block Declarations */

Extern struct {
    real ext[20], ome[20], gasym[20], phase[20], qhase[20], uhase[20];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

Extern struct {
    real roatm[60]	/* was [3][20] */, dtdir[60]	/* was [3][20] */, 
	    dtdif[60]	/* was [3][20] */, utdir[60]	/* was [3][20] */, 
	    utdif[60]	/* was [3][20] */, sphal[60]	/* was [3][20] */, 
	    wldis[20], trayl[20], traypl[20], rqatm[60]	/* was [3][20] */, 
	    ruatm[60]	/* was [3][20] */;
} sixs_disc__;

#define sixs_disc__1 sixs_disc__

Extern struct {
    real delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

/*<    >*/
/* Subroutine */ int interp_(integer *iaer, integer *idatmp, real *wl, real *
	taer55, real *taer55p, real *xmud, real *romix, real *rorayl, real *
	roaero, real *phaa, real *phar, real *rqmix, real *rqrayl, real *
	rqaero, real *qhaa, real *qhar, real *rumix, real *rurayl, real *
	ruaero, real *uhaa, real *uhar, real *tsca, real *tray, real *trayp, 
	real *taer, real *taerp, real *dtott, real *utott, real *astot, real *
	asray, real *asaer, real *utotr, real *utota, real *dtotr, real *
	dtota, integer *ipol, real *roatm_fi__, real *romix_fi__, real *
	rorayl_fi__, integer *nfi, real *roluts, real *rolut, real *rolutsq, 
	real *rolutq, real *rolutsu, real *rolutu, integer *nfilut)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double log(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    integer i__, j, ll, mu, ifi;
    real coef;
    integer linf, lsup;
    real test1, test2, test3, betaa, betac, aainf, dainf, coefl, betar, arinf,
	     drinf, dtinf, uainf, atinf, dtotc, wlinf, dasup, urinf, utinf, 
	    drsup, dtsup, utotc, uasup, arsup, atsup, aasup, ursup, utsup, 
	    ruaer0, alphaa, alphac, alphar, depolar1, depolar2;

/*<       include "paramdef.inc" >*/
/*<       real test1,test2,test3 >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       Real wl,taer55,taer55p >*/
/*<       Real xmud,romix,rorayl,roaero,phaa,phar,tsca,tray >*/
/*<       Real rqmix,rqrayl,rqaero,qhaa,qhar,rqatm,qhase >*/
/*<       Real rumix,rurayl,ruaero,uhaa,uhar,ruatm,uhase >*/
/*<       Real trayp,taer,taerp,dtott,utott,astot,asray,asaer,utotr >*/
/*<       Real utota,dtotr,dtota,ext,ome,gasym,phase,roatm,dtdir >*/
/*<       Real dtdif,utdir,utdif,sphal,wldis,trayl,traypl,delta,sigma >*/
/*<       Real alphaa,betaa,alphar,betar,alphac,betac,coef,coefl,wlinf >*/
/*<       Real drinf,drsup,dtinf,dtsup,dtotc,dainf,dasup,urinf,ursup >*/
/*<       Real utinf,utsup,utotc,uainf,uasup,arinf,arsup,atinf,atsup >*/
/*<       Real aainf,aasup,depolar1,depolar2 >*/
/*<       real romix_fi(nfi),rorayl_fi(nfi),roatm_fi(3,20,nfi) >*/
/*<       real rolut(mu_p,41),roluts(20,mu_p,41) >*/
/*<       real rolutq(mu_p,41),rolutsq(20,mu_p,41) >*/
/*<       real rolutu(mu_p,41),rolutsu(20,mu_p,41) >*/
/*<       integer nfilut(mu_p),mu >*/
/*<       Integer iaer,idatmp,linf,ll,lsup,ipol >*/
/*<    >*/
/*<    >*/
/*<       common /sixs_del/ delta,sigma >*/
/*<       mu=mu_p >*/
    /* Parameter adjustments */
    --rorayl_fi__;
    --romix_fi__;
    roatm_fi__ -= 64;
    roluts -= 521;
    rolut -= 26;
    rolutsq -= 521;
    rolutq -= 26;
    rolutsu -= 521;
    rolutu -= 26;
    --nfilut;

    /* Function Body */
    mu = 25;
/*     that for the atmosphere (if polarization requested) : */
/*     the reflectances */
/*                    rayleigh                   = rorayl ,(rqrayl,rurayl)
*/
/*                    aerosols                   = roaero ,(rqaero,ruaero)
*/
/*                     mixing                     = romix ,(rqmix,rumix) 
*/
/*     the downward transmittances */
/*                     rayleigh                   = dtotr */
/*                     aerosols                   = dtota */
/*                     total                      = dtott */
/*     the upward transmittances */
/*                     rayleigh                   = utotr */
/*                     aerosols                   = utota */
/*                     total                      = utott */
/*     the spherical albedos */
/*                     rayleigh                   = asray */
/*                     aerosols                   = asaer */
/*                     total                      = astot */
/*     the optical thickness of total atmosphere */
/*                     rayleigh                   = tray */
/*                     aerosols                   = taer */
/*     the optical thickness of the atmosphere above the plane */
/*                     rayleigh                   = trayp */
/*                     aerosols                   = taerp */
/*     the tsca of the aerosols */
/*                     total atmosphere           = tsca */
/*<       linf=1 >*/
    linf = 1;
/*<       do 81 ll=1,19 >*/
    for (ll = 1; ll <= 19; ++ll) {
/*<         if(wl.gt.wldis(ll).and.wl.le.wldis(ll+1)) linf=ll >*/
	if (*wl > sixs_disc__1.wldis[ll - 1] && *wl <= sixs_disc__1.wldis[ll])
		 {
	    linf = ll;
	}
/*<    81 continue >*/
/* L81: */
    }
/*<       if(wl.gt.wldis(20)) linf=19 >*/
    if (*wl > sixs_disc__1.wldis[19]) {
	linf = 19;
    }
/*<       lsup=linf+1 >*/
    lsup = linf + 1;
/*     interpolation in function of wavelength for scattering */
/*     atmospheric functions from discrete values at wldis */
/*<       alphaa=0. >*/
    alphaa = (float)0.;
/*<       betaa=0. >*/
    betaa = (float)0.;
/*<       alphar=0. >*/
    alphar = (float)0.;
/*<       betar=0. >*/
    betar = (float)0.;
/*<       alphac=0. >*/
    alphac = (float)0.;
/*<       betac=0. >*/
    betac = (float)0.;
/*<       phaa=0. >*/
    *phaa = (float)0.;
/*<       qhaa=0. >*/
    *qhaa = (float)0.;
/*<       uhaa=0. >*/
    *uhaa = (float)0.;
/*<       phar=0. >*/
    *phar = (float)0.;
/*<       qhar=0. >*/
    *qhar = (float)0.;
/*<       uhar=0. >*/
    *uhar = (float)0.;
/*<       roaero=0. >*/
    *roaero = (float)0.;
/*<       rqaero=0. >*/
    *rqaero = (float)0.;
/*<       ruaero=0. >*/
    *ruaero = (float)0.;
/*<       rorayl=0. >*/
    *rorayl = (float)0.;
/*<       rqrayl=0. >*/
    *rqrayl = (float)0.;
/*<       rurayl=0. >*/
    *rurayl = (float)0.;
/*<       romix=0. >*/
    *romix = (float)0.;
/*<       rqmix=0. >*/
    *rqmix = (float)0.;
/*<       rumix=0. >*/
    *rumix = (float)0.;
/*<       dtota=1. >*/
    *dtota = (float)1.;
/*<       utota=1. >*/
    *utota = (float)1.;
/*<       asaer=0. >*/
    *asaer = (float)0.;
/*<       taer=0. >*/
    *taer = (float)0.;
/*<       taerp=0. >*/
    *taerp = (float)0.;
/*<       coef=alog(wldis(lsup)/wldis(linf)) >*/
    coef = log(sixs_disc__1.wldis[lsup - 1] / sixs_disc__1.wldis[linf - 1]);
/*<       wlinf=wldis(linf) >*/
    wlinf = sixs_disc__1.wldis[linf - 1];
/* Here we spectrally interpolate phase functions and Stokes' */
/* parameters I, Q and U (all divided by cos(teta_sun)), both */
/* for Rayleigh, Aerosol, and a mixture Rayleigh+aerosol */
/* For the Rayleigh Phase functions we need to take into */
/* account the depolarization factor delta. */
/*<       depolar1=2.*(1.-delta)/(2.+delta) >*/
    depolar1 = ((float)1. - sixs_del__1.delta) * (float)2. / (
	    sixs_del__1.delta + (float)2.);
/*<       depolar2=3.*delta/(2.+delta) >*/
    depolar2 = sixs_del__1.delta * (float)3. / (sixs_del__1.delta + (float)2.)
	    ;

/* we first start with the parameter I (with truncation) */
/*    (divided by cos(teta_sun)) */
/* if requested, we continue with the parameter I (without truncation)... 
*/
/*    (divided by cos(teta_sun)) */
/*<           if(iaer.eq.0) goto 2240 >*/
    if (*iaer == 0) {
	goto L2240;
    }
/*<           alphaa=alog(phase(lsup)/phase(linf))/coef >*/
    alphaa = log(sixs_aer__1.phase[lsup - 1] / sixs_aer__1.phase[linf - 1]) / 
	    coef;
/*<           betaa=phase(linf)/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = sixs_aer__1.phase[linf - 1] / pow_dd(&d__1, &d__2);
/*<           phaa=betaa*(wl**alphaa) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *phaa = betaa * pow_dd(&d__1, &d__2);
/*<  2240     continue >*/
L2240:
/*<           phar=depolar1*.75*(1.+xmud*xmud)+depolar2 >*/
    *phar = depolar1 * (float).75 * (*xmud * *xmud + (float)1.) + depolar2;
/*<           if (idatmp.eq.0) goto 2234 >*/
    if (*idatmp == 0) {
	goto L2234;
    }
/*          if((roatm(1,linf).le..001).or.(roatm(1,lsup).le..001)) then */
/*            rorayl=roatm(1,linf)+(roatm(1,lsup)-roatm(1,linf)) */
/*     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) */
/*          else */
/*<              alphar=alog(roatm(1,lsup)/roatm(1,linf))/ coef >*/
    alphar = log(sixs_disc__1.roatm[lsup * 3 - 3] / sixs_disc__1.roatm[linf * 
	    3 - 3]) / coef;
/*<              betar=roatm(1,linf)/(wlinf**(alphar)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphar;
    betar = sixs_disc__1.roatm[linf * 3 - 3] / pow_dd(&d__1, &d__2);
/*<  	    rorayl=betar*(wl**alphar) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphar;
    *rorayl = betar * pow_dd(&d__1, &d__2);
/*< 	    do ifi=1,nfi >*/
    i__1 = *nfi;
    for (ifi = 1; ifi <= i__1; ++ifi) {
/*< 	    alphar=alog(roatm_fi(1,lsup,ifi)/roatm_fi(1,linf,ifi))/ coef >*/
	alphar = log(roatm_fi__[(lsup + ifi * 20) * 3 + 1] / roatm_fi__[(linf 
		+ ifi * 20) * 3 + 1]) / coef;
/*<             betar=roatm_fi(1,linf,ifi)/(wlinf**(alphar)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphar;
	betar = roatm_fi__[(linf + ifi * 20) * 3 + 1] / pow_dd(&d__1, &d__2);
/*< 	    rorayl_fi(ifi)=betar*(wl**alphar) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphar;
	rorayl_fi__[ifi] = betar * pow_dd(&d__1, &d__2);
/*<             enddo >*/
    }
/*          endif */
/*         write(6,*)'Ib ',phar,rorayl,betar,alphar,roatm(1,lsup), */
/*    s            roatm(1,linf) */
/*         if((roatm(2,linf).le..00000001).or.(roatm(2,lsup).le..00000001)
) then*/
/*            romix=roatm(2,linf)+(roatm(2,lsup)-roatm(2,linf)) */
/*     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) */
/*            xtmp=(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) */
/*            do ifi=1,nfi */
/* 	    romix_fi(ifi)=roatm_fi(2,linf,ifi)+ */
/*     s      (roatm_fi(2,lsup,ifi)-roatm_fi(2,linf,ifi))*xtmp */
/*            enddo */
/*          else */
/*<             alphac=alog(roatm(2,lsup)/roatm(2,linf))/coef >*/
    alphac = log(sixs_disc__1.roatm[lsup * 3 - 2] / sixs_disc__1.roatm[linf * 
	    3 - 2]) / coef;
/*<             betac=roatm(2,linf)/(wlinf**(alphac)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphac;
    betac = sixs_disc__1.roatm[linf * 3 - 2] / pow_dd(&d__1, &d__2);
/*< 	    romix=betac*(wl**alphac) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphac;
    *romix = betac * pow_dd(&d__1, &d__2);
/*<      	    do ifi=1,nfi >*/
    i__1 = *nfi;
    for (ifi = 1; ifi <= i__1; ++ifi) {
/*<             alphac=alog(roatm_fi(2,lsup,ifi)/roatm_fi(2,linf,ifi))/coef >*/
	alphac = log(roatm_fi__[(lsup + ifi * 20) * 3 + 2] / roatm_fi__[(linf 
		+ ifi * 20) * 3 + 2]) / coef;
/*<             betac=roatm_fi(2,linf,ifi)/(wlinf**(alphac)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphac;
	betac = roatm_fi__[(linf + ifi * 20) * 3 + 2] / pow_dd(&d__1, &d__2);
/*< 	    romix_fi(ifi)=betac*(wl**alphac) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphac;
	romix_fi__[ifi] = betac * pow_dd(&d__1, &d__2);
/*< 	    enddo >*/
    }
/*          endif */
/*<           if(iaer.eq.0) goto 2234 >*/
    if (*iaer == 0) {
	goto L2234;
    }
/*          if((roatm(3,linf).lt..001).or.(roatm(3,lsup).le..001))then */
/*            roaero=roatm(3,linf)+(roatm(3,lsup)-roatm(3,linf)) */
/*     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) */
/*          else */
/*<             alphaa=alog(roatm(3,lsup)/roatm(3,linf))/coef >*/
    alphaa = log(sixs_disc__1.roatm[lsup * 3 - 1] / sixs_disc__1.roatm[linf * 
	    3 - 1]) / coef;
/*<             betaa=roatm(3,linf)/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = sixs_disc__1.roatm[linf * 3 - 1] / pow_dd(&d__1, &d__2);
/*<             roaero=betaa*(wl**alphaa) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *roaero = betaa * pow_dd(&d__1, &d__2);
/*          endif */
/* Look up table update */
/*<         coefl=(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) >*/
    coefl = (*wl - sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 
	    1] - sixs_disc__1.wldis[linf - 1]);
/*< 	do i=1,mu >*/
    i__1 = mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*< 	do j=1,nfilut(i) >*/
	i__2 = nfilut[i__];
	for (j = 1; j <= i__2; ++j) {
/* 	if ((roluts(lsup,i,j).gt.0.001).and.(roluts(linf,i,j).gt.0.001
)) then */
/*<            alphac=alog(roluts(lsup,i,j)/roluts(linf,i,j))/coef >*/
	    alphac = log(roluts[lsup + (i__ + j * 25) * 20] / roluts[linf + (
		    i__ + j * 25) * 20]) / coef;
/*<            betac=roluts(linf,i,j)/(wlinf**(alphac)) >*/
	    d__1 = (doublereal) wlinf;
	    d__2 = (doublereal) alphac;
	    betac = roluts[linf + (i__ + j * 25) * 20] / pow_dd(&d__1, &d__2);
/*< 	   rolut(i,j)=betac*(wl**alphac) >*/
	    d__1 = (doublereal) (*wl);
	    d__2 = (doublereal) alphac;
	    rolut[i__ + j * 25] = betac * pow_dd(&d__1, &d__2);
/* 	   else */
/* 	   rolut(i,j)=roluts(linf,i,j) */
/*     &      +(roluts(lsup,i,j)-roluts(linf,i,j))*coefl */
/* 	endif */
/*< 	if ((rolutsq(lsup,i,j).gt.0.001).and.(rolutsq(linf,i,j).gt.0.001)) then >*/
	    if (rolutsq[lsup + (i__ + j * 25) * 20] > (float).001 && rolutsq[
		    linf + (i__ + j * 25) * 20] > (float).001) {
/*<            alphac=alog(rolutsq(lsup,i,j)/rolutsq(linf,i,j))/coef >*/
		alphac = log(rolutsq[lsup + (i__ + j * 25) * 20] / rolutsq[
			linf + (i__ + j * 25) * 20]) / coef;
/*<            betac=rolutsq(linf,i,j)/(wlinf**(alphac)) >*/
		d__1 = (doublereal) wlinf;
		d__2 = (doublereal) alphac;
		betac = rolutsq[linf + (i__ + j * 25) * 20] / pow_dd(&d__1, &
			d__2);
/*< 	   rolutq(i,j)=betac*(wl**alphac) >*/
		d__1 = (doublereal) (*wl);
		d__2 = (doublereal) alphac;
		rolutq[i__ + j * 25] = betac * pow_dd(&d__1, &d__2);
/*< 	   else >*/
	    } else {
/*< 	  >*/
		rolutq[i__ + j * 25] = rolutsq[linf + (i__ + j * 25) * 20] + (
			rolutsq[lsup + (i__ + j * 25) * 20] - rolutsq[linf + (
			i__ + j * 25) * 20]) * coefl;
/*< 	endif >*/
	    }
/*< 	if ((rolutsu(lsup,i,j).gt.0.001).and.(rolutsu(linf,i,j).gt.0.001)) then >*/
	    if (rolutsu[lsup + (i__ + j * 25) * 20] > (float).001 && rolutsu[
		    linf + (i__ + j * 25) * 20] > (float).001) {
/*<            alphac=alog(rolutsu(lsup,i,j)/rolutsu(linf,i,j))/coef >*/
		alphac = log(rolutsu[lsup + (i__ + j * 25) * 20] / rolutsu[
			linf + (i__ + j * 25) * 20]) / coef;
/*<            betac=rolutsu(linf,i,j)/(wlinf**(alphac)) >*/
		d__1 = (doublereal) wlinf;
		d__2 = (doublereal) alphac;
		betac = rolutsu[linf + (i__ + j * 25) * 20] / pow_dd(&d__1, &
			d__2);
/*< 	   rolutu(i,j)=betac*(wl**alphac) >*/
		d__1 = (doublereal) (*wl);
		d__2 = (doublereal) alphac;
		rolutu[i__ + j * 25] = betac * pow_dd(&d__1, &d__2);
/*< 	   else >*/
	    } else {
/*< 	  >*/
		rolutu[i__ + j * 25] = rolutsu[linf + (i__ + j * 25) * 20] + (
			rolutsu[lsup + (i__ + j * 25) * 20] - rolutsu[linf + (
			i__ + j * 25) * 20]) * coefl;
/*< 	endif >*/
	    }
/*< 	enddo >*/
	}
/*< 	enddo >*/
    }
/* End Look up table update */
/*<  2234     continue >*/
L2234:
/* we continue with the parameter Q .... */
/*<         if(iaer.eq.0) goto 3240 >*/
    if (*iaer == 0) {
	goto L3240;
    }
/*< 	if ((qhase(lsup).gt.0.001).and.(qhase(linf).gt.0.001)) then >*/
    if (sixs_aer__1.qhase[lsup - 1] > (float).001 && sixs_aer__1.qhase[linf - 
	    1] > (float).001) {
/*<         alphaa=alog(qhase(lsup)/qhase(linf))/coef >*/
	alphaa = log(sixs_aer__1.qhase[lsup - 1] / sixs_aer__1.qhase[linf - 1]
		) / coef;
/*<         betaa=qhase(linf)/(wlinf**(alphaa)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphaa;
	betaa = sixs_aer__1.qhase[linf - 1] / pow_dd(&d__1, &d__2);
/*<         qhaa=betaa*(wl**alphaa) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphaa;
	*qhaa = betaa * pow_dd(&d__1, &d__2);
/*< 	else >*/
    } else {
/*< 	qhaa=qhase(linf)+(qhase(lsup)-qhase(linf))*coefl >*/
	*qhaa = sixs_aer__1.qhase[linf - 1] + (sixs_aer__1.qhase[lsup - 1] - 
		sixs_aer__1.qhase[linf - 1]) * coefl;
/*< 	endif >*/
    }
/*<  3240   continue >*/
L3240:
/*<         qhar=depolar1*.75*(xmud*xmud-1.) >*/
    *qhar = depolar1 * (float).75 * (*xmud * *xmud - (float)1.);
/*<         if (idatmp.eq.0) goto 3234 >*/
    if (*idatmp == 0) {
	goto L3234;
    }
/*< 	test1=abs(rqatm(1,linf)) >*/
    test1 = (r__1 = sixs_disc__1.rqatm[linf * 3 - 3], dabs(r__1));
/*< 	test2=abs(rqatm(1,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.rqatm[lsup * 3 - 3], dabs(r__1));
/*< 	test3=rqatm(1,lsup)*rqatm(1,linf) >*/
    test3 = sixs_disc__1.rqatm[lsup * 3 - 3] * sixs_disc__1.rqatm[linf * 3 - 
	    3];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	*rqrayl = sixs_disc__1.rqatm[linf * 3 - 3] + (sixs_disc__1.rqatm[lsup 
		* 3 - 3] - sixs_disc__1.rqatm[linf * 3 - 3]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*<         else >*/
    } else {
/*<           alphar=alog(rqatm(1,lsup)/rqatm(1,linf))/ coef >*/
	alphar = log(sixs_disc__1.rqatm[lsup * 3 - 3] / sixs_disc__1.rqatm[
		linf * 3 - 3]) / coef;
/*<           betar=rqatm(1,linf)/(wlinf**(alphar)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphar;
	betar = sixs_disc__1.rqatm[linf * 3 - 3] / pow_dd(&d__1, &d__2);
/*< 	  rqrayl=betar*(wl**alphar) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphar;
	*rqrayl = betar * pow_dd(&d__1, &d__2);
/*< 	endif >*/
    }
/*       write(6,*)'Q ',qhar,rqrayl,betar,alphar,rqatm(1,lsup), */
/*    s          rqatm(1,linf) */
/*< 	test1=abs(rqatm(2,linf)) >*/
    test1 = (r__1 = sixs_disc__1.rqatm[linf * 3 - 2], dabs(r__1));
/*< 	test2=abs(rqatm(2,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.rqatm[lsup * 3 - 2], dabs(r__1));
/*< 	test3=rqatm(2,lsup)*rqatm(2,linf) >*/
    test3 = sixs_disc__1.rqatm[lsup * 3 - 2] * sixs_disc__1.rqatm[linf * 3 - 
	    2];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	*rqmix = sixs_disc__1.rqatm[linf * 3 - 2] + (sixs_disc__1.rqatm[lsup *
		 3 - 2] - sixs_disc__1.rqatm[linf * 3 - 2]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*< 	else >*/
    } else {
/*<           alphac=alog(rqatm(2,lsup)/rqatm(2,linf))/coef >*/
	alphac = log(sixs_disc__1.rqatm[lsup * 3 - 2] / sixs_disc__1.rqatm[
		linf * 3 - 2]) / coef;
/*<           betac=rqatm(2,linf)/(wlinf**(alphac)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphac;
	betac = sixs_disc__1.rqatm[linf * 3 - 2] / pow_dd(&d__1, &d__2);
/*<           rqmix=betac*(wl**alphac) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphac;
	*rqmix = betac * pow_dd(&d__1, &d__2);
/*<         endif >*/
    }
/*<         if(iaer.eq.0) goto 3234 >*/
    if (*iaer == 0) {
	goto L3234;
    }
/*        write(6,*) "rqaero ",rqatm(3,linf),rqatm(3,lsup) */
/*< 	test1=abs(rqatm(3,linf)) >*/
    test1 = (r__1 = sixs_disc__1.rqatm[linf * 3 - 1], dabs(r__1));
/*< 	test2=abs(rqatm(3,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.rqatm[lsup * 3 - 1], dabs(r__1));
/*< 	test3=rqatm(3,lsup)*rqatm(3,linf) >*/
    test3 = sixs_disc__1.rqatm[lsup * 3 - 1] * sixs_disc__1.rqatm[linf * 3 - 
	    1];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	*rqaero = sixs_disc__1.rqatm[linf * 3 - 1] + (sixs_disc__1.rqatm[lsup 
		* 3 - 1] - sixs_disc__1.rqatm[linf * 3 - 1]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*< 	else >*/
    } else {
/*<           alphaa=alog(rqatm(3,lsup)/rqatm(3,linf))/coef >*/
	alphaa = log(sixs_disc__1.rqatm[lsup * 3 - 1] / sixs_disc__1.rqatm[
		linf * 3 - 1]) / coef;
/*<           betaa=rqatm(3,linf)/(wlinf**(alphaa)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphaa;
	betaa = sixs_disc__1.rqatm[linf * 3 - 1] / pow_dd(&d__1, &d__2);
/*<           rqaero=betaa*(wl**alphaa) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphaa;
	*rqaero = betaa * pow_dd(&d__1, &d__2);
/*< 	endif >*/
    }
/*        write(6,*) "rqaero ",rqaero */
/*<  3234  	continue >*/
L3234:
/* .... and we finish with the parameter U */
/*<         if(iaer.eq.0) goto 4242 >*/
    if (*iaer == 0) {
	goto L4242;
    }
/*< 	if ((uhase(lsup).gt.0.001).and.(uhase(linf).gt.0.001)) then >*/
    if (sixs_aer__1.uhase[lsup - 1] > (float).001 && sixs_aer__1.uhase[linf - 
	    1] > (float).001) {
/*<         alphaa=alog(uhase(lsup)/uhase(linf))/coef >*/
	alphaa = log(sixs_aer__1.uhase[lsup - 1] / sixs_aer__1.uhase[linf - 1]
		) / coef;
/*<         betaa=uhase(linf)/(wlinf**(alphaa)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphaa;
	betaa = sixs_aer__1.uhase[linf - 1] / pow_dd(&d__1, &d__2);
/*<         uhaa=betaa*(wl**alphaa) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphaa;
	*uhaa = betaa * pow_dd(&d__1, &d__2);
/*< 	else >*/
    } else {
/*< 	uhaa=uhase(linf)+(uhase(lsup)-uhase(linf))*coefl >*/
	*uhaa = sixs_aer__1.uhase[linf - 1] + (sixs_aer__1.uhase[lsup - 1] - 
		sixs_aer__1.uhase[linf - 1]) * coefl;
/*< 	endif >*/
    }
/*<  4242   continue >*/
L4242:
/*<         uhar=depolar1*3./2.*xmud >*/
    *uhar = depolar1 * (float)3. / (float)2. * *xmud;
/*<         if (idatmp.eq.0) goto 4234 >*/
    if (*idatmp == 0) {
	goto L4234;
    }
/*< 	test1=abs(ruatm(1,linf)) >*/
    test1 = (r__1 = sixs_disc__1.ruatm[linf * 3 - 3], dabs(r__1));
/*< 	test2=abs(ruatm(1,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.ruatm[lsup * 3 - 3], dabs(r__1));
/*< 	test3=ruatm(1,lsup)*ruatm(1,linf) >*/
    test3 = sixs_disc__1.ruatm[lsup * 3 - 3] * sixs_disc__1.ruatm[linf * 3 - 
	    3];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	*rurayl = sixs_disc__1.ruatm[linf * 3 - 3] + (sixs_disc__1.ruatm[lsup 
		* 3 - 3] - sixs_disc__1.ruatm[linf * 3 - 3]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*<         else >*/
    } else {
/*<           alphar=alog(ruatm(1,lsup)/ruatm(1,linf))/ coef >*/
	alphar = log(sixs_disc__1.ruatm[lsup * 3 - 3] / sixs_disc__1.ruatm[
		linf * 3 - 3]) / coef;
/*<           betar=ruatm(1,linf)/(wlinf**(alphar)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphar;
	betar = sixs_disc__1.ruatm[linf * 3 - 3] / pow_dd(&d__1, &d__2);
/*< 	  rurayl=betar*(wl**alphar) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphar;
	*rurayl = betar * pow_dd(&d__1, &d__2);
/*< 	endif >*/
    }
/*       write(6,*)'U ',uhar,rurayl,betar,alphar,ruatm(1,lsup), */
/*    s          ruatm(1,linf) */
/*< 	test1=abs(ruatm(2,linf)) >*/
    test1 = (r__1 = sixs_disc__1.ruatm[linf * 3 - 2], dabs(r__1));
/*< 	test2=abs(ruatm(2,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.ruatm[lsup * 3 - 2], dabs(r__1));
/*< 	test3=ruatm(2,lsup)*ruatm(2,linf) >*/
    test3 = sixs_disc__1.ruatm[lsup * 3 - 2] * sixs_disc__1.ruatm[linf * 3 - 
	    2];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	*rumix = sixs_disc__1.ruatm[linf * 3 - 2] + (sixs_disc__1.ruatm[lsup *
		 3 - 2] - sixs_disc__1.ruatm[linf * 3 - 2]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*< 	else >*/
    } else {
/*<           alphac=alog(ruatm(2,lsup)/ruatm(2,linf))/coef >*/
	alphac = log(sixs_disc__1.ruatm[lsup * 3 - 2] / sixs_disc__1.ruatm[
		linf * 3 - 2]) / coef;
/*<           betac=ruatm(2,linf)/(wlinf**(alphac)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphac;
	betac = sixs_disc__1.ruatm[linf * 3 - 2] / pow_dd(&d__1, &d__2);
/*<           rumix=betac*(wl**alphac) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphac;
	*rumix = betac * pow_dd(&d__1, &d__2);
/*<         endif >*/
    }
/*<         if(iaer.eq.0) goto 4234 >*/
    if (*iaer == 0) {
	goto L4234;
    }
/*< 	test1=abs(ruatm(3,linf)) >*/
    test1 = (r__1 = sixs_disc__1.ruatm[linf * 3 - 1], dabs(r__1));
/*< 	test2=abs(ruatm(3,lsup)) >*/
    test2 = (r__1 = sixs_disc__1.ruatm[lsup * 3 - 1], dabs(r__1));
/*< 	test3=ruatm(3,lsup)*ruatm(3,linf) >*/
    test3 = sixs_disc__1.ruatm[lsup * 3 - 1] * sixs_disc__1.ruatm[linf * 3 - 
	    1];
/*<    >*/
    if (test1 < (float).001 || test2 < (float).001 || test3 < (float)0.) {
/*<    >*/
	ruaer0 = sixs_disc__1.ruatm[linf * 3 - 1] + (sixs_disc__1.ruatm[lsup *
		 3 - 1] - sixs_disc__1.ruatm[linf * 3 - 1]) * (*wl - 
		sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 1] 
		- sixs_disc__1.wldis[linf - 1]);
/*< 	else >*/
    } else {
/*<           alphaa=alog(ruatm(3,lsup)/ruatm(3,linf))/coef >*/
	alphaa = log(sixs_disc__1.ruatm[lsup * 3 - 1] / sixs_disc__1.ruatm[
		linf * 3 - 1]) / coef;
/*<           betaa=ruatm(3,linf)/(wlinf**(alphaa)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphaa;
	betaa = sixs_disc__1.ruatm[linf * 3 - 1] / pow_dd(&d__1, &d__2);
/*<           ruaero=betaa*(wl**alphaa) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphaa;
	*ruaero = betaa * pow_dd(&d__1, &d__2);
/*<         endif >*/
    }
/*<  4234  	continue >*/
L4234:


/*<       alphar=alog(trayl(lsup)/trayl(linf))/coef >*/
    alphar = log(sixs_disc__1.trayl[lsup - 1] / sixs_disc__1.trayl[linf - 1]) 
	    / coef;
/*<       betar=trayl(linf)/(wlinf**(alphar)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphar;
    betar = sixs_disc__1.trayl[linf - 1] / pow_dd(&d__1, &d__2);
/*<       tray=betar*(wl**alphar) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphar;
    *tray = betar * pow_dd(&d__1, &d__2);
/*<       if (idatmp.ne.0.) then >*/
    if ((real) (*idatmp) != (float)0.) {
/*<         alphar=alog(traypl(lsup)/traypl(linf))/coef >*/
	alphar = log(sixs_disc__1.traypl[lsup - 1] / sixs_disc__1.traypl[linf 
		- 1]) / coef;
/*<         betar=traypl(linf)/(wlinf**(alphar)) >*/
	d__1 = (doublereal) wlinf;
	d__2 = (doublereal) alphar;
	betar = sixs_disc__1.traypl[linf - 1] / pow_dd(&d__1, &d__2);
/*<         trayp=betar*(wl**alphar) >*/
	d__1 = (doublereal) (*wl);
	d__2 = (doublereal) alphar;
	*trayp = betar * pow_dd(&d__1, &d__2);
/*<       else >*/
    } else {
/*<         trayp=0. >*/
	*trayp = (float)0.;
/*<       endif >*/
    }

/*<       if(iaer.eq.0) goto 1235 >*/
    if (*iaer == 0) {
	goto L1235;
    }
/*<       alphaa=alog(ext(lsup)*ome(lsup)/(ext(linf)*ome(linf)))/coef >*/
    alphaa = log(sixs_aer__1.ext[lsup - 1] * sixs_aer__1.ome[lsup - 1] / (
	    sixs_aer__1.ext[linf - 1] * sixs_aer__1.ome[linf - 1])) / coef;
/*<       betaa=ext(linf)*ome(linf)/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = sixs_aer__1.ext[linf - 1] * sixs_aer__1.ome[linf - 1] / pow_dd(&
	    d__1, &d__2);
/*<       tsca=taer55*betaa*(wl**alphaa)/ext(8) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *tsca = *taer55 * betaa * pow_dd(&d__1, &d__2) / sixs_aer__1.ext[7];
/*<       alphaa=alog(ext(lsup)/ext(linf))/coef >*/
    alphaa = log(sixs_aer__1.ext[lsup - 1] / sixs_aer__1.ext[linf - 1]) / 
	    coef;
/*<       betaa=ext(linf)/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = sixs_aer__1.ext[linf - 1] / pow_dd(&d__1, &d__2);
/*<       taerp=taer55p*betaa*(wl**alphaa)/ext(8) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *taerp = *taer55p * betaa * pow_dd(&d__1, &d__2) / sixs_aer__1.ext[7];
/*<       taer=taer55*betaa*(wl**alphaa)/ext(8) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *taer = *taer55 * betaa * pow_dd(&d__1, &d__2) / sixs_aer__1.ext[7];

/*<  1235 drinf=dtdif(1,linf)+dtdir(1,linf) >*/
L1235:
    drinf = sixs_disc__1.dtdif[linf * 3 - 3] + sixs_disc__1.dtdir[linf * 3 - 
	    3];
/*<       drsup=dtdif(1,lsup)+dtdir(1,lsup) >*/
    drsup = sixs_disc__1.dtdif[lsup * 3 - 3] + sixs_disc__1.dtdir[lsup * 3 - 
	    3];
/*<       alphar=alog(drsup/drinf)/coef >*/
    alphar = log(drsup / drinf) / coef;
/*<       betar=drinf/(wlinf**(alphar)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphar;
    betar = drinf / pow_dd(&d__1, &d__2);
/*<       dtotr=betar*(wl**alphar) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphar;
    *dtotr = betar * pow_dd(&d__1, &d__2);
/*<       dtinf=dtdif(2,linf)+dtdir(2,linf) >*/
    dtinf = sixs_disc__1.dtdif[linf * 3 - 2] + sixs_disc__1.dtdir[linf * 3 - 
	    2];
/*<       dtsup=dtdif(2,lsup)+dtdir(2,lsup) >*/
    dtsup = sixs_disc__1.dtdif[lsup * 3 - 2] + sixs_disc__1.dtdir[lsup * 3 - 
	    2];
/*<       alphac=alog((dtsup*drinf)/(dtinf*drsup))/coef >*/
    alphac = log(dtsup * drinf / (dtinf * drsup)) / coef;
/*<       betac=(dtinf/drinf)/(wlinf**(alphac)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphac;
    betac = dtinf / drinf / pow_dd(&d__1, &d__2);
/*<       dtotc=betac*(wl**alphac) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphac;
    dtotc = betac * pow_dd(&d__1, &d__2);
/*<       dainf=dtdif(3,linf)+dtdir(3,linf) >*/
    dainf = sixs_disc__1.dtdif[linf * 3 - 1] + sixs_disc__1.dtdir[linf * 3 - 
	    1];
/*<       dasup=dtdif(3,lsup)+dtdir(3,lsup) >*/
    dasup = sixs_disc__1.dtdif[lsup * 3 - 1] + sixs_disc__1.dtdir[lsup * 3 - 
	    1];
/*<       if(iaer.eq.0) goto 1236 >*/
    if (*iaer == 0) {
	goto L1236;
    }
/*<       alphaa=alog(dasup/dainf)/coef >*/
    alphaa = log(dasup / dainf) / coef;
/*<       betaa=dainf/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = dainf / pow_dd(&d__1, &d__2);
/*<       dtota=betaa*(wl**alphaa) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *dtota = betaa * pow_dd(&d__1, &d__2);
/*<  1236 dtott=dtotc*dtotr >*/
L1236:
    *dtott = dtotc * *dtotr;
/*<       urinf=utdif(1,linf)+utdir(1,linf) >*/
    urinf = sixs_disc__1.utdif[linf * 3 - 3] + sixs_disc__1.utdir[linf * 3 - 
	    3];
/*<       ursup=utdif(1,lsup)+utdir(1,lsup) >*/
    ursup = sixs_disc__1.utdif[lsup * 3 - 3] + sixs_disc__1.utdir[lsup * 3 - 
	    3];
/*<       alphar=alog(ursup/urinf)/ coef >*/
    alphar = log(ursup / urinf) / coef;
/*<       betar=urinf/(wlinf**(alphar)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphar;
    betar = urinf / pow_dd(&d__1, &d__2);
/*<       utotr=betar*(wl**alphar) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphar;
    *utotr = betar * pow_dd(&d__1, &d__2);
/*<       utinf=utdif(2,linf)+utdir(2,linf) >*/
    utinf = sixs_disc__1.utdif[linf * 3 - 2] + sixs_disc__1.utdir[linf * 3 - 
	    2];
/*<       utsup=utdif(2,lsup)+utdir(2,lsup) >*/
    utsup = sixs_disc__1.utdif[lsup * 3 - 2] + sixs_disc__1.utdir[lsup * 3 - 
	    2];
/*<       alphac=alog((utsup*urinf)/(utinf*ursup))/ coef >*/
    alphac = log(utsup * urinf / (utinf * ursup)) / coef;
/*<       betac=(utinf/urinf)/(wlinf**(alphac)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphac;
    betac = utinf / urinf / pow_dd(&d__1, &d__2);
/*<       utotc=betac*(wl**alphac) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphac;
    utotc = betac * pow_dd(&d__1, &d__2);
/*<       uainf=utdif(3,linf)+utdir(3,linf) >*/
    uainf = sixs_disc__1.utdif[linf * 3 - 1] + sixs_disc__1.utdir[linf * 3 - 
	    1];
/*<       uasup=utdif(3,lsup)+utdir(3,lsup) >*/
    uasup = sixs_disc__1.utdif[lsup * 3 - 1] + sixs_disc__1.utdir[lsup * 3 - 
	    1];
/*<       if(iaer.eq.0) goto 1237 >*/
    if (*iaer == 0) {
	goto L1237;
    }
/*<       alphaa=alog(uasup/uainf)/ coef >*/
    alphaa = log(uasup / uainf) / coef;
/*<       betaa=uainf/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = uainf / pow_dd(&d__1, &d__2);
/*<       utota=betaa*(wl**alphaa) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *utota = betaa * pow_dd(&d__1, &d__2);
/*<  1237 utott=utotc*utotr >*/
L1237:
    *utott = utotc * *utotr;
/*<       arinf=sphal(1,linf) >*/
    arinf = sixs_disc__1.sphal[linf * 3 - 3];
/*<       arsup=sphal(1,lsup) >*/
    arsup = sixs_disc__1.sphal[lsup * 3 - 3];
/*<       alphar=alog(arsup/arinf)/ coef >*/
    alphar = log(arsup / arinf) / coef;
/*<       betar=arinf/(wlinf**(alphar)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphar;
    betar = arinf / pow_dd(&d__1, &d__2);
/*<       asray=betar*(wl**alphar) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphar;
    *asray = betar * pow_dd(&d__1, &d__2);
/*<       atinf=sphal(2,linf) >*/
    atinf = sixs_disc__1.sphal[linf * 3 - 2];
/*<       atsup=sphal(2,lsup) >*/
    atsup = sixs_disc__1.sphal[lsup * 3 - 2];
/*<       alphac=alog(atsup/atinf)/coef >*/
    alphac = log(atsup / atinf) / coef;
/*<       betac=atinf/(wlinf**(alphac)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphac;
    betac = atinf / pow_dd(&d__1, &d__2);
/*<       astot=betac*(wl**alphac) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphac;
    *astot = betac * pow_dd(&d__1, &d__2);
/*<       aainf=sphal(3,linf) >*/
    aainf = sixs_disc__1.sphal[linf * 3 - 1];
/*<       aasup=sphal(3,lsup) >*/
    aasup = sixs_disc__1.sphal[lsup * 3 - 1];
/*<       if(iaer.eq.0) goto 1239 >*/
    if (*iaer == 0) {
	goto L1239;
    }
/*<       alphaa=alog(aasup/aainf)/coef >*/
    alphaa = log(aasup / aainf) / coef;
/*<       betaa=aainf/(wlinf**(alphaa)) >*/
    d__1 = (doublereal) wlinf;
    d__2 = (doublereal) alphaa;
    betaa = aainf / pow_dd(&d__1, &d__2);
/*<       asaer=betaa*(wl**alphaa) >*/
    d__1 = (doublereal) (*wl);
    d__2 = (doublereal) alphaa;
    *asaer = betaa * pow_dd(&d__1, &d__2);
/*<  1239 return >*/
L1239:
    return 0;
/*<       end >*/
} /* interp_ */

#ifdef __cplusplus
	}
#endif
