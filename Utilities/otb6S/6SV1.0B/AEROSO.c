/* AEROSO.f -- translated by f2c (version 19970805).
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

/*OTB */
/*Extern*/ struct {
    doublereal ph[20000]	/* was [20][1000] */, qh[20000]	/* was [20][
	    1000] */, uh[20000]	/* was [20][1000] */;
} sixs_aerbas__;

#define sixs_aerbas__1 sixs_aerbas__

/*OTB */
/*Extern*/ struct {
    doublereal phasel[20000]	/* was [20][1000] */, qhasel[20000]	/* 
	    was [20][1000] */, uhasel[20000]	/* was [20][1000] */;
} sixs_phase__;

#define sixs_phase__1 sixs_phase__

/*OTB */
/*Extern*/ struct {
    doublereal ext[20], ome[20], gasym[20], phase[20], qhase[20], uhase[20];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

/*OTB */
/*Extern*/ struct {
    doublereal ex[80]	/* was [4][20] */, sc[80]	/* was [4][20] */, 
	    asy[80]	/* was [4][20] */, vi[4];
} sixs_coef__;

#define sixs_coef__1 sixs_coef__

/* Table of constant values */

static integer c__3 = 3;
static integer c__1 = 1;
static doublereal c_b19 = -1.;
static doublereal c_b20 = 1.;

/*<       subroutine aeroso (iaer,co,xmud,wldis,FILE,ipol) >*/
/* Subroutine */ int aeroso_(integer *iaer, doublereal *co, doublereal *xmud, 
	doublereal *wldis, char *file, integer *ipol, ftnlen file_len)
{

    /* Format strings */
    static char fmt_761[] = "(2x,f6.2,20(1x,e10.4))";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;
    cilist ci__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    double atan(doublereal);
    integer f_open(olist *), s_rsle(cilist *), do_lio(integer *, integer *, 
	    char *, ftnlen), e_rsle(), s_rsfe(cilist *), do_fio(integer *, 
	    char *, ftnlen), e_rsfe(), f_clos(cllist *), s_wsle(cilist *), 
	    e_wsle(), s_wsfe(cilist *), e_wsfe();
    double acos(doublereal);

    /* Local variables */
    integer i__, j, k, l, j1, j2;
    doublereal dd[80]	/* was [4][20] */, ci[4], pi, qq[80]	/* was [4][20]
	     */, uu[80]	/* was [4][20] */;
    extern /* Subroutine */ int bbm_(), bdm_();
    doublereal cij[4], sca[20], pha[100000]	/* was [5][20][1000] */, qha[
	    100000]	/* was [5][20][1000] */;
    extern /* Subroutine */ int mie_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *);
    integer icp;
    doublereal uha[100000]	/* was [5][20][1000] */, nis;
    extern /* Subroutine */ int stm_(), ocea_();
    doublereal coef, sigm;
    integer nbmu;
    extern /* Subroutine */ int wate_(), dust_(), soot_(), gauss_(doublereal *
	    , doublereal *, doublereal *, doublereal *, integer *);
    doublereal sumni;
    integer nbmu_2__;
    doublereal cosang[1000], pdgs_s__[1000], weight[1000], cgaus_s__[1000];

    /* Fortran I/O blocks */
    static cilist io___3 = { 0, 10, 0, 0, 0 };
    static cilist io___5 = { 0, 10, 0, 0, 0 };
    static cilist io___31 = { 0, 10, 0, 0, 0 };
    static cilist io___32 = { 0, 10, 0, fmt_761, 0 };
    static cilist io___33 = { 0, 10, 0, fmt_761, 0 };
    static cilist io___34 = { 0, 10, 0, fmt_761, 0 };


/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real ph,qh,uh >*/
/*<       common /sixs_aerbas/ ph(20,nqmax_p),qh(20,nqmax_p),uh(20,nqmax_p) >*/
/*<       real cgaus_S(nqmax_p),pdgs_S(nqmax_p) >*/
/*<       real phasel,qhasel,uhasel >*/
/*<    >*/
/*<       integer nbmu, nbmu_2 >*/
/*<       real cosang(nqmax_p),weight(nqmax_p) >*/
/* - to vary the number of quadratures */
/*<       double precision cij(4),nis,sumni,ni(4) >*/
/*<       real co(4),dd(4,20),qq(4,20),ci(4) >*/
/*<       real pha(5,20,nqmax_p),qha(5,20,nqmax_p),uha(5,20,nqmax_p) >*/
/*<       real sca(20),wldis(20),uu(4,20) >*/
/*<       real xmud,ext,ome,gasym,phase,qhase,uhase >*/
/*<       real coef,sigm,pi >*/
/*<       integer i,j,k,l,j1,j2,iaer,icp,ipol >*/
/*<       character FILE*80 >*/
/*<    >*/
/*<       real ex,sc,asy,vi >*/
/*<       common /sixs_coef/ ex(4,20),sc(4,20),asy(4,20),vi(4) >*/
/*<       real wldisc(20) >*/
/*<    >*/
    /* Parameter adjustments */
    --wldis;
    --co;

    /* Function Body */
/*<       pi=4.*atan(1.)  >*/
    pi = atan(1.) * 4.;
/*      if(iaer.eq.0) return */
/*<       if (iaer.eq.12) then >*/
    if (*iaer == 12) {
/*<         open(10,file=FILE) >*/
	o__1.oerr = 0;
	o__1.ounit = 10;
	o__1.ofnmlen = 80;
	o__1.ofnm = file;
	o__1.orl = 0;
	o__1.osta = 0;
	o__1.oacc = 0;
	o__1.ofm = 0;
	o__1.oblnk = 0;
	f_open(&o__1);
/*< 	read (10,*) nbmu >*/
	s_rsle(&io___3);
	do_lio(&c__3, &c__1, (char *)&nbmu, (ftnlen)sizeof(integer));
	e_rsle();
/*<         read(10,*) >*/
	s_rsle(&io___5);
	e_rsle();
/*<         do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<           read(10,'(10x,4(3x,f8.4,3x))')ext(l),sca(l),ome(l),gasym(l) >*/
	    ci__1.cierr = 0;
	    ci__1.ciend = 0;
	    ci__1.ciunit = 10;
	    ci__1.cifmt = "(10x,4(3x,f8.4,3x))";
	    s_rsfe(&ci__1);
	    do_fio(&c__1, (char *)&sixs_aer__1.ext[l - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&sca[l - 1], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&sixs_aer__1.ome[l - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&sixs_aer__1.gasym[l - 1], (ftnlen)sizeof(
		    doublereal));
	    e_rsfe();
/*<         enddo     >*/
	}
/*<         read(10,'(///)') >*/
	ci__1.cierr = 0;
	ci__1.ciend = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(///)";
	s_rsfe(&ci__1);
	e_rsfe();
/*<         do k=1,nbmu >*/
	i__1 = nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<           read(10,'(8x,20(1x,e10.4))')(phasel(l,k),l=1,20) >*/
	    ci__1.cierr = 0;
	    ci__1.ciend = 0;
	    ci__1.ciunit = 10;
	    ci__1.cifmt = "(8x,20(1x,e10.4))";
	    s_rsfe(&ci__1);
	    for (l = 1; l <= 20; ++l) {
		do_fio(&c__1, (char *)&sixs_phase__1.phasel[l + k * 20 - 21], 
			(ftnlen)sizeof(doublereal));
	    }
	    e_rsfe();
/*<         enddo    >*/
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	  do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<  	    read(10,'(8x,20(1x,e10.4))')(qhasel(l,k),l=1,20) >*/
		ci__1.cierr = 0;
		ci__1.ciend = 0;
		ci__1.ciunit = 10;
		ci__1.cifmt = "(8x,20(1x,e10.4))";
		s_rsfe(&ci__1);
		for (l = 1; l <= 20; ++l) {
		    do_fio(&c__1, (char *)&sixs_phase__1.qhasel[l + k * 20 - 
			    21], (ftnlen)sizeof(doublereal));
		}
		e_rsfe();
/*<           enddo >*/
	    }
/*< 	  do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<  	    read(10,'(8x,20(1x,e10.4))')(uhasel(l,k),l=1,20) >*/
		ci__1.cierr = 0;
		ci__1.ciend = 0;
		ci__1.ciunit = 10;
		ci__1.cifmt = "(8x,20(1x,e10.4))";
		s_rsfe(&ci__1);
		for (l = 1; l <= 20; ++l) {
		    do_fio(&c__1, (char *)&sixs_phase__1.uhasel[l + k * 20 - 
			    21], (ftnlen)sizeof(doublereal));
		}
		e_rsfe();
/*<           enddo >*/
	    }
/*< 	endif >*/
	}
/*< 	nquad=nbmu >*/
	num_quad__1.nquad = nbmu;
/*<         close(10) >*/
	cl__1.cerr = 0;
	cl__1.cunit = 10;
	cl__1.csta = 0;
	f_clos(&cl__1);
/*<       endif >*/
    }
