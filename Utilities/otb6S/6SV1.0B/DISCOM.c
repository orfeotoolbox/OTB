/* DISCOM.f -- translated by f2c (version 19970805).
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

/*OTB */
/*Extern*/ struct {
    doublereal pha[1000], qha[1000], uha[1000], alphal[1001], betal[1001], 
	    gammal[1001], zetal[1001];
} sixs_polar__;

#define sixs_polar__1 sixs_polar__

Extern struct {
    doublereal ext[20], ome[20], gasym[20], phase[20], qhase[20], uhase[20];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

Extern struct {
    doublereal roatm[60]	/* was [3][20] */, dtdir[60]	/* was [3][20]
	     */, dtdif[60]	/* was [3][20] */, utdir[60]	/* was [3][20]
	     */, utdif[60]	/* was [3][20] */, sphal[60]	/* was [3][20]
	     */, wldis[20], trayl[20], traypl[20], rqatm[60]	/* was [3][20]
	     */, ruatm[60]	/* was [3][20] */;
} sixs_disc__;

#define sixs_disc__1 sixs_disc__

Extern struct {
    doublereal s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

Extern struct {
    integer num_z__;
    doublereal alt_z__[101], taer_z__[101], taer55_z__[101];
} aeroprof_;

#define aeroprof_1 aeroprof_

/*<    >*/
/* Subroutine */ int discom_(integer *idatmp, integer *iaer, integer *
	iaer_prof__, doublereal *xmus, doublereal *xmuv, doublereal *phi, 
	doublereal *taer55, doublereal *taer55p, doublereal *palt, doublereal 
	*phirad, integer *nt, integer *mu, integer *np, doublereal *rm, 
	doublereal *gb, doublereal *rp, doublereal *ftray, integer *ipol, 
	doublereal *xlm1, doublereal *xlm2, doublereal *roatm_fi__, integer *
	nfi, integer *nfilut, doublereal *filut, doublereal *roluts, 
	doublereal *rolutsq, doublereal *rolutsu)
{
    /* System generated locals */
    integer rm_offset, gb_offset, xlm1_dim1, xlm1_offset, xlm2_dim1, 
	    xlm2_offset, roluts_dim2, roluts_offset, rolutsq_dim2, 
	    rolutsq_offset, rolutsu_dim2, rolutsu_offset, filut_dim1, 
	    filut_offset, i__1;

    /* Local variables */
    doublereal romix_fi__[181];
    integer i__, j, k, l;
    doublereal rorayl_fi__[181], wl;
    integer ifi;
    doublereal taer, nbmu, piza, tray, coeff = 0., taerp, tamoy, romix, trayp, 
	    rqmix, rumix, rolut[1025]	/* was [25][41] */, ddifta, ddirta, 
	    ddiftt, ddiftr, udifta;
    extern /* Subroutine */ int atmref_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal roaero, ddirtr, rqaero, ddirtt, udirta;
    extern /* Subroutine */ int odrayl_(doublereal *, doublereal *);
    doublereal ruaero, udiftr, udiftt;
    extern /* Subroutine */ int trunca_(doublereal *, integer *), scatra_(
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal rorayl, tamoyp, rqrayl, udirtt, udirtr, rurayl, rolutq[1025]	
	    /* was [25][41] */, pizmoy, rolutu[1025]	/* was [25][41] */, 
	    sphalba, sphalbr, sphalbt;

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
/*<       real nbmu  >*/
/* - to vary the number of quadratures */
/*<       integer mu,np,ipol >*/
/*<       integer idatmp,iaer,nt,l,k >*/
/*<       real rm(-mu:mu),rp(np),gb(-mu:mu) >*/
/*<       real ftray,xlm1(-mu:mu,np),xlm2(-mu:mu,np) >*/
/*<       real roatm_fi(3,20,nfi) >*/
/* THOMAS */
/* 			real rorayl_fi(nfi),romix_fi(nfi) */
/*< 			real rorayl_fi(181),romix_fi(181) >*/
/* THOMAS */
/*      real rolut(mu,41) */
/*<       real rolut(25,41) >*/
/*< 			real roluts(20,mu,41) >*/
/* THOMAS */
/*      real rolutq(mu,41) */
/*<       real rolutq(25,41) >*/
/*< 			real rolutsq(20,mu,41) >*/
/* THOMAS */
/*      real rolutu(mu,41) */
/*<       real rolutu(25,41) >*/
/*< 			real rolutsu(20,mu,41) >*/
/*<       real filut(mu,41) >*/
/*<       integer nfilut(mu) >*/
/*<       real xmus,xmuv,phi >*/
/*<       real taer55,taer55p,palt,phirad,ext,ome,gasym,phase,roatm >*/
/*<       real dtdir,dtdif,utdir,utdif,sphal,wldis,trayl,traypl,s >*/
/*<       real wlinf,wlsup,wl,tray,trayp,taer >*/
/*<       real rqatm,ruatm,rurayl,ruaero,rumix,qhase,uhase >*/
/*<       real taerp,piza,tamoy,tamoyp,pizmoy,rorayl,rqrayl,rqmix,rqaero >*/
/*<       real roaero,romix,ddirtt,ddiftt,udirtt,udiftt,sphalbt,ddirtr >*/
/*<       real ddiftr,udirtr,udiftr,sphalbr,ddirta,ddifta,udirta,udifta >*/
/*<       real sphalba,coeff >*/
/*<    >*/
/*<    >*/
/*<       common /sixs_ffu/s(1501),wlinf,wlsup >*/
/*<       real alt_z,taer_z,taer55_z >*/
/*<    >*/
/*<       integer iaer_prof >*/
/*     computation of all scattering parameters at wavelength */
/*     discrete values,so we can interpolate at any wavelength */
/*<       do 50 l=1,20 >*/
    /* Parameter adjustments */
    rolutsu_dim2 = *mu;
    rolutsu_offset = (rolutsu_dim2 + 1) * 20 + 1;
    rolutsu -= rolutsu_offset;
    rolutsq_dim2 = *mu;
    rolutsq_offset = (rolutsq_dim2 + 1) * 20 + 1;
    rolutsq -= rolutsq_offset;
    roluts_dim2 = *mu;
    roluts_offset = (roluts_dim2 + 1) * 20 + 1;
    roluts -= roluts_offset;
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
    roatm_fi__ -= 64;

    /* Function Body */
    for (l = 1; l <= 20; ++l) {
/*<         wl=wldis(l) >*/
	wl = sixs_disc__1.wldis[l - 1];
/*<         if ((wlsup.lt.wldis(1)).and.(l.le.2)) goto 30 >*/
	if (sixs_ffu__1.wlsup < sixs_disc__1.wldis[0] && l <= 2) {
	    goto L30;
	}
/*<         if (wlinf.gt.wldis(20).and.(l.ge.19)) goto 30 >*/
	if (sixs_ffu__1.wlinf > sixs_disc__1.wldis[19] && l >= 19) {
	    goto L30;
	}
/*<    >*/
	if (l < 20 && sixs_disc__1.wldis[l - 1] < sixs_ffu__1.wlinf && 
		sixs_disc__1.wldis[l] < sixs_ffu__1.wlinf) {
	    goto L50;
	}
/*<    >*/
	if (l > 1 && sixs_disc__1.wldis[l - 1] > sixs_ffu__1.wlsup && 
		sixs_disc__1.wldis[l - 2] > sixs_ffu__1.wlsup) {
	    goto L50;
	}
/*     computation of rayleigh optical depth at wl */
/*<  30     call odrayl(wl,tray) >*/
L30:
	odrayl_(&wl, &tray);
/* plane case discussed here above */
/*<       if (idatmp.eq.0.or.idatmp.eq.4) then >*/
	if (*idatmp == 0 || *idatmp == 4) {
/*< 	  if (idatmp.eq.4) trayp=tray >*/
	    if (*idatmp == 4) {
		trayp = tray;
	    }
/*< 	  if (idatmp.eq.0) trayp=0. >*/
	    if (*idatmp == 0) {
		trayp = 0.;
	    }
/*< 	else >*/
	} else {
/*<           trayp=tray*ftray >*/
	    trayp = tray * *ftray;
/*<         endif >*/
	}
/*<         trayl(l)=tray >*/
	sixs_disc__1.trayl[l - 1] = tray;
/*<         traypl(l)=trayp >*/
	sixs_disc__1.traypl[l - 1] = trayp;
/*     computation of aerosol optical properties at wl */
/*<         taer=taer55*ext(l) >*/
	taer = *taer55 * sixs_aer__1.ext[l - 1];
/*<         taerp=taer55p*ext(l) >*/
	taerp = *taer55p * sixs_aer__1.ext[l - 1];
/*<         piza=ome(l) >*/
	piza = sixs_aer__1.ome[l - 1];
/* - in case of the user-defined aerosol profile */
/*<         do i=1,num_z >*/
	i__1 = aeroprof_1.num_z__;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<          taer_z(i)=taer55_z(i)*ext(l) >*/
	    aeroprof_1.taer_z__[i__] = aeroprof_1.taer55_z__[i__] * 
		    sixs_aer__1.ext[l - 1];
/*<         enddo >*/
	}
/* - in case of the user-defined aerosol profile */

/*     computation of atmospheric reflectances */
/*               rorayl is rayleigh ref */
/*               roaero is aerosol ref */
/*     call trunca to decompose aerosol phase function in */
/*     Legendre polynomials. */
/*<         nbmu=nquad >*/
	nbmu = (doublereal) num_quad__1.nquad;
/*<           do k=1,nbmu >*/
	i__1 = (integer) nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<             pha(k)=phasel(l,k) >*/
	    sixs_polar__1.pha[k - 1] = sixs_phase__1.phasel[l + k * 20 - 21];
/*<           enddo >*/
	}
/*< 	  if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*<             do k=1,nbmu >*/
	    i__1 = (integer) nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<               qha(k)=qhasel(l,k) >*/
		sixs_polar__1.qha[k - 1] = sixs_phase__1.qhasel[l + k * 20 - 
			21];
/*<               uha(k)=uhasel(l,k) >*/
		sixs_polar__1.uha[k - 1] = sixs_phase__1.uhasel[l + k * 20 - 
			21];
/*<             enddo >*/
	    }
/*< 	  endif >*/
	}
/*< 	if (iaer.ne.0) call trunca(coeff,ipol) ! the routine is modified >*/
	if (*iaer != 0) {
	    trunca_(&coeff, ipol);
	}
/*<         tamoy=taer*(1.-piza*coeff) >*/
	tamoy = taer * (1. - piza * coeff);
/*<         tamoyp=taerp*(1.-piza*coeff) >*/
	tamoyp = taerp * (1. - piza * coeff);
/*<         pizmoy=piza*(1.-coeff)/(1.-piza*coeff) >*/
	pizmoy = piza * (1. - coeff) / (1. - piza * coeff);
/*<    >*/
	atmref_(iaer, iaer_prof__, &tamoy, &taer, &tray, &pizmoy, &piza, &
		tamoyp, &taerp, &trayp, palt, phi, xmus, xmuv, phirad, nt, mu,
		 np, &rm[rm_offset], &gb[gb_offset], &rp[1], &rorayl, &roaero,
		 &romix, &rqrayl, &rqaero, &rqmix, &rurayl, &ruaero, &rumix, 
		ipol, &xlm1[xlm1_offset], &xlm2[xlm2_offset], rorayl_fi__, 
		romix_fi__, nfi, &nfilut[1], &filut[filut_offset], rolut, 
		rolutq, rolutu);
/*<         roatm(1,l)=rorayl >*/
	sixs_disc__1.roatm[l * 3 - 3] = rorayl;
/*<         roatm(2,l)=romix >*/
	sixs_disc__1.roatm[l * 3 - 2] = romix;
/*<         roatm(3,l)=roaero >*/
	sixs_disc__1.roatm[l * 3 - 1] = roaero;
/*        write(6,*) 'rorayl',rorayl */
/*        write(6,*)'romix',romix */
/*        write(6,*)'roaero',roaero */
/*<         rqatm(1,l)=rqrayl >*/
	sixs_disc__1.rqatm[l * 3 - 3] = rqrayl;
/*<         rqatm(2,l)=rqmix >*/
	sixs_disc__1.rqatm[l * 3 - 2] = rqmix;
/*<         rqatm(3,l)=rqaero >*/
	sixs_disc__1.rqatm[l * 3 - 1] = rqaero;
/*        write(6,*) 'rqrayl',rqrayl */
/*        write(6,*)'rqmix',rqmix */
/*        write(6,*)'rqaero',rqaero */
/*<         ruatm(1,l)=rurayl >*/
	sixs_disc__1.ruatm[l * 3 - 3] = rurayl;
/*<         ruatm(2,l)=rumix >*/
	sixs_disc__1.ruatm[l * 3 - 2] = rumix;
/*<         ruatm(3,l)=ruaero >*/
	sixs_disc__1.ruatm[l * 3 - 1] = ruaero;
/*       write(6,*) 'rurayl',rurayl */
/*        write(6,*)'rumix',rumix */
/*        write(6,*)'ruaero',ruaero */
/*<       do ifi=1,nfi >*/
	i__1 = *nfi;
	for (ifi = 1; ifi <= i__1; ++ifi) {
/*<       roatm_fi(1,l,ifi)=rorayl_fi(ifi) >*/
	    roatm_fi__[(l + ifi * 20) * 3 + 1] = rorayl_fi__[ifi - 1];
/*<       roatm_fi(2,l,ifi)=romix_fi(ifi) >*/
	    roatm_fi__[(l + ifi * 20) * 3 + 2] = romix_fi__[ifi - 1];
/*<       enddo >*/
	}
/*<       do i=1,mu >*/
	i__1 = *mu;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<       do j=1,41 >*/
	    for (j = 1; j <= 41; ++j) {
/*<       roluts(l,i,j)=rolut(i,j) >*/
		roluts[l + (i__ + j * roluts_dim2) * 20] = rolut[i__ + j * 25 
			- 26];
/*<       rolutsq(l,i,j)=rolutq(i,j) >*/
		rolutsq[l + (i__ + j * rolutsq_dim2) * 20] = rolutq[i__ + j * 
			25 - 26];
/*<       rolutsu(l,i,j)=rolutu(i,j) >*/
		rolutsu[l + (i__ + j * rolutsu_dim2) * 20] = rolutu[i__ + j * 
			25 - 26];
/*<       enddo >*/
	    }
/*<       enddo >*/
	}
