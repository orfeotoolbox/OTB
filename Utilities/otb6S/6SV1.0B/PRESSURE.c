/* PRESSURE.f -- translated by f2c (version 19970805).
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

/*<        subroutine pressure(uw,uo3,xps) >*/
/* Subroutine */ int pressure_(doublereal *uw, doublereal *uo3, doublereal *
	xps)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal g;
    integer i__, k, l;
    doublereal ds, xa, xb, ps, ro3, air, xwh, xwo, rmo3[34];
    integer iinf;
    doublereal xalt, rmwh[34];
    integer isup;
    doublereal roair, xtemp;

/*<        common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<        real z,p,t,wh,wo,xa,xb,xalt,xtemp,xwo,xwh,g >*/
/*<        real air,ro3,roair,ds >*/
/*<        integer i,isup,iinf,l,k >*/
/*<        real rmo3(34),rmwh(34) >*/
/*<        real ps,xps,uo3,uw >*/
/*      log linear interpolation */
/*<            xps=-xps >*/
    *xps = -(*xps);
/*< 	   if (xps.ge.100.) xps=99.99 >*/
    if (*xps >= 100.) {
	*xps = 99.99;
    }
/*<            i=0 >*/
    i__ = 0;
/*<  10        i=i+1 >*/
L10:
    ++i__;
/*<            if (z(i).le.xps) goto 10 >*/
    if (sixs_atm__1.z__[i__ - 1] <= *xps) {
	goto L10;
    }
/*<            isup=i >*/
    isup = i__;
/*<            iinf=i-1 >*/
    iinf = i__ - 1;
/*<            xa=(z(isup)-z(iinf))/alog(p(isup)/p(iinf)) >*/
    xa = (sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]) / log(
	    sixs_atm__1.p[isup - 1] / sixs_atm__1.p[iinf - 1]);
/*<            xb=z(isup)-xa*alog(p(isup)) >*/
    xb = sixs_atm__1.z__[isup - 1] - xa * log(sixs_atm__1.p[isup - 1]);
/*<            ps=exp((xps-xb)/xa) >*/
    ps = exp((*xps - xb) / xa);
/* interpolating temperature wator vapor and ozone profile versus altitud 
*/
/*< 	   xalt=xps >*/
    xalt = *xps;