/* - calculation of gauss points */
/*<       nbmu=nquad  >*/
    nbmu = num_quad__1.nquad;
/*<       nbmu_2=(nbmu-3)/2 >*/
    nbmu_2__ = (nbmu - 3) / 2;
/*<       call gauss(-1.,1.,cosang,weight,nbmu-3) >*/
    i__1 = nbmu - 3;
    gauss_(&c_b19, &c_b20, cosang, weight, &i__1);
/*<       cgaus_S(1)=-1.0 >*/
    cgaus_s__[0] = -1.;
/*<       pdgs_S(1)=0.0 >*/
    pdgs_s__[0] = 0.;
/*<       do j=1,nbmu_2 >*/
    i__1 = nbmu_2__;
    for (j = 1; j <= i__1; ++j) {
/*<         cgaus_S(j+1)=cosang(j) >*/
	cgaus_s__[j] = cosang[j - 1];
/*<         pdgs_S(j+1)=weight(j) >*/
	pdgs_s__[j] = weight[j - 1];
/*<       enddo  >*/
    }
/*<       cgaus_S(nbmu_2+2)=0. >*/
    cgaus_s__[nbmu_2__ + 1] = 0.;
/*<       pdgs_S(nbmu_2+2)=0. >*/
    pdgs_s__[nbmu_2__ + 1] = 0.;
