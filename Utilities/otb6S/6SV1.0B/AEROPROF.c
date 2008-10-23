/* AEROPROF.f -- translated by f2c (version 19970805).
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
    integer num_z__;
    doublereal alt_z__[101], taer_z__[101], taer55_z__[101];
} aeroprof_;

#define aeroprof_1 aeroprof_

/*<    >*/
/* Subroutine */ int aero_prof__(doublereal *ta, doublereal *piz, doublereal *
	tr, doublereal *hr, integer *nt, doublereal *xmus, doublereal *h__, 
	doublereal *ch, doublereal *ydel, doublereal *xdel, doublereal *altc)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    doublereal dtau_aer__, dtau_ray__;
    integer i__, j, n;
    doublereal z__, dz, dtau, z_up__, ssa_aer__, dtau_os__;

/*<       include "paramdef.inc" >*/
/*<       double precision xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt) >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       double precision altc(0:nt),ta,piz,tr,hr,xmus >*/
/*<       double precision dz,z_up,dtau_ray,dtau_aer,dtau,dtau_OS >*/
/*<       real alt_z,taer_z,taer55_z,ssa_aer					  >*/
/*<       integer j,i,nt,num_z >*/
/*<    >*/
/* If the maximum aerosol height is less than 300 km, one additional */
/* layer is added above with the aerosol optical thickness equal to 0. */
/*<       if (alt_z(0).lt.300) then >*/
    if (aeroprof_1.alt_z__[0] < 300.) {
/*<        taer_z(0)=0.0 >*/
	aeroprof_1.taer_z__[0] = 0.;
/*<        num_z=num_z+1 >*/
	++aeroprof_1.num_z__;
/*<        do i=0,num_z-1 >*/
	i__1 = aeroprof_1.num_z__ - 1;
	for (i__ = 0; i__ <= i__1; ++i__) {
/*<         alt_z(num_z-i)=alt_z(num_z-i-1) >*/
	    aeroprof_1.alt_z__[aeroprof_1.num_z__ - i__] = aeroprof_1.alt_z__[
		    aeroprof_1.num_z__ - i__ - 1];
/*<         taer_z(num_z-i)=taer_z(num_z-i-1) >*/
	    aeroprof_1.taer_z__[aeroprof_1.num_z__ - i__] = 
		    aeroprof_1.taer_z__[aeroprof_1.num_z__ - i__ - 1];
/*<        enddo >*/
	}
/*<       endif >*/
    }
/*<       alt_z(0)=300 >*/
    aeroprof_1.alt_z__[0] = 300.;
/*<       ssa_aer=piz >*/
    ssa_aer__ = *piz;
/* The atmosphere is divided into nt layers with the same */
/* (molecular + aerosol) optical thickness. */
/*<       dtau_OS=(tr+ta)/nt >*/
    dtau_os__ = (*tr + *ta) / *nt;
/*<       i=0 >*/
    i__ = 0;
/*<       dz=0.0001 >*/
    dz = 1e-4;
/*<       h(0)=0.0 >*/
    h__[0] = 0.;
/*<       altc(0)=300.0 >*/
    altc[0] = 300.;
/*<       z_up=alt_z(0) >*/
    z_up__ = aeroprof_1.alt_z__[0];
/*<       ch(0)=0.5 >*/
    ch[0] = .5;
/*<       ydel(0)=1.0 >*/
    ydel[0] = 1.;
/*<       xdel(0)=0.0 >*/
    xdel[0] = 0.;
/*<       j=1 >*/
    j = 1;
/*<       n=1 >*/
    n = 1;
/*<       dtau_aer=0.0 >*/
    dtau_aer__ = 0.;
/*<  11   i=i+1 >*/
L11:
    ++i__;
/*<       z=alt_z(0)-dz*i >*/
    z__ = aeroprof_1.alt_z__[0] - dz * i__;
/*<       dtau_ray=tr*(exp(-z/hr)-exp(-z_up/hr)) >*/
    dtau_ray__ = *tr * (exp(-z__ / *hr) - exp(-z_up__ / *hr));
/*<       dtau_aer=dtau_aer+taer_z(n)*dz/(alt_z(n-1)-alt_z(n)) >*/
    dtau_aer__ += aeroprof_1.taer_z__[n] * dz / (aeroprof_1.alt_z__[n - 1] - 
	    aeroprof_1.alt_z__[n]);
/*<       if (z.lt.alt_z(n)) n=n+1 >*/
    if (z__ < aeroprof_1.alt_z__[n]) {
	++n;
    }
/*<       dtau=dtau_ray+dtau_aer >*/
    dtau = dtau_ray__ + dtau_aer__;
/*<       if (dtau.ge.dtau_OS) then >*/
    if (dtau >= dtau_os__) {
/*<         altc(j)=z >*/
	altc[j] = z__;
/*<         h(j)=h(j-1)+dtau >*/
	h__[j] = h__[j - 1] + dtau;
/*<         ch(j)=exp(-h(j)/xmus)/2 >*/
	ch[j] = exp(-h__[j] / *xmus) / 2;
/*<         xdel(j)=dtau_aer*ssa_aer/dtau ! aerosol portion in the j-th laye >*/
	xdel[j] = dtau_aer__ * ssa_aer__ / dtau;
/*<         ydel(j)=dtau_ray/dtau         ! molecular portion in the j-th la >*/
	ydel[j] = dtau_ray__ / dtau;
/*        write(6,*)j,z,dtau_ray,dtau_aer,dtau,(ta+tr)/nt */
/*<         j=j+1 >*/
	++j;
/*<         z_up=z >*/
	z_up__ = z__;
/*<         dtau_aer=0.0 >*/
	dtau_aer__ = 0.;
/*<       endif >*/
    }
/*<       if(z.gt.0) goto 11 >*/
    if (z__ > 0.) {
	goto L11;
    }
/*<        altc(nt)=0 >*/
    altc[*nt] = 0.;
/*<        h(nt)=tr+ta >*/
    h__[*nt] = *tr + *ta;
/*<        ch(nt)=exp(-h(nt)/xmus)/2 >*/
    ch[*nt] = exp(-h__[*nt] / *xmus) / 2;
/*<        xdel(nt)=dtau_aer*ssa_aer/dtau >*/
    xdel[*nt] = dtau_aer__ * ssa_aer__ / dtau;
/*<        ydel(nt)=dtau_ray/dtau >*/
    ydel[*nt] = dtau_ray__ / dtau;
/*  checking */
/*       do j=0,nt */
/*      write(6,*)j,altc(j),h(j),h(j)-h(j-1) */
/*       enddo */
/*<       return >*/
    return 0;
/*<       end >*/
} /* aero_prof__ */

#ifdef __cplusplus
	}
#endif