/*< 	   xtemp=(t(isup)-t(iinf))/(z(isup)-z(iinf)) >*/
    xtemp = (sixs_atm__1.t[isup - 1] - sixs_atm__1.t[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*< 	   xtemp=xtemp*(xalt-z(iinf))+t(iinf) >*/
    xtemp = xtemp * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.t[iinf - 
	    1];
/*< 	   xwo=(wo(isup)-wo(iinf))/(z(isup)-z(iinf)) >*/
    xwo = (sixs_atm__1.wo[isup - 1] - sixs_atm__1.wo[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*< 	   xwo=xwo*(xalt-z(iinf))+wo(iinf) >*/
    xwo = xwo * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.wo[iinf - 1];
/*< 	   xwh=(wh(isup)-wh(iinf))/(z(isup)-z(iinf)) >*/
    xwh = (sixs_atm__1.wh[isup - 1] - sixs_atm__1.wh[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*< 	   xwh=xwh*(xalt-z(iinf))+wh(iinf) >*/
    xwh = xwh * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.wh[iinf - 1];
/* uptading atmospheric profile */
/*  1rst level: target     , complete to 34 */
/*  with interpolated layers */
/*<       z(1)=xalt >*/
    sixs_atm__1.z__[0] = xalt;
/*<       p(1)=ps >*/
    sixs_atm__1.p[0] = ps;
/*<       t(1)=xtemp >*/
    sixs_atm__1.t[0] = xtemp;
/*<       wh(1)=xwh >*/
    sixs_atm__1.wh[0] = xwh;
/*<       wo(1)=xwo >*/
    sixs_atm__1.wo[0] = xwo;
/*<       do i=2,33-iinf+1 >*/
    i__1 = 33 - iinf + 1;
    for (i__ = 2; i__ <= i__1; ++i__) {
/*<       z(i)=z(i+iinf-1) >*/
	sixs_atm__1.z__[i__ - 1] = sixs_atm__1.z__[i__ + iinf - 2];
/*<       p(i)=p(i+iinf-1) >*/
	sixs_atm__1.p[i__ - 1] = sixs_atm__1.p[i__ + iinf - 2];
/*<       t(i)=t(i+iinf-1) >*/
	sixs_atm__1.t[i__ - 1] = sixs_atm__1.t[i__ + iinf - 2];
/*<       wh(i)=wh(i+iinf-1) >*/
	sixs_atm__1.wh[i__ - 1] = sixs_atm__1.wh[i__ + iinf - 2];
/*<       wo(i)=wo(i+iinf-1) >*/
	sixs_atm__1.wo[i__ - 1] = sixs_atm__1.wo[i__ + iinf - 2];
/*<       enddo >*/
    }
/*<       l=33-iinf+1 >*/
    l = 33 - iinf + 1;
/*<       do i=l+1,34 >*/
    for (i__ = l + 1; i__ <= 34; ++i__) {
/*<       z(i)=(z(34)-z(l))*(i-l)/(34-l)+z(l) >*/
	sixs_atm__1.z__[i__ - 1] = (sixs_atm__1.z__[33] - sixs_atm__1.z__[l - 
		1]) * (i__ - l) / (34 - l) + sixs_atm__1.z__[l - 1];
/*<       p(i)=(p(34)-p(l))*(i-l)/(34-l)+p(l) >*/
	sixs_atm__1.p[i__ - 1] = (sixs_atm__1.p[33] - sixs_atm__1.p[l - 1]) * 
		(i__ - l) / (34 - l) + sixs_atm__1.p[l - 1];
/*<       t(i)=(t(34)-t(l))*(i-l)/(34-l)+t(l) >*/
	sixs_atm__1.t[i__ - 1] = (sixs_atm__1.t[33] - sixs_atm__1.t[l - 1]) * 
		(i__ - l) / (34 - l) + sixs_atm__1.t[l - 1];
/*<       wh(i)=(wh(34)-wh(l))*(i-l)/(34-l)+wh(l) >*/
	sixs_atm__1.wh[i__ - 1] = (sixs_atm__1.wh[33] - sixs_atm__1.wh[l - 1])
		 * (i__ - l) / (34 - l) + sixs_atm__1.wh[l - 1];
/*<       wo(i)=(wo(34)-wo(l))*(i-l)/(34-l)+wo(l) >*/
	sixs_atm__1.wo[i__ - 1] = (sixs_atm__1.wo[33] - sixs_atm__1.wo[l - 1])
		 * (i__ - l) / (34 - l) + sixs_atm__1.wo[l - 1];
/*<       enddo >*/
    }
/* compute modified h2o and o3 integrated content */
/*<       uw=0. >*/
    *uw = 0.;
/*<       uo3=0. >*/
    *uo3 = 0.;
/*<       g=98.1 >*/
    g = 98.1;
/*<       air=0.028964/0.0224 >*/
    air = 1.2930357142857143;
/*<       ro3=0.048/0.0224 >*/
    ro3 = 2.1428571428571428;
/*<       do k=1,33 >*/
    for (k = 1; k <= 33; ++k) {
/*<       roair=air*273.16*p(k)/(1013.25*t(k)) >*/
	roair = air * 273.16 * sixs_atm__1.p[k - 1] / (sixs_atm__1.t[k - 1] * 
		1013.25);
/*<       rmwh(k)=wh(k)/(roair*1000.) >*/
	rmwh[k - 1] = sixs_atm__1.wh[k - 1] / (roair * 1e3);
/*<       rmo3(k)=wo(k)/(roair*1000.) >*/
	rmo3[k - 1] = sixs_atm__1.wo[k - 1] / (roair * 1e3);
/*<       enddo >*/
    }
/*<       do k=2,33 >*/
    for (k = 2; k <= 33; ++k) {
/*<       ds=(p(k-1)-p(k))/p(1) >*/
	ds = (sixs_atm__1.p[k - 2] - sixs_atm__1.p[k - 1]) / sixs_atm__1.p[0];
/*<       uw=uw+((rmwh(k)+rmwh(k-1))/2.)*ds >*/
	*uw += (rmwh[k - 1] + rmwh[k - 2]) / 2. * ds;
/*<       uo3=uo3+((rmo3(k)+rmo3(k-1))/2.)*ds >*/
	*uo3 += (rmo3[k - 1] + rmo3[k - 2]) / 2. * ds;
/*<       enddo >*/
    }
/*<       uw=uw*p(1)*100./g >*/
    *uw = *uw * sixs_atm__1.p[0] * 100. / g;
/*<       uo3=uo3*p(1)*100./g >*/
    *uo3 = *uo3 * sixs_atm__1.p[0] * 100. / g;
/*<       uo3=1000.*uo3/ro3 >*/
    *uo3 = *uo3 * 1e3 / ro3;
/*<       return >*/
    return 0;
/*<       end >*/
} /* pressure_ */

#ifdef __cplusplus
	}
#endif