/*<       do j=nbmu_2+1,nbmu-3 >*/
    i__1 = nbmu - 3;
    for (j = nbmu_2__ + 1; j <= i__1; ++j) {
/*<         cgaus_S(j+2)=cosang(j) >*/
	cgaus_s__[j + 1] = cosang[j - 1];
/*<         pdgs_S(j+2)=weight(j) >*/
	pdgs_s__[j + 1] = weight[j - 1];
/*<       enddo >*/
    }
/*<       cgaus_S(nbmu)=1.0 >*/
    cgaus_s__[nbmu - 1] = 1.;
/*<       pdgs_S(nbmu)=0. >*/
    pdgs_s__[nbmu - 1] = 0.;
/* - calculation of gauss points */
/*<       do 7 k=1,nbmu-1 >*/
    i__1 = nbmu - 1;
    for (k = 1; k <= i__1; ++k) {
/*<         if((xmud.ge.cgaus_S(k)).and.(xmud.lt.cgaus_S(k+1))) go to 8 >*/
	if (*xmud >= cgaus_s__[k - 1] && *xmud < cgaus_s__[k]) {
	    goto L8;
	}
/*<     7 continue >*/
/* L7: */
    }
/*<       return >*/
    return 0;
/*<     8 j1=k >*/
L8:
    j1 = k;
/*<       j2=j1+1 >*/
    j2 = j1 + 1;
/*<       coef=-(xmud-cgaus_S(j1))/(cgaus_S(j2)-cgaus_S(j1)) >*/
    coef = -(*xmud - cgaus_s__[j1 - 1]) / (cgaus_s__[j2 - 1] - cgaus_s__[j1 - 
	    1]);
/*<       if (iaer.eq.12) then >*/
    if (*iaer == 12) {
/*<         do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<           phase(l)=phasel(l,j1)+coef*(phasel(l,j1)-phasel(l,j2)) >*/
	    sixs_aer__1.phase[l - 1] = sixs_phase__1.phasel[l + j1 * 20 - 21] 
		    + coef * (sixs_phase__1.phasel[l + j1 * 20 - 21] - 
		    sixs_phase__1.phasel[l + j2 * 20 - 21]);
/*<         enddo >*/
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*<           do l=1,20 >*/
	    for (l = 1; l <= 20; ++l) {
/*<             qhase(l)=qhasel(l,j1)+coef*(qhasel(l,j1)-qhasel(l,j2)) >*/
		sixs_aer__1.qhase[l - 1] = sixs_phase__1.qhasel[l + j1 * 20 - 
			21] + coef * (sixs_phase__1.qhasel[l + j1 * 20 - 21] 
			- sixs_phase__1.qhasel[l + j2 * 20 - 21]);
/*<             uhase(l)=uhasel(l,j1)+coef*(uhasel(l,j1)-uhasel(l,j2)) >*/
		sixs_aer__1.uhase[l - 1] = sixs_phase__1.uhasel[l + j1 * 20 - 
			21] + coef * (sixs_phase__1.uhasel[l + j1 * 20 - 21] 
			- sixs_phase__1.uhasel[l + j2 * 20 - 21]);
/*<           enddo >*/
	    }
/*< 	endif >*/
	}
