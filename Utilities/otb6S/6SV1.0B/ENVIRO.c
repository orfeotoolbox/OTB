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
/* Subroutine */ int enviro_(real *difr, real *difa, real *r__, real *palt, 
	real *xmuv, real *fra, real *fae, real *fr)
{
    /* Initialized data */

    static real alt[16] = { (float).5,(float)1.,(float)2.,(float)3.,(float)4.,
	    (float)5.,(float)6.,(float)7.,(float)8.,(float)10.,(float)12.,(
	    float)14.,(float)16.,(float)18.,(float)20.,(float)60. };
    static real cfr1[16] = { (float).73,(float).71,(float).656,(float).606,(
	    float).56,(float).516,(float).473,(float).433,(float).395,(float)
	    .323,(float).258,(float).209,(float).171,(float).142,(float).122,(
	    float).07 };
    static real cfr2[16] = { (float)2.8,(float)1.51,(float).845,(float).634,(
	    float).524,(float).465,(float).429,(float).405,(float).39,(float)
	    .386,(float).409,(float).445,(float).488,(float).545,(float).608,(
	    float).868 };
    static real cfa1[16] = { (float).239,(float).396,(float).588,(float).626,(
	    float).612,(float).505,(float).454,(float).448,(float).444,(float)
	    .445,(float).444,(float).448,(float).448,(float).448,(float).448,(
	    float).448 };
    static real cfa2[16] = { (float)1.4,(float)1.2,(float)1.02,(float).86,(
	    float).74,(float).56,(float).46,(float).42,(float).38,(float).34,(
	    float).3,(float).28,(float).27,(float).27,(float).27,(float).27 };
    static real cfa3[16] = { (float)9.17,(float)6.26,(float)5.48,(float)5.16,(
	    float)4.74,(float)3.65,(float)3.24,(float)3.15,(float)3.07,(float)
	    2.97,(float)2.88,(float)2.83,(float)2.83,(float)2.83,(float)2.83,(
	    float)2.83 };

    /* Builtin functions */
    double exp(doublereal), log(doublereal);

    /* Local variables */
    integer i__;
    real a0, b0, a1, b1, fae0, fra0, zmin, zmax, xlnv, xcfa1, xcfa2, xcfa3, 
	    xcfr1, xcfr2;

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
    a0 = (float)1.3347;
/*<       b0=0.57757 >*/
    b0 = (float).57757;
/*<       a1=-1.479 >*/
    a1 = (float)-1.479;
/*<       b1=-1.5275 >*/
    b1 = (float)-1.5275;
/*<       if (palt.ge.60.) then >*/
    if (*palt >= (float)60.) {
/*<       fae0=1-0.448*exp(-r*0.27)-0.552*exp(-r*2.83) >*/
	fae0 = 1 - exp(-(*r__) * (float).27) * (float).448 - exp(-(*r__) * (
		float)2.83) * (float).552;
/*<       fra0=1-0.930*exp(-r*0.080)-0.070*exp(-r*1.100) >*/
	fra0 = 1 - exp(-(*r__) * (float).08) * (float).93 - exp(-(*r__) * (
		float)1.1) * (float).07;
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
	fra0 = (float)1. - xcfr1 * exp(-(*r__) * xcfr2) - ((float)1. - xcfr1) 
		* exp(-(*r__) * (float).08);
/*<       fae0=1.-xcfa1*exp(-r*xcfa2)-(1.-xcfa1)*exp(-r*xcfa3) >*/
	fae0 = (float)1. - xcfa1 * exp(-(*r__) * xcfa2) - ((float)1. - xcfa1) 
		* exp(-(*r__) * xcfa3);
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
    if (*difa + *difr > (float).001) {
/*<          fr=(fae*difa+fra*difr)/(difa+difr) >*/
	*fr = (*fae * *difa + *fra * *difr) / (*difa + *difr);
/*<          else >*/
    } else {
/*<          fr=1. >*/
	*fr = (float)1.;
/*<          endif >*/
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* enviro_ */

#ifdef __cplusplus
	}
#endif
