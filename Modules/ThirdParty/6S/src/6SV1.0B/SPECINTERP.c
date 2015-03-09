/* SPECINTERP.f -- translated by f2c (version 19970805).
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
    integer nquad;
} num_quad__;

#define num_quad__1 num_quad__

Extern struct {
    doublereal phasel[20000]	/* was [20][1000] */, qhasel[20000]	/* 
	    was [20][1000] */, uhasel[20000]	/* was [20][1000] */;
} sixs_phase__;

#define sixs_phase__1 sixs_phase__

Extern struct {
    doublereal pha[1000], qha[1000], uha[1000], alphal[1001], betal[1001], 
	    gammal[1001], zetal[1001];
} sixs_polar__;

#define sixs_polar__1 sixs_polar__

Extern struct {
    doublereal roatm[60]	/* was [3][20] */, dtdir[60]	/* was [3][20]
	     */, dtdif[60]	/* was [3][20] */, utdir[60]	/* was [3][20]
	     */, utdif[60]	/* was [3][20] */, sphal[60]	/* was [3][20]
	     */, wldis[20], trayl[20], traypl[20], rpatm[60]	/* was [3][20]
	     */, dpatm[60]	/* was [3][20] */;
} sixs_disc__;

#define sixs_disc__1 sixs_disc__