/*<         return >*/
	return 0;
/*<       endif >*/
    }
/*<       do 1 l=1,20 >*/
    for (l = 1; l <= 20; ++l) {
/*<        ext(l)=0. >*/
	sixs_aer__1.ext[l - 1] = 0.;
/*<        sca(l)=0. >*/
	sca[l - 1] = 0.;
/*<        if(l.eq.4.and.iaer.eq.0) ext(l)=1. >*/
	if (l == 4 && *iaer == 0) {
	    sixs_aer__1.ext[l - 1] = 1.;
	}
/*<        ome(l)=0. >*/
	sixs_aer__1.ome[l - 1] = 0.;
/*<        gasym(l)=0. >*/
	sixs_aer__1.gasym[l - 1] = 0.;
/*<        phase(l)=0. >*/
	sixs_aer__1.phase[l - 1] = 0.;
/*<        qhase(l)=0. >*/
	sixs_aer__1.qhase[l - 1] = 0.;
/*<        uhase(l)=0. >*/
	sixs_aer__1.uhase[l - 1] = 0.;
/*<        do 1 k=1,nbmu >*/
	i__1 = nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<         phasel(l,k)=0. >*/
	    sixs_phase__1.phasel[l + k * 20 - 21] = 0.;
/*<         qhasel(l,k)=0. >*/
	    sixs_phase__1.qhasel[l + k * 20 - 21] = 0.;
/*<         uhasel(l,k)=0. >*/
	    sixs_phase__1.uhasel[l + k * 20 - 21] = 0.;
/*<     1 continue >*/
/* L1: */
	}
    }
/*<       do 2 j=1,4 >*/
    for (j = 1; j <= 4; ++j) {
/*<        ci(j)=co(j) >*/
	ci[j - 1] = co[j];
/*<     2 continue >*/
/* L2: */
    }
/*<       if(iaer.eq.0) goto 777 >*/
    if (*iaer == 0) {
	goto L777;
    }
/*<       if (iaer.ge.5.and.iaer.le.11) then >*/
    if (*iaer >= 5 && *iaer <= 11) {
/* calling a special aerosol model */
/*     (background desert model...) */
/*<          if (iaer.eq.5) call bdm >*/
	if (*iaer == 5) {
	    bdm_();
	}
/*     (biomass burning model...) */
/*<          if (iaer.eq.6) call bbm >*/
	if (*iaer == 6) {
	    bbm_();
	}
/*     (stratospherique aerosol model...) */
/*<          if (iaer.eq.7) call stm >*/
	if (*iaer == 7) {
	    stm_();
	}
/*     (user defined model from size distribution) */
/*<          if (iaer.ge.8.and.iaer.le.11) then >*/
	if (*iaer >= 8 && *iaer <= 11) {
/*< 	   call mie(iaer,wldis,ex,sc,asy,ipol) >*/
	    mie_(iaer, &wldis[1], sixs_coef__1.ex, sixs_coef__1.sc, 
		    sixs_coef__1.asy, ipol);
/*<          endif >*/
	}
/*<          do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<            dd(1,l)=ph(l,j1)+coef*(ph(l,j1)-ph(l,j2)) >*/
	    dd[(l << 2) - 4] = sixs_aerbas__1.ph[l + j1 * 20 - 21] + coef * (
		    sixs_aerbas__1.ph[l + j1 * 20 - 21] - sixs_aerbas__1.ph[l 
		    + j2 * 20 - 21]);
/*<            do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<              pha(1,l,k)=ph(l,k) >*/
		pha[(l + k * 20) * 5 - 105] = sixs_aerbas__1.ph[l + k * 20 - 
			21];
/*<            enddo >*/
	    }
