/* ODRAYL.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int odrayl_(real *wl, real *tray)
{
    /* Initialized data */

    static real pi = (float)3.1415926;

    /* System generated locals */
    real r__1, r__2;
    doublereal d__1, d__2;

    /* Local variables */
    doublereal a;
    integer k;
    doublereal a1, a2, a3, a4;
    real ak;
    doublereal an;
    real ns, sr;
    doublereal awl;
    real dppt;

/*<       double precision a1,a2,a3,a4,awl,an,a >*/
/*<       real wl,tray,z,p,t,wh,wo,delta,sigma,pi,ak,dppt,sr >*/
/*<       integer k >*/
/*     molecular optical depth */
/*<       common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       common /sixs_del/ delta,sigma >*/
/*<       real ns >*/
/*<       data pi /3.1415926/ >*/
/*<       ak=1/wl >*/
    ak = 1 / *wl;
/*<       awl=wl >*/
    awl = *wl;
/*     air refraction index edlen 1966 / metrologia,2,71-80  putting pw=0 
*/
/*<       a1=130.-ak*ak >*/
    a1 = (float)130. - ak * ak;
/*<       a2=38.9-ak*ak >*/
    a2 = (float)38.9 - ak * ak;
/*<       a3=2406030./a1 >*/
    a3 = (float)2406030. / a1;
/*<       a4=15997./a2 >*/
    a4 = (float)15997. / a2;
/*<       an=(8342.13+a3+a4)*1.0e-08 >*/
    an = (a3 + (float)8342.13 + a4) * (float)1e-8;
/*<       an=an+1.d+00 >*/
    an += 1.;
/*<    >*/
/* Computing 3rd power */
    r__1 = pi, r__2 = r__1;
/* Computing 2nd power */
    d__1 = an * an - (float)1.;
/* Computing 2nd power */
    d__2 = an * an + (float)2.;
    a = r__2 * (r__1 * r__1) * (float)24. * (d__1 * d__1) * (
	    sixs_del__1.delta * (float)3. + (float)6.) / ((float)6. - 
	    sixs_del__1.delta * (float)7.) / (d__2 * d__2);
/*<       tray=0. >*/
    *tray = (float)0.;
/*<       do k=1,33 >*/
    for (k = 1; k <= 33; ++k) {
/*<         ns=2.54743e+19 >*/
	ns = (float)2.54743e19;
/*<         dppt=(288.15/1013.25)*(p(k)/t(k)+p(k+1)/t(k+1))/2. >*/
	dppt = (sixs_atm__1.p[k - 1] / sixs_atm__1.t[k - 1] + sixs_atm__1.p[k]
		 / sixs_atm__1.t[k]) * (float).28438193930421907 / (float)2.;
/*<         sr=(a*dppt/(awl**4)/ns*1.e+16)*1.e+05 >*/
/* Computing 4th power */
	d__1 = awl, d__1 *= d__1;
	sr = a * dppt / (d__1 * d__1) / ns * (float)1e16 * (float)1e5;
/*<         tray=tray+(z(k+1)-z(k))*sr >*/
	*tray += (sixs_atm__1.z__[k] - sixs_atm__1.z__[k - 1]) * sr;
/*<       enddo >*/
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* odrayl_ */

#ifdef __cplusplus
	}
#endif