Extern struct {
    doublereal ext[20], ome[20], gasym[20], phase[20], qhase[20], uhase[20];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

/*<    >*/
/* Subroutine */ int specinterp_(doublereal *wl, doublereal *taer55, 
	doublereal *taer55p, doublereal *tamoy, doublereal *tamoyp, 
	doublereal *pizmoy, doublereal *pizmoyp, integer *ipol)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double log(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    integer k, ll;
    doublereal coef;
    integer linf;
    doublereal tsca;
    integer nbmu, lsup, ipol0;
    doublereal test1, test2, test3, betaa, coeff, coefl, wlinf, alphaa;
    extern /* Subroutine */ int trunca_(doublereal *, integer *);

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real phasel,qhasel,uhasel >*/
/*<    >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/*<       integer nbmu >*/
/* - to vary the number of quadratures */
/*<       real wl,taer55,taer55p,tamoy,tamoyp,pizmoy,pizmoyp,roatm >*/
/*<       real dtdir,dtdif,utdir,utdif,sphal,wldis,trayl,traypl >*/
/*<       real ext,ome,gasym,phase,coef,rpatm,dpatm >*/
/*<       real qhase,uhase >*/
/*<       real wlinf,alphaa,betaa,tsca,coeff >*/
/*<       integer linf,ll,lsup,k,ipol,ipol0 >*/
/*<    >*/
/*<    >*/
/*<       real test1,test2,test3 >*/
/*<       nbmu=nquad >*/
    nbmu = num_quad__1.nquad;
/*<       linf=1 >*/
    linf = 1;
/*<       do 80 ll=1,19 >*/
    for (ll = 1; ll <= 19; ++ll) {
/*<         if(wl.ge.wldis(ll).and.wl.le.wldis(ll+1)) linf=ll >*/
	if (*wl >= sixs_disc__1.wldis[ll - 1] && *wl <= sixs_disc__1.wldis[ll]
		) {
	    linf = ll;
	}
/*<    80 continue >*/
/* L80: */
    }
/*<       if(wl.gt.wldis(20)) linf=19 >*/
    if (*wl > sixs_disc__1.wldis[19]) {
	linf = 19;
    }
/*<       lsup=linf+1 >*/
    lsup = linf + 1;
/*<       coef=alog(wldis(lsup)/wldis(linf)) >*/
    coef = log(sixs_disc__1.wldis[lsup - 1] / sixs_disc__1.wldis[linf - 1]);
/*<       coefl=(wl-wldis(linf))/(wldis(lsup)-wldis(linf)) >*/
    coefl = (*wl - sixs_disc__1.wldis[linf - 1]) / (sixs_disc__1.wldis[lsup - 
	    1] - sixs_disc__1.wldis[linf - 1]);
/*<       wlinf=wldis(linf) >*/
    wlinf = sixs_disc__1.wldis[linf - 1];
/*<       alphaa=alog(ext(lsup)*ome(lsup)/(ext(linf)*ome(linf)))/coef >*/
    alphaa = log(sixs_aer__1.ext[lsup - 1] * sixs_aer__1.ome[lsup - 1] / (
	    sixs_aer__1.ext[linf - 1] * sixs_aer__1.ome[linf - 1])) / coef;
/*<       betaa=ext(linf)*ome(linf)/(wlinf**(alphaa)) >*/
    betaa = sixs_aer__1.ext[linf - 1] * sixs_aer__1.ome[linf - 1] / pow_dd(&
	    wlinf, &alphaa);
/*<       tsca=taer55*betaa*(wl**alphaa)/ext(4) >*/
    tsca = *taer55 * betaa * pow_dd(wl, &alphaa) / sixs_aer__1.ext[3];
/*<       alphaa=alog(ext(lsup)/(ext(linf)))/coef >*/
    alphaa = log(sixs_aer__1.ext[lsup - 1] / sixs_aer__1.ext[linf - 1]) / 
	    coef;
/*<       betaa=ext(linf)/(wlinf**(alphaa)) >*/
    betaa = sixs_aer__1.ext[linf - 1] / pow_dd(&wlinf, &alphaa);
/*<       tamoy=taer55*betaa*(wl**alphaa)/ext(4) >*/
    *tamoy = *taer55 * betaa * pow_dd(wl, &alphaa) / sixs_aer__1.ext[3];
/*<       tamoyp=taer55p*betaa*(wl**alphaa)/ext(4) >*/
    *tamoyp = *taer55p * betaa * pow_dd(wl, &alphaa) / sixs_aer__1.ext[3];
/*<       pizmoy=tsca/tamoy >*/
    *pizmoy = tsca / *tamoy;
/*<       pizmoyp=pizmoy >*/
    *pizmoyp = *pizmoy;
/*<       do 81 k=1,nbmu >*/
    i__1 = nbmu;
    for (k = 1; k <= i__1; ++k) {
/*<         alphaa=alog(phasel(lsup,k)/phasel(linf,k))/coef >*/
	alphaa = log(sixs_phase__1.phasel[lsup + k * 20 - 21] / 
		sixs_phase__1.phasel[linf + k * 20 - 21]) / coef;
/*<         betaa=phasel(linf,k)/(wlinf**(alphaa)) >*/
	betaa = sixs_phase__1.phasel[linf + k * 20 - 21] / pow_dd(&wlinf, &
		alphaa);
/*<         pha(k)=betaa*(wl**alphaa) >*/
	sixs_polar__1.pha[k - 1] = betaa * pow_dd(wl, &alphaa);
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	  test1=qhasel(linf,k) >*/
	    test1 = sixs_phase__1.qhasel[linf + k * 20 - 21];
/*< 	  test2=qhasel(lsup,k) >*/
	    test2 = sixs_phase__1.qhasel[lsup + k * 20 - 21];
/*< 	  test3=qhasel(lsup,k)*qhasel(linf,k) >*/
	    test3 = sixs_phase__1.qhasel[lsup + k * 20 - 21] * 
		    sixs_phase__1.qhasel[linf + k * 20 - 21];
/*<    >*/
	    if (test1 < .001 || test2 < .001 || test3 < 0.) {
/*< 	  qha(k)=qhasel(linf,k)+(qhasel(lsup,k)-qhasel(linf,k))*coefl >*/
		sixs_polar__1.qha[k - 1] = sixs_phase__1.qhasel[linf + k * 20 
			- 21] + (sixs_phase__1.qhasel[lsup + k * 20 - 21] - 
			sixs_phase__1.qhasel[linf + k * 20 - 21]) * coefl;
/*< 	  else >*/
	    } else {
/*<           alphaa=alog(qhasel(lsup,k)/qhasel(linf,k))/coef >*/
		alphaa = log(sixs_phase__1.qhasel[lsup + k * 20 - 21] / 
			sixs_phase__1.qhasel[linf + k * 20 - 21]) / coef;
/*<           betaa=qhasel(linf,k)/(wlinf**(alphaa)) >*/
		betaa = sixs_phase__1.qhasel[linf + k * 20 - 21] / pow_dd(&
			wlinf, &alphaa);
/*<           qha(k)=betaa*(wl**alphaa) >*/
		sixs_polar__1.qha[k - 1] = betaa * pow_dd(wl, &alphaa);
/*< 	  endif >*/
	    }
/*< 	  test1=uhasel(linf,k) >*/
	    test1 = sixs_phase__1.uhasel[linf + k * 20 - 21];
/*< 	  test2=uhasel(lsup,k) >*/
	    test2 = sixs_phase__1.uhasel[lsup + k * 20 - 21];
/*< 	  test3=uhasel(lsup,k)*qhasel(linf,k) >*/
	    test3 = sixs_phase__1.uhasel[lsup + k * 20 - 21] * 
		    sixs_phase__1.qhasel[linf + k * 20 - 21];
/*<    >*/
	    if (test1 < .001 || test2 < .001 || test3 < 0.) {
/*< 	  uha(k)=uhasel(linf,k)+(uhasel(lsup,k)-uhasel(linf,k))*coefl >*/
		sixs_polar__1.uha[k - 1] = sixs_phase__1.uhasel[linf + k * 20 
			- 21] + (sixs_phase__1.uhasel[lsup + k * 20 - 21] - 
			sixs_phase__1.uhasel[linf + k * 20 - 21]) * coefl;
/*< 	  else >*/
	    } else {
/*<           alphaa=alog(uhasel(lsup,k)/uhasel(linf,k))/coef >*/
		alphaa = log(sixs_phase__1.uhasel[lsup + k * 20 - 21] / 
			sixs_phase__1.uhasel[linf + k * 20 - 21]) / coef;
/*<           betaa=uhasel(linf,k)/(wlinf**(alphaa)) >*/
		betaa = sixs_phase__1.uhasel[linf + k * 20 - 21] / pow_dd(&
			wlinf, &alphaa);
/*<           uha(k)=betaa*(wl**alphaa) >*/
		sixs_polar__1.uha[k - 1] = betaa * pow_dd(wl, &alphaa);
/*< 	  endif >*/
	    }
/*< 	endif   >*/
	}
/*<  81     continue >*/
/* L81: */
    }
/* here we don't need coefficients for computation of the polarization */
/*<       ipol0=0 >*/
    ipol0 = 0;
/*<       call trunca(coeff,ipol0) >*/
    trunca_(&coeff, &ipol0);
/*<       tamoy=tamoy*(1.-pizmoy*coeff) >*/
    *tamoy *= 1. - *pizmoy * coeff;
/*<       tamoyp=tamoyp*(1.-pizmoyp*coeff) >*/
    *tamoyp *= 1. - *pizmoyp * coeff;
/*<       pizmoy=pizmoy*(1.-coeff)/(1.-pizmoy*coeff) >*/
    *pizmoy = *pizmoy * (1. - coeff) / (1. - *pizmoy * coeff);
/*<       return >*/
    return 0;
/*<       end >*/
} /* specinterp_ */

#ifdef __cplusplus
	}
#endif