/*<          enddo >*/
	}
/*< 	 if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	   do l=1,20 >*/
	    for (l = 1; l <= 20; ++l) {
/*< 	     qq(1,l)=qh(l,j1)+coef*(qh(l,j1)-qh(l,j2)) >*/
		qq[(l << 2) - 4] = sixs_aerbas__1.qh[l + j1 * 20 - 21] + coef 
			* (sixs_aerbas__1.qh[l + j1 * 20 - 21] - 
			sixs_aerbas__1.qh[l + j2 * 20 - 21]);
/*<              uu(1,l)=uh(l,j1)+coef*(uh(l,j1)-uh(l,j2)) >*/
		uu[(l << 2) - 4] = sixs_aerbas__1.uh[l + j1 * 20 - 21] + coef 
			* (sixs_aerbas__1.uh[l + j1 * 20 - 21] - 
			sixs_aerbas__1.uh[l + j2 * 20 - 21]);
/*<              do k=1,nbmu >*/
		i__1 = nbmu;
		for (k = 1; k <= i__1; ++k) {
/*<                qha(1,l,k)=qh(l,k) >*/
		    qha[(l + k * 20) * 5 - 105] = sixs_aerbas__1.qh[l + k * 
			    20 - 21];
/*<                uha(1,l,k)=uh(l,k) >*/
		    uha[(l + k * 20) * 5 - 105] = sixs_aerbas__1.uh[l + k * 
			    20 - 21];
/*< 	     enddo >*/
		}
/*<            enddo >*/
	    }
/*< 	 endif >*/
	}
/*<          icp=1 >*/
	icp = 1;
/*<          cij(1)=1.00 >*/
	cij[0] = 1.;
/* for normalization of the extinction coefficient */
/*<          nis=1.d+00/ex(1,8) >*/
	nis = 1. / sixs_coef__1.ex[28];
/*<       else >*/
    } else {
/* calling each sra components */
/*<          icp=4 >*/
	icp = 4;
/* extrapolate each component for wavelength */
/*<       do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*< 	  do j=1,4 >*/
	    for (j = 1; j <= 4; ++j) {
/*<  	   ex(j,l)=0 >*/
		sixs_coef__1.ex[j + (l << 2) - 5] = 0.;
/*< 	   sc(j,l)=0.  >*/
		sixs_coef__1.sc[j + (l << 2) - 5] = 0.;
/*< 	   asy(j,l)=0. >*/
		sixs_coef__1.asy[j + (l << 2) - 5] = 0.;
/*< 	  enddo >*/
	    }
/*< 	  enddo >*/
	}
/*  phase function of 4 components */
/*<         do j=1,4 >*/
	for (j = 1; j <= 4; ++j) {
/*<         if (j.eq.1) call dust >*/
	    if (j == 1) {
		dust_();
	    }
/*<         if (j.eq.2) call wate >*/
	    if (j == 2) {
		wate_();
	    }
/*<         if (j.eq.3) call ocea >*/
	    if (j == 3) {
		ocea_();
	    }
/*<         if (j.eq.4) call soot >*/
	    if (j == 4) {
		soot_();
	    }
/*<          do l=1,20 >*/
	    for (l = 1; l <= 20; ++l) {
/*<            dd(j,l)=ph(l,j1)+coef*(ph(l,j1)-ph(l,j2)) >*/
		dd[j + (l << 2) - 5] = sixs_aerbas__1.ph[l + j1 * 20 - 21] + 
			coef * (sixs_aerbas__1.ph[l + j1 * 20 - 21] - 
			sixs_aerbas__1.ph[l + j2 * 20 - 21]);
/*<            do k=1,nbmu >*/
		i__1 = nbmu;
		for (k = 1; k <= i__1; ++k) {
/*<              pha(j,l,k)=ph(l,k) >*/
		    pha[j + (l + k * 20) * 5 - 106] = sixs_aerbas__1.ph[l + k 
			    * 20 - 21];
/*<            enddo >*/
		}
/*< 	   if (ipol.ne.0)then >*/
		if (*ipol != 0) {
/*<              qq(j,l)=qh(l,j1)+coef*(qh(l,j1)-qh(l,j2)) >*/
		    qq[j + (l << 2) - 5] = sixs_aerbas__1.qh[l + j1 * 20 - 21]
			     + coef * (sixs_aerbas__1.qh[l + j1 * 20 - 21] - 
			    sixs_aerbas__1.qh[l + j2 * 20 - 21]);
/*<              uu(j,l)=uh(l,j1)+coef*(uh(l,j1)-uh(l,j2)) >*/
		    uu[j + (l << 2) - 5] = sixs_aerbas__1.uh[l + j1 * 20 - 21]
			     + coef * (sixs_aerbas__1.uh[l + j1 * 20 - 21] - 
			    sixs_aerbas__1.uh[l + j2 * 20 - 21]);
/*<              do k=1,nbmu >*/
		    i__1 = nbmu;
		    for (k = 1; k <= i__1; ++k) {
/*<                qha(j,l,k)=qh(l,k) >*/
			qha[j + (l + k * 20) * 5 - 106] = sixs_aerbas__1.qh[l 
				+ k * 20 - 21];
/*<                uha(j,l,k)=uh(l,k) >*/
			uha[j + (l + k * 20) * 5 - 106] = sixs_aerbas__1.uh[l 
				+ k * 20 - 21];
/*<              enddo >*/
		    }
/*< 	   endif >*/
		}
/*<          enddo >*/
	    }
/*<       enddo  >*/
	}