/*     computation of scattering transmitances (direct and diffuse) */
/*     first time for rayleigh ,next total (rayleigh+aerosols) */
/*<    >*/
	scatra_(iaer_prof__, &tamoy, &tamoyp, &tray, &trayp, &pizmoy, palt, 
		nt, mu, &rm[rm_offset], &gb[gb_offset], xmus, xmuv, &ddirtt, &
		ddiftt, &udirtt, &udiftt, &sphalbt, &ddirtr, &ddiftr, &udirtr,
		 &udiftr, &sphalbr, &ddirta, &ddifta, &udirta, &udifta, &
		sphalba);
/*<         dtdir(1,l)=ddirtr >*/
	sixs_disc__1.dtdir[l * 3 - 3] = ddirtr;
/*<         dtdif(1,l)=ddiftr >*/
	sixs_disc__1.dtdif[l * 3 - 3] = ddiftr;
/*<         dtdir(2,l)=ddirtt >*/
	sixs_disc__1.dtdir[l * 3 - 2] = ddirtt;
/*<         dtdif(2,l)=ddiftt >*/
	sixs_disc__1.dtdif[l * 3 - 2] = ddiftt;
/*<         dtdir(3,l)=ddirta >*/
	sixs_disc__1.dtdir[l * 3 - 1] = ddirta;
