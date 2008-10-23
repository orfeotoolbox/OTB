/* ENVIRO.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int enviro_(doublereal *difr, doublereal *difa, doublereal *
	r__, doublereal *palt, doublereal *xmuv, doublereal *fra, doublereal *
	fae, doublereal *fr)
{
    /* Initialized data */

    static doublereal alt[16] = { .5,1.,2.,3.,4.,5.,6.,7.,8.,10.,12.,14.,16.,
	    18.,20.,60. };
    static doublereal cfr1[16] = { .73,.71,.656,.606,.56,.516,.473,.433,.395,
	    .323,.258,.209,.171,.142,.122,.07 };
    static doublereal cfr2[16] = { 2.8,1.51,.845,.634,.524,.465,.429,.405,.39,
	    .386,.409,.445,.488,.545,.608,.868 };
    static doublereal cfa1[16] = { .239,.396,.588,.626,.612,.505,.454,.448,
	    .444,.445,.444,.448,.448,.448,.448,.448 };
    static doublereal cfa2[16] = { 1.4,1.2,1.02,.86,.74,.56,.46,.42,.38,.34,
	    .3,.28,.27,.27,.27,.27 };
    static doublereal cfa3[16] = { 9.17,6.26,5.48,5.16,4.74,3.65,3.24,3.15,
	    3.07,2.97,2.88,2.83,2.83,2.83,2.83,2.83 };

    /* Builtin functions */
    double exp(doublereal), log(doublereal);

    /* Local variables */
    integer i__;
    doublereal a0, b0, a1, b1, fae0, fra0, zmin, zmax, xlnv, xcfa1, xcfa2, 
	    xcfa3, xcfr1, xcfr2;

/*<       real difr, difa, r, palt >*/
/*<       real fae,fra,fr,fae0,fra0,xmuv,xlnv,a0,b0,a1,b1 >*/
/*<       real zmin,zmax,xcfr1,xcfr2,xcfa1,xcfa2,xcfa3 >*/
/*<       real alt(16),cfr1(16),cfr2(16),cfa1(16),cfa2(16),cfa3(16) >*/
/*<       integer i >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/

/*     calculation of the environmental function for */
/*     rayleigh and aerosols contribution. */

/*     this calculation have been done for nadir observation */
/*     and are corrected of the effect of the view zenith angle. */

/*<       a0=1.3347 >*/
    a0 = 1.3347;
/*<       b0=0.57757 >*/
    b0 = .57757;
/*<       a1=-1.479 >*/
    a1 = -1.479;
/*<       b1=-1.5275 >*/
    b1 = -1.5275;