/*     summ of the ci/vi calculation */
/*<          sigm=0. >*/
	sigm = 0.;
/*<          sumni=0.0 >*/
	sumni = 0.;
/*<          do 3 i=1,4 >*/
	for (i__ = 1; i__ <= 4; ++i__) {
/*<     3    sigm=sigm+ci(i)/vi(i) >*/
/* L3: */
	    sigm += ci[i__ - 1] / sixs_coef__1.vi[i__ - 1];
	}
/*     cij coefficients calculation */
/*<          do 4 j=1,4 >*/
	for (j = 1; j <= 4; ++j) {
/*<            cij(j)=(ci(j)/vi(j)/sigm) >*/
	    cij[j - 1] = ci[j - 1] / sixs_coef__1.vi[j - 1] / sigm;
/*<            sumni=sumni+cij(j)*ex(j,8) >*/
	    sumni += cij[j - 1] * sixs_coef__1.ex[j + 27];
/*<     4    continue >*/
/* L4: */
	}
/*<          nis=1.d+00/sumni >*/
	nis = 1. / sumni;
/*<       endif >*/
    }
/*     mixing parameters calculation */
/*<       do 5 l=1,20 >*/
    for (l = 1; l <= 20; ++l) {
/*<         do 6 j=1,icp >*/
	i__1 = icp;
	for (j = 1; j <= i__1; ++j) {
/*<           ext(l)=ex(j,l)*cij(j)+ext(l) >*/
	    sixs_aer__1.ext[l - 1] = sixs_coef__1.ex[j + (l << 2) - 5] * cij[
		    j - 1] + sixs_aer__1.ext[l - 1];
/*<           sca(l)=sc(j,l)*cij(j)+sca(l) >*/
	    sca[l - 1] = sixs_coef__1.sc[j + (l << 2) - 5] * cij[j - 1] + sca[
		    l - 1];
/*<           gasym(l)=sc(j,l)*cij(j)*asy(j,l)+gasym(l) >*/
	    sixs_aer__1.gasym[l - 1] = sixs_coef__1.sc[j + (l << 2) - 5] * 
		    cij[j - 1] * sixs_coef__1.asy[j + (l << 2) - 5] + 
		    sixs_aer__1.gasym[l - 1];
/*<           phase(l)=sc(j,l)*cij(j)*dd(j,l)+phase(l) >*/
	    sixs_aer__1.phase[l - 1] = sixs_coef__1.sc[j + (l << 2) - 5] * 
		    cij[j - 1] * dd[j + (l << 2) - 5] + sixs_aer__1.phase[l - 
		    1];
/*<           do 77 k=1,nbmu >*/
	    i__2 = nbmu;
	    for (k = 1; k <= i__2; ++k) {
/*<             phasel(l,k)=sc(j,l)*cij(j)*pha(j,l,k)+phasel(l,k) >*/
		sixs_phase__1.phasel[l + k * 20 - 21] = sixs_coef__1.sc[j + (
			l << 2) - 5] * cij[j - 1] * pha[j + (l + k * 20) * 5 
			- 106] + sixs_phase__1.phasel[l + k * 20 - 21];
/*<    77     continue >*/
/* L77: */
	    }
/*< 	  if (ipol.ne.0)then >*/
	    if (*ipol != 0) {
/*< 	    qhase(l)=sc(j,l)*cij(j)*qq(j,l)+qhase(l) >*/
		sixs_aer__1.qhase[l - 1] = sixs_coef__1.sc[j + (l << 2) - 5] *
			 cij[j - 1] * qq[j + (l << 2) - 5] + 
			sixs_aer__1.qhase[l - 1];
/*< 	    uhase(l)=sc(j,l)*cij(j)*uu(j,l)+uhase(l) >*/
		sixs_aer__1.uhase[l - 1] = sixs_coef__1.sc[j + (l << 2) - 5] *
			 cij[j - 1] * uu[j + (l << 2) - 5] + 
			sixs_aer__1.uhase[l - 1];
/*< 	    do k=1,nbmu >*/
		i__2 = nbmu;
		for (k = 1; k <= i__2; ++k) {
/*< 	      qhasel(l,k)=sc(j,l)*cij(j)*qha(j,l,k)+qhasel(l,k) >*/
		    sixs_phase__1.qhasel[l + k * 20 - 21] = sixs_coef__1.sc[j 
			    + (l << 2) - 5] * cij[j - 1] * qha[j + (l + k * 
			    20) * 5 - 106] + sixs_phase__1.qhasel[l + k * 20 
			    - 21];
/*< 	      uhasel(l,k)=sc(j,l)*cij(j)*uha(j,l,k)+uhasel(l,k) >*/
		    sixs_phase__1.uhasel[l + k * 20 - 21] = sixs_coef__1.sc[j 
			    + (l << 2) - 5] * cij[j - 1] * uha[j + (l + k * 
			    20) * 5 - 106] + sixs_phase__1.uhasel[l + k * 20 
			    - 21];
/*< 	    enddo >*/
		}
/*< 	  endif >*/
	    }
/*<     6   continue >*/
/* L6: */
	}
/*<         ome(l)=sca(l)/ext(l) >*/
	sixs_aer__1.ome[l - 1] = sca[l - 1] / sixs_aer__1.ext[l - 1];
/*<         gasym(l)=gasym(l)/sca(l) >*/
	sixs_aer__1.gasym[l - 1] /= sca[l - 1];
/*<         phase(l)=phase(l)/sca(l) >*/
	sixs_aer__1.phase[l - 1] /= sca[l - 1];
/*<         do 78 k=1,nbmu >*/
	i__1 = nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<           phasel(l,k)=phasel(l,k)/sca(l) >*/
	    sixs_phase__1.phasel[l + k * 20 - 21] /= sca[l - 1];
/*<    78   continue >*/
/* L78: */
	}
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	  qhase(l)=qhase(l)/sca(l) >*/
	    sixs_aer__1.qhase[l - 1] /= sca[l - 1];
