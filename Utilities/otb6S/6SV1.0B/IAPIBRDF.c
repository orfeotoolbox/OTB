/* IAPIBRDF.f -- translated by f2c (version 19970805).
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
static doublereal c_b52 = -1.;
static doublereal c_b53 = 1.;

/*<    >*/
/* Subroutine */ int iapibrdf_(integer *pild, doublereal *pxlt, doublereal *
	prl, doublereal *ptl, doublereal *prs, integer *pihs, doublereal *pc, 
	integer *mu, integer *np, doublereal *rm, doublereal *rp, doublereal *
	brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    /* Subroutine */ int s_stop(char *, ftnlen);
    double acos(doublereal);

    /* Local variables */
    integer j, k;
    doublereal fi, mu1, mu2;
    extern /* Subroutine */ int lad_();
    integer ihs;
    extern doublereal ro_1__(doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal phi_i__, phi_v__;
    extern /* Subroutine */ int solve_(doublereal *), gauleg_(doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *);
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


/*<       integer np,mu >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real ro_1 >*/
/*<       integer iwr,k,j >*/
/*<       integer      pild,pihs >*/
/*<       real         pxlt,prl,ptl,prs,pc >*/
/*<       logical ier >*/
/*<       common/sixs_ier/iwr,ier >*/

/*<       real mu1,mu2,fi >*/
/*<       real pi >*/
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
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
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

/*<         if (ihs.eq.0) c=0. >*/
    if (ihs == 0) {
	p_1.c__ = 0.;
    }

/*<       mu1=rm(0) >*/
    mu1 = rm[0];
/*<       Theta_i=acos(mu1) >*/
    theta_i__ = acos(mu1);
/*<       Theta_i=Pi-Theta_i >*/
    theta_i__ = 3.141592653589793 - theta_i__;


/* - Gauss's quadrature (n points) */

/*<         n=10 >*/
    gauss_m__1.n = 10;
/*<         call gauleg (-1.,1.,xgm,wgm,n) >*/
    gauleg_(&c_b52, &c_b53, gauss_m__1.xgm, gauss_m__1.wgm, &gauss_m__1.n);

/* - Computation of the multiple scattering (Ro_mult) */

/*<         call solve (Theta_i) >*/
    solve_(&theta_i__);

/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       mu2=rm(j) >*/
	    mu2 = rm[j];
/*<       if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<          fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<          else >*/
	    } else {
/*<          fi=rp(k)+rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<       endif >*/
	    }
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
/*<       brdfint(j,k)=Ro_1(Theta_i,Phi_i,Theta_v,Phi_v)+Ro_mult >*/
	    brdfint[j + k * brdfint_dim1] = ro_1__(&theta_i__, &phi_i__, &
		    theta_v__, &phi_v__) + ro_1.ro_mult__;
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* iapibrdf_ */

#ifdef __cplusplus
	}
#endif