/*<       if (palt.ge.60.) then >*/
    if (*palt >= 60.) {
/*<       fae0=1-0.448*exp(-r*0.27)-0.552*exp(-r*2.83) >*/
	fae0 = 1 - exp(-(*r__) * .27) * .448 - exp(-(*r__) * 2.83) * .552;
/*<       fra0=1-0.930*exp(-r*0.080)-0.070*exp(-r*1.100) >*/
	fra0 = 1 - exp(-(*r__) * .08) * .93 - exp(-(*r__) * 1.1) * .07;
/*<       else >*/
    } else {
/*<       i=0 >*/
	i__ = 0;
/*<  10   i=i+1 >*/
L10:
	++i__;
/*<       if (palt.ge.alt(i)) goto 10 >*/
	if (*palt >= alt[i__ - 1]) {
	    goto L10;
	}
/*<       if ((i.gt.1).and.(i.le.16)) then >*/
	if (i__ > 1 && i__ <= 16) {
/*<          zmin=alt(i-1) >*/
	    zmin = alt[i__ - 2];
/*<          zmax=alt(i) >*/
	    zmax = alt[i__ - 1];
/*<          xcfr1=cfr1(i-1)+(cfr1(i)-cfr1(i-1))*(palt-zmin)/(zmax-zmin) >*/
	    xcfr1 = cfr1[i__ - 2] + (cfr1[i__ - 1] - cfr1[i__ - 2]) * (*palt 
		    - zmin) / (zmax - zmin);
/*<          xcfr2=cfr2(i-1)+(cfr2(i)-cfr2(i-1))*(palt-zmin)/(zmax-zmin) >*/
	    xcfr2 = cfr2[i__ - 2] + (cfr2[i__ - 1] - cfr2[i__ - 2]) * (*palt 
		    - zmin) / (zmax - zmin);
/*<          xcfa1=cfa1(i-1)+(cfa1(i)-cfa1(i-1))*(palt-zmin)/(zmax-zmin) >*/
	    xcfa1 = cfa1[i__ - 2] + (cfa1[i__ - 1] - cfa1[i__ - 2]) * (*palt 
		    - zmin) / (zmax - zmin);
/*<          xcfa2=cfa2(i-1)+(cfa2(i)-cfa2(i-1))*(palt-zmin)/(zmax-zmin) >*/
	    xcfa2 = cfa2[i__ - 2] + (cfa2[i__ - 1] - cfa2[i__ - 2]) * (*palt 
		    - zmin) / (zmax - zmin);
/*<          xcfa3=cfa3(i-1)+(cfa3(i)-cfa3(i-1))*(palt-zmin)/(zmax-zmin) >*/
	    xcfa3 = cfa3[i__ - 2] + (cfa3[i__ - 1] - cfa3[i__ - 2]) * (*palt 
		    - zmin) / (zmax - zmin);
/*<          endif >*/
	}
/*<       if (i.eq.1) then >*/
	if (i__ == 1) {
/*<          xcfr1=cfr1(1) >*/
	    xcfr1 = cfr1[0];
/*<          xcfr2=cfr2(1) >*/
	    xcfr2 = cfr2[0];
/*<          xcfa1=cfa1(1) >*/
	    xcfa1 = cfa1[0];
/*<          xcfa2=cfa2(1) >*/
	    xcfa2 = cfa2[0];
/*<          xcfa3=cfa3(1) >*/
	    xcfa3 = cfa3[0];
/*<          endif >*/
	}
/*<       fra0=1.-xcfr1*exp(-r*xcfr2)-(1.-xcfr1)*exp(-r*0.08) >*/
	fra0 = 1. - xcfr1 * exp(-(*r__) * xcfr2) - (1. - xcfr1) * exp(-(*r__) 
		* .08);
/*<       fae0=1.-xcfa1*exp(-r*xcfa2)-(1.-xcfa1)*exp(-r*xcfa3) >*/
	fae0 = 1. - xcfa1 * exp(-(*r__) * xcfa2) - (1. - xcfa1) * exp(-(*r__) 
		* xcfa3);
/*<       endif >*/
    }
/* correction of the effect of the view zenith angle */
/*<       xlnv=log(xmuv) >*/
    xlnv = log(*xmuv);
/*<       fra=fra0*(xlnv*(1-fra0)+1) >*/
    *fra = fra0 * (xlnv * (1 - fra0) + 1);
/*<    >*/
    *fae = fae0 * (a0 * xlnv + 1 + b0 * xlnv * xlnv + fae0 * (a1 * xlnv + b1 *
	     xlnv * xlnv) + fae0 * fae0 * ((-a1 - a0) * xlnv + (-b1 - b0) * 
	    xlnv * xlnv));

/*<       if ((difa+difr).gt.1.E-03) then >*/
    if (*difa + *difr > .001) {
/*<          fr=(fae*difa+fra*difr)/(difa+difr) >*/
	*fr = (*fae * *difa + *fra * *difr) / (*difa + *difr);
/*<          else >*/
    } else {
/*<          fr=1. >*/
	*fr = 1.;
/*<          endif >*/
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* enviro_ */

#ifdef __cplusplus
	}
#endif
