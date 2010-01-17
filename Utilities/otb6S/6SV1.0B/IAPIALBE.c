/* IAPIALBE.f -- translated by f2c (version 19970805).
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
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

Extern struct {
    doublereal xgm[20], wgm[20];
    integer n;
} gauss_m__;

#define gauss_m__1 gauss_m__

Extern struct {
    doublereal xlt, rl, tl, rs, c__;
    integer ild;
} p_;

#define p_1 p_

Extern struct {
    doublereal a_ld__, b_ld__, c_ld__, d_ld__;
} ld_;

#define ld_1 ld_

Extern struct {
    doublereal ro_1_c__, ro_1_s__, ro_mult__;
} ro_;

#define ro_1 ro_

/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static integer c__24 = 24;
static integer c__48 = 48;
static doublereal c_b54 = -1.;
static doublereal c_b55 = 1.;

/*<    >*/
/* Subroutine */ int iapialbe_(integer *pild, doublereal *pxlt, doublereal *
	prl, doublereal *ptl, doublereal *prs, integer *pihs, doublereal *pc, 
	doublereal *brdfalb)
{
    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    /* Subroutine */ int s_stop(char *, ftnlen);
    double cos(doublereal), sin(doublereal), acos(doublereal);

    /* Local variables */
    integer j, k, l;
    doublereal y, fa[48], fi, ta[24], si1, si2, mu1, mu2;
    extern /* Subroutine */ int lad_();
    doublereal wfa[48];
    integer ihs;
    doublereal wta[24];
    extern doublereal ro_1__(doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal phi1, phi2, pond, summ, teta1, teta2, phi_i__, phi_v__;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *), solve_(doublereal *), gauleg_(
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    ;
    doublereal theta_i__, theta_v__;

    /* Fortran I/O blocks */
    static cilist io___2 = { 0, 6, 0, 0, 0 };
    static cilist io___3 = { 0, 6, 0, 0, 0 };
    static cilist io___4 = { 0, 6, 0, 0, 0 };
    static cilist io___5 = { 0, 6, 0, 0, 0 };
    static cilist io___6 = { 0, 6, 0, 0, 0 };
    static cilist io___7 = { 0, 6, 0, 0, 0 };
    static cilist io___8 = { 0, 6, 0, 0, 0 };
    static cilist io___9 = { 0, 6, 0, 0, 0 };
    static cilist io___10 = { 0, 6, 0, 0, 0 };
    static cilist io___11 = { 0, 6, 0, 0, 0 };
    static cilist io___12 = { 0, 6, 0, 0, 0 };
    static cilist io___13 = { 0, 6, 0, 0, 0 };
    static cilist io___14 = { 0, 6, 0, 0, 0 };



/* interface between the computer code of the model of Iaquinta and Pinty 
*/
/* the computer code is courtesy of Jean Ianquinta */
/* see module IAPITOOLS.f for a complete description */


/*<       parameter (nta=24,nfa=48) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real mu1,mu2,fi >*/
/*<       real pi,y >*/
/*<       real brdfalb,summ,si1,si2,ro_1,pond >*/
/*<       integer iwr,l,k,j >*/
/*<       integer      pild,pihs >*/
/*<       real         pxlt,prl,ptl,prs,pc >*/
/*<       logical ier >*/
/*<       common/sixs_ier/iwr,ier >*/
/* begin of Iaquinta and Pinty model parameter and declaration */
/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /gauss_m/xgm (20),wgm (20),n >*/
/*<         real xgm,wgm >*/
/*<         integer n >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c >*/
/*<         integer ild >*/
/*<         common /ld/a_ld,b_ld,c_ld,d_ld >*/
/*<         real a_ld,b_ld,c_ld,d_ld >*/
/*<         common /Ro/Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Theta_i,Phi_i >*/
/*<         real Theta_v,Phi_v >*/
/*<         integer ihs >*/

/*       xLt =  leaf area index */
/*       Rl = leaf reflection coefficient (Bi-Lambertian) */
/*       Tl = leaf transmission coefficient (Bi-Lambertian) */
/*       ild = leaf angle distribution : */
/*                                       1 = planophile */
/*                                       2 = erectophile */
/*                                       3 = plagiophile */
/*                                       4 = extremophile */
/*                                       5 = uniform */
/*       Rs = soil albedo */
/*       c = 2*r*Lambda */

/*       Ro_1_c  = single scattering by the canopy term */
/*       Ro_1_s  = uncollided by the leaves (or singly scattered by */
/*                 the soil) radiation */
/*                (Ro_1 = Ro_1_c + Ro_1_s) */
/*       Ro_mult = multiple scattering */
/* transfer paramater to common / / parameter struture */
/*<          ild=pild >*/
    p_1.ild = *pild;
/*<          Xlt=pXlt >*/
    p_1.xlt = *pxlt;
/*<          Rl=pRl >*/
    p_1.rl = *prl;
/*<          Tl=pTl >*/
    p_1.tl = *ptl;
/*<          Rs=pRs >*/
    p_1.rs = *prs;
/*<          ihs=pihs >*/
    ihs = *pihs;
/*<          c=pc >*/
    p_1.c__ = *pc;

/* Check parameter validity */
/*<    >*/
    if (p_1.ild != 1 && p_1.ild != 2 && p_1.ild != 3 && p_1.ild != 4 && 
	    p_1.ild != 5) {
/*<               print*,'Leaf angle distribution !' >*/
	s_wsle(&io___2);
	do_lio(&c__9, &c__1, "Leaf angle distribution !", 25L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (xlt.le.0.) then >*/
    if (p_1.xlt <= 0.) {
/*<               print*,'Leaf area index < 0. !' >*/
	s_wsle(&io___3);
	do_lio(&c__9, &c__1, "Leaf area index < 0. !", 22L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (xlt.lt.1.) then >*/
    if (p_1.xlt < 1.) {
/*<               print*,'Leaf area index < 1. !' >*/
	s_wsle(&io___4);
	do_lio(&c__9, &c__1, "Leaf area index < 1. !", 22L);
	e_wsle();
/*<             endif >*/
    }
/*<             if (xlt.gt.15.) then >*/
    if (p_1.xlt > 15.) {
/*<               print*,'Leaf area index > 15. !' >*/
	s_wsle(&io___5);
	do_lio(&c__9, &c__1, "Leaf area index > 15. !", 23L);
	e_wsle();
/*<             endif >*/
    }
/*<             if (Rl.lt.0.) then >*/
    if (p_1.rl < 0.) {
/*<               print*,'Leaf reflectance < 0. !' >*/
	s_wsle(&io___6);
	do_lio(&c__9, &c__1, "Leaf reflectance < 0. !", 23L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Rl.gt..99) then >*/
    if (p_1.rl > .99) {
/*<               print*,'Leaf reflectance > .99 !' >*/
	s_wsle(&io___7);
	do_lio(&c__9, &c__1, "Leaf reflectance > .99 !", 24L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Tl.lt.0.) then >*/
    if (p_1.tl < 0.) {
/*<               print*,'Leaf transmittance < 0. !' >*/
	s_wsle(&io___8);
	do_lio(&c__9, &c__1, "Leaf transmittance < 0. !", 25L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Tl.gt..99) then >*/
    if (p_1.tl > .99) {
/*<               print*,'Leaf transmittance > .99 !' >*/
	s_wsle(&io___9);
	do_lio(&c__9, &c__1, "Leaf transmittance > .99 !", 26L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Rl+Tl.gt..99) then >*/
    if (p_1.rl + p_1.tl > .99) {
/*<               print*,'Single scattering albedo > .99 !' >*/
	s_wsle(&io___10);
	do_lio(&c__9, &c__1, "Single scattering albedo > .99 !", 32L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Rs.lt.0.) then >*/
    if (p_1.rs < 0.) {
/*<               print*,'Soil albedo < 0. !' >*/
	s_wsle(&io___11);
	do_lio(&c__9, &c__1, "Soil albedo < 0. !", 18L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (Rs.gt..99) then >*/
    if (p_1.rs > .99) {
/*<               print*,'Soil albedo > .99 !' >*/
	s_wsle(&io___12);
	do_lio(&c__9, &c__1, "Soil albedo > .99 !", 19L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (c.lt.0.) then >*/
    if (p_1.c__ < 0.) {
/*<               print*,'Hot-spot parameter < 0. !' >*/
	s_wsle(&io___13);
	do_lio(&c__9, &c__1, "Hot-spot parameter < 0. !", 25L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/*<             if (c.gt.2.) then >*/
    if (p_1.c__ > 2.) {
/*<               print*,'Hot-spot parameter > 2. !' >*/
	s_wsle(&io___14);
	do_lio(&c__9, &c__1, "Hot-spot parameter > 2. !", 25L);
	e_wsle();
/*<               stop >*/
	s_stop("", 0L);
/*<             endif >*/
    }
/* compute leaf area angle distribution */
/*<       call lad >*/
    lad_();

/* - Hot-spot parameter */

/*<         if (ihs.eq.0) c=1.e-20 >*/
    if (ihs == 0) {
	p_1.c__ = 1e-20;
    }

/*<       teta1=0. >*/
    teta1 = 0.;
/*<       teta2=pi/2. >*/
    teta2 = 1.5707963267948966;
/*<       call gauss(teta1,teta2,ta,wta,nta) >*/
    gauss_(&teta1, &teta2, ta, wta, &c__24);
/*<       phi1=0. >*/
    phi1 = 0.;
/*<       phi2=2.*pi >*/
    phi2 = 6.2831853071795862;
/*<       call gauss(phi1,phi2,fa,wfa,nfa) >*/
    gauss_(&phi1, &phi2, fa, wfa, &c__48);
/*<       brdfalb=0. >*/
    *brdfalb = 0.;
/*<       summ=0. >*/
    summ = 0.;
/* - Gauss's quadrature (n points) */

/*<         n=10 >*/
    gauss_m__1.n = 10;
/*<         call gauleg (-1.,1.,xgm,wgm,n) >*/
    gauleg_(&c_b54, &c_b55, gauss_m__1.xgm, gauss_m__1.wgm, &gauss_m__1.n);

/*<       do 3 l=1,nta >*/
    for (l = 1; l <= 24; ++l) {
/*<       mu1=cos(ta(l)) >*/
	mu1 = cos(ta[l - 1]);
/*<       si1=sin(ta(l)) >*/
	si1 = sin(ta[l - 1]);
/*<       Theta_i=acos(mu1) >*/
	theta_i__ = acos(mu1);
/*<       Theta_i=Pi-Theta_i >*/
	theta_i__ = 3.141592653589793 - theta_i__;
/* - Computation of the multiple scattering (Ro_mult) */

/*<         call solve (Theta_i) >*/
	solve_(&theta_i__);

/*<       do 1 k=1,nfa >*/
	for (k = 1; k <= 48; ++k) {
/*<       do 2 j=1,nta >*/
	    for (j = 1; j <= 24; ++j) {
/*<       mu2=cos(ta(j)) >*/
		mu2 = cos(ta[j - 1]);
/*<       si2=sin(ta(j)) >*/
		si2 = sin(ta[j - 1]);
/*<       fi=fa(k) >*/
		fi = fa[k - 1];
/*<       Theta_v=acos(mu2) >*/
		theta_v__ = acos(mu2);
/*<       if (fi.lt.0.) fi=fi+2.*pi >*/
		if (fi < 0.) {
		    fi += 6.2831853071795862;
		}
/*<       if (fi.gt.(2.*pi)) fi=fi-2.*pi >*/
		if (fi > 6.2831853071795862) {
		    fi += -6.2831853071795862;
		}
/*<       Phi_i=fi >*/
		phi_i__ = fi;
/*<       Phi_v=0. >*/
		phi_v__ = 0.;
/*<       y=Ro_1(Theta_i,Phi_i,Theta_v,Phi_v)+Ro_mult >*/
		y = ro_1__(&theta_i__, &phi_i__, &theta_v__, &phi_v__) + 
			ro_1.ro_mult__;
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       brdfalb=brdfalb+pond*y >*/
		*brdfalb += pond * y;
/*<       summ=summ+pond >*/
		summ += pond;
/*<   2   continue >*/
/* L2: */
	    }
/*<   1   continue >*/
/* L1: */
	}
/*<   3   continue >*/
/* L3: */
    }
/*<       brdfalb=brdfalb/summ >*/
    *brdfalb /= summ;
/*<       return >*/
    return 0;
/*<       end >*/
} /* iapialbe_ */

#ifdef __cplusplus
	}
#endif