/*<         dtdif(3,l)=ddifta >*/
	sixs_disc__1.dtdif[l * 3 - 1] = ddifta;
/*<         utdir(1,l)=udirtr >*/
	sixs_disc__1.utdir[l * 3 - 3] = udirtr;
/*<         utdif(1,l)=udiftr >*/
	sixs_disc__1.utdif[l * 3 - 3] = udiftr;
/*<         utdir(2,l)=udirtt >*/
	sixs_disc__1.utdir[l * 3 - 2] = udirtt;
/*<         utdif(2,l)=udiftt >*/
	sixs_disc__1.utdif[l * 3 - 2] = udiftt;
/*<         utdir(3,l)=udirta >*/
	sixs_disc__1.utdir[l * 3 - 1] = udirta;
/*<         utdif(3,l)=udifta >*/
	sixs_disc__1.utdif[l * 3 - 1] = udifta;
/*<         sphal(1,l)=sphalbr >*/
	sixs_disc__1.sphal[l * 3 - 3] = sphalbr;
/*<         sphal(2,l)=sphalbt >*/
	sixs_disc__1.sphal[l * 3 - 2] = sphalbt;
/*<         sphal(3,l)=sphalba >*/
	sixs_disc__1.sphal[l * 3 - 1] = sphalba;
/*<    50 continue >*/
L50:
	;
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* discom_ */

#ifdef __cplusplus
	}
#endif
