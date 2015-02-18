/* ODA550.f -- translated by f2c (version 19970805).
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
    doublereal z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

Extern struct {
    doublereal delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

/*<    >*/
/* Subroutine */ int oda550_(integer *iaer, doublereal *v, doublereal *taer55)
{
    /* Initialized data */

    static doublereal an23[34] = { 2828.,1244.,537.1,225.6,119.2,89.87,63.37,
	    58.9,60.69,58.18,56.75,53.17,55.85,51.56,50.48,47.44,45.11,44.58,
	    43.14,36.34,26.67,19.33,14.55,11.13,8.826,7.429,2.238,.589,.155,
	    .04082,.01078,5.55e-5,1.969e-8,0. };
    static doublereal an5[34] = { 13780.,5030.,1844.,673.1,245.3,89.87,63.37,
	    58.9,60.69,58.18,56.75,53.17,55.85,51.56,50.48,47.44,45.11,44.58,
	    43.14,36.34,26.67,19.33,14.55,11.13,8.826,7.429,2.238,.589,.155,
	    .04082,.01078,5.55e-5,1.969e-8,0. };

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    integer k;
    doublereal az, bz, ev, dz, bn5, az1, bz1, bn23, bn51, bnz, bn231, bnz1;

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
    *taer55 = 0.;
/*<       if(abs(v).le.0.) return >*/
    if (abs(*v) <= 0.) {
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
	az = (bn5 - bn23) * 6.3888888888888893;
/*<       az1=(115./18.)*(bn51-bn231) >*/
	az1 = (bn51 - bn231) * 6.3888888888888893;
/*<       bz=(5.*bn5/18.)-(23.*bn23/18.) >*/
	bz = bn5 * 5. / 18. - bn23 * 23. / 18.;
/*<       bz1=(5.*bn51/18.)-(23.*bn231/18.) >*/
	bz1 = bn51 * 5. / 18. - bn231 * 23. / 18.;
/*<       bnz=az/v-bz >*/
	bnz = az / *v - bz;
/*<       bnz1=az1/v-bz1 >*/
	bnz1 = az1 / *v - bz1;
/*<       ev=dz*exp((dlog(bnz)+dlog(bnz1))*.5) >*/
	ev = dz * exp((log(bnz) + log(bnz1)) * .5);
/*<       taer55=taer55+ev*sigma*1.0e-03 >*/
	*taer55 += ev * sixs_del__1.sigma * .001;
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