/*< 	  uhase(l)=uhase(l)/sca(l) >*/
	    sixs_aer__1.uhase[l - 1] /= sca[l - 1];
/*< 	  do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<             qhasel(l,k)=qhasel(l,k)/sca(l) >*/
		sixs_phase__1.qhasel[l + k * 20 - 21] /= sca[l - 1];
/*<             uhasel(l,k)=uhasel(l,k)/sca(l) >*/
		sixs_phase__1.uhasel[l + k * 20 - 21] /= sca[l - 1];
/*< 	  enddo >*/
	    }
/*< 	endif >*/
	}
/*<         ext(l)=ext(l)*nis >*/
	sixs_aer__1.ext[l - 1] *= nis;
/*<         sca(l)=sca(l)*nis >*/
	sca[l - 1] *= nis;
/*<     5 continue >*/
/* L5: */
    }
/*<       if (iaer.ge.8.and.iaer.le.11) then >*/
    if (*iaer >= 8 && *iaer <= 11) {
/*<         open(10,file=FILE) >*/
	o__1.oerr = 0;
	o__1.ounit = 10;
	o__1.ofnmlen = 80;
	o__1.ofnm = file;
	o__1.orl = 0;
	o__1.osta = 0;
	o__1.oacc = 0;
	o__1.ofm = 0;
	o__1.oblnk = 0;
	f_open(&o__1);
/*< 	write(10,*) nbmu >*/
	s_wsle(&io___31);
	do_lio(&c__3, &c__1, (char *)&nbmu, (ftnlen)sizeof(integer));
	e_wsle();
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(3x,A5,1x,5(1x,A10,1x),1x,A10)";
	s_wsfe(&ci__1);
	do_fio(&c__1, "Wlgth", 5L);
	do_fio(&c__1, "Nor_Ext_Co", 10L);
	do_fio(&c__1, "Nor_Sca_Co", 10L);
	do_fio(&c__1, "Sg_Sca_Alb", 10L);
	do_fio(&c__1, "Asymm_Para", 10L);
	do_fio(&c__1, "Extinct_Co", 10L);
	do_fio(&c__1, "Scatter_Co", 10L);
	e_wsfe();
/*<         do 79 l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<    >*/
	    ci__1.cierr = 0;
	    ci__1.ciunit = 10;
	    ci__1.cifmt = "(2x,f8.4,4(3x,f8.4,3x),2(2x,e10.4))";
	    s_wsfe(&ci__1);
	    do_fio(&c__1, (char *)&wldis[l], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&sixs_aer__1.ext[l - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&sca[l - 1], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&sixs_aer__1.ome[l - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&sixs_aer__1.gasym[l - 1], (ftnlen)sizeof(
		    doublereal));
	    d__1 = sixs_aer__1.ext[l - 1] / nis;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    d__2 = sca[l - 1] / nis;
	    do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/*<  79     continue >*/
/* L79: */
	}
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(//,T20,A16,/,3x,A4,1x,20(3x,f6.4,2x))";
	s_wsfe(&ci__1);
	do_fio(&c__1, " Phase Function ", 16L);
	do_fio(&c__1, "TETA", 4L);
	for (l = 1; l <= 20; ++l) {
	    do_fio(&c__1, (char *)&wldis[l], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*<         do 76 k=1,nbmu >*/
	i__1 = nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<           write(10,761)180.*acos(cgaus_S(k))/pi,(phasel(l,k),l=1,20) >*/
	    s_wsfe(&io___32);
	    d__1 = acos(cgaus_s__[k - 1]) * 180. / pi;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    for (l = 1; l <= 20; ++l) {
		do_fio(&c__1, (char *)&sixs_phase__1.phasel[l + k * 20 - 21], 
			(ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
/*<  76     continue >*/
/* L76: */
	}
/*<  761    format (2x,f6.2,20(1x,e10.4)) >*/
/*< 	if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*< 	  do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<             write(10,761)180.*acos(cgaus_S(k))/pi,(qhasel(l,k),l=1,20) >*/
		s_wsfe(&io___33);
		d__1 = acos(cgaus_s__[k - 1]) * 180. / pi;
		do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
		for (l = 1; l <= 20; ++l) {
		    do_fio(&c__1, (char *)&sixs_phase__1.qhasel[l + k * 20 - 
			    21], (ftnlen)sizeof(doublereal));
		}
		e_wsfe();
/*< 	  enddo >*/
	    }
/*< 	  do k=1,nbmu >*/
	    i__1 = nbmu;
	    for (k = 1; k <= i__1; ++k) {
/*<             write(10,761)180.*acos(cgaus_S(k))/pi,(uhasel(l,k),l=1,20) >*/
		s_wsfe(&io___34);
		d__1 = acos(cgaus_s__[k - 1]) * 180. / pi;
		do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
		for (l = 1; l <= 20; ++l) {
		    do_fio(&c__1, (char *)&sixs_phase__1.uhasel[l + k * 20 - 
			    21], (ftnlen)sizeof(doublereal));
		}
		e_wsfe();
/*< 	  enddo >*/
	    }
/*< 	endif >*/
	}
/*<         close(10) >*/
	cl__1.cerr = 0;
	cl__1.cunit = 10;
	cl__1.csta = 0;
	f_clos(&cl__1);
/*<       endif >*/
    }
/*<  777  continue       >*/
L777:
/*<       return >*/
    return 0;
/*<       end >*/
} /* aeroso_ */

#ifdef __cplusplus
	}
#endif
