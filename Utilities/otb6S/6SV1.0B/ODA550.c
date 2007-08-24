/* ODA550.f -- translated by f2c (version 19970805).
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
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

Extern struct {
    real delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

/*<    >*/
/* Subroutine */ int oda550_(integer *iaer, real *v, real *taer55)
{
    /* Initialized data */

    static real an23[34] = { (float)2828.,(float)1244.,(float)537.1,(float)
	    225.6,(float)119.2,(float)89.87,(float)63.37,(float)58.9,(float)
	    60.69,(float)58.18,(float)56.75,(float)53.17,(float)55.85,(float)
	    51.56,(float)50.48,(float)47.44,(float)45.11,(float)44.58,(float)
	    43.14,(float)36.34,(float)26.67,(float)19.33,(float)14.55,(float)
	    11.13,(float)8.826,(float)7.429,(float)2.238,(float).589,(float)
	    .155,(float).04082,(float).01078,(float)5.55e-5,(float)1.969e-8,(
	    float)0. };
    static real an5[34] = { (float)13780.,(float)5030.,(float)1844.,(float)
	    673.1,(float)245.3,(float)89.87,(float)63.37,(float)58.9,(float)
	    60.69,(float)58.18,(float)56.75,(float)53.17,(float)55.85,(float)
	    51.56,(float)50.48,(float)47.44,(float)45.11,(float)44.58,(float)
	    43.14,(float)36.34,(float)26.67,(float)19.33,(float)14.55,(float)
	    11.13,(float)8.826,(float)7.429,(float)2.238,(float).589,(float)
	    .155,(float).04082,(float).01078,(float)5.55e-5,(float)1.969e-8,(
	    float)0. };

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    integer k;
    real az, bz, ev, dz, bn5, az1, bz1, bn23, bn51;
    doublereal bnz;
    real bn231;
    doublereal bnz1;

/*<       double precision bnz,bnz1 >*/
/*<       common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       common /sixs_del/ delta,sigma >*/
/*<       real an5(34),an23(34) >*/
/*<       Real v,taer55,z,p,t,wh >*/
/*<       Real wo,delta,sigma,dz,bn5,bn51,bn23,bn231,az >*/
/*<       Real az1,bz,bz1,ev >*/
/*<       Integer iaer,k >*/
/*    aerosol optical depth at wl=550nm */
/*     vertical repartition of aerosol density for v=23km */
/*                     ( in nbr of part/cm3 ) */
/*<    >*/
/*     vertical repartition of aerosol density for v=5km */
/*                     ( in nbr of part/cm3 ) */
/*<    >*/
/*<       taer55=0. >*/
    *taer55 = (float)0.;
/*<       if(abs(v).le.0.) return >*/
    if (dabs(*v) <= (float)0.) {
	return 0;
    }
/*<       if(iaer.eq.0) return >*/
    if (*iaer == 0) {
	return 0;
    }
/*<       do 1 k=1,32 >*/
    for (k = 1; k <= 32; ++k) {
/*<       dz=z(k+1)-z(k) >*/
	dz = sixs_atm__1.z__[k] - sixs_atm__1.z__[k - 1];
/*<       bn5=an5(k) >*/
	bn5 = an5[k - 1];
/*<       bn51=an5(k+1) >*/
	bn51 = an5[k];
/*<       bn23=an23(k) >*/
	bn23 = an23[k - 1];
/*<       bn231=an23(k+1) >*/
	bn231 = an23[k];
/*<       az=(115./18.)*(bn5-bn23) >*/
	az = (bn5 - bn23) * (float)6.3888888888888893;
/*<       az1=(115./18.)*(bn51-bn231) >*/
	az1 = (bn51 - bn231) * (float)6.3888888888888893;
/*<       bz=(5.*bn5/18.)-(23.*bn23/18.) >*/
	bz = bn5 * (float)5. / (float)18. - bn23 * (float)23. / (float)18.;
/*<       bz1=(5.*bn51/18.)-(23.*bn231/18.) >*/
	bz1 = bn51 * (float)5. / (float)18. - bn231 * (float)23. / (float)18.;
/*<       bnz=az/v-bz >*/
	bnz = az / *v - bz;
/*<       bnz1=az1/v-bz1 >*/
	bnz1 = az1 / *v - bz1;
/*<       ev=dz*exp((dlog(bnz)+dlog(bnz1))*.5) >*/
	ev = dz * exp((log(bnz) + log(bnz1)) * (float).5);
/*<       taer55=taer55+ev*sigma*1.0e-03 >*/
	*taer55 += ev * sixs_del__1.sigma * (float).001;
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* oda550_ */

#ifdef __cplusplus
	}
#endif
