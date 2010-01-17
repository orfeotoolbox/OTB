/* ODRAYL.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int odrayl_(doublereal *wl, doublereal *tray)
{
    /* Initialized data */

    static doublereal pi = 3.1415926;

    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4;

    /* Local variables */
    doublereal a;
    integer k;
    doublereal a1, a2, a3, a4, ak, an, ns, sr, awl, dppt;

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
    a1 = 130. - ak * ak;
/*<       a2=38.9-ak*ak >*/
    a2 = 38.9 - ak * ak;
/*<       a3=2406030./a1 >*/
    a3 = 2406030. / a1;
/*<       a4=15997./a2 >*/
    a4 = 15997. / a2;
/*<       an=(8342.13+a3+a4)*1.0e-08 >*/
    an = (a3 + 8342.13 + a4) * 1e-8;
/*<       an=an+1.d+00 >*/
    an += 1.;
/*<    >*/
/* Computing 3rd power */
    d__1 = pi, d__2 = d__1;
/* Computing 2nd power */
    d__3 = an * an - 1.;
/* Computing 2nd power */
    d__4 = an * an + 2.;
    a = d__2 * (d__1 * d__1) * 24. * (d__3 * d__3) * (sixs_del__1.delta * 3. 
	    + 6.) / (6. - sixs_del__1.delta * 7.) / (d__4 * d__4);
/*<       tray=0. >*/
    *tray = 0.;
/*<       do k=1,33 >*/
    for (k = 1; k <= 33; ++k) {
/*<         ns=2.54743e+19 >*/
	ns = 2.54743e19;
/*<         dppt=(288.15/1013.25)*(p(k)/t(k)+p(k+1)/t(k+1))/2. >*/
	dppt = (sixs_atm__1.p[k - 1] / sixs_atm__1.t[k - 1] + sixs_atm__1.p[k]
		 / sixs_atm__1.t[k]) * .28438193930421907 / 2.;
/*<         sr=(a*dppt/(awl**4)/ns*1.e+16)*1.e+05 >*/
/* Computing 4th power */
	d__1 = awl, d__1 *= d__1;
	sr = a * dppt / (d__1 * d__1) / ns * 1e16 * 1e5;
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
