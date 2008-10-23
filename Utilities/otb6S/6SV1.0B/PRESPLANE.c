/* PRESPLANE.f -- translated by f2c (version 19970805).
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
    doublereal zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#define sixs_planesim__1 sixs_planesim__

/*<       subroutine presplane(uw,uo3,xpp,ftray) >*/
/* Subroutine */ int presplane_(doublereal *uw, doublereal *uo3, doublereal *
	xpp, doublereal *ftray)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal g;
    integer i__, k;
    doublereal ds, xa, xb, rp, ps, rt, ro3, air, xwh, xwo, rmo3[34];
    integer iinf;
    doublereal xalt, rmwh[34];
    integer isup;
    doublereal roair, xtemp;

/*<       real z,p,t,wh,wo,zpl,ppl,tpl,whpl,wopl,xa,xb,xalt >*/
/*<       real xtemp,xwo,xwh,g,air,ro3,rt,rp,roair,ds >*/
/*<       integer i,isup,iinf,k >*/
/*<       real rmo3(34),rmwh(34) >*/
/*<       real ps,xpp,uo3,uw,ftray >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       common /sixs_planesim/zpl(34),ppl(34),tpl(34),whpl(34),wopl(34) >*/
/* log linear interpolation */
/*<       xpp=xpp+z(1) >*/
    *xpp += sixs_atm__1.z__[0];
/*<       if (xpp.ge.100.) xpp=1000. >*/
    if (*xpp >= 100.) {
	*xpp = 1e3;
    }
/*<       i=0 >*/
    i__ = 0;
/*<  10   i=i+1 >*/
L10:
    ++i__;
/*<       if (z(i).le.xpp) goto 10 >*/
    if (sixs_atm__1.z__[i__ - 1] <= *xpp) {
	goto L10;
    }
/*<       isup=i >*/
    isup = i__;
/*<       iinf=i-1 >*/
    iinf = i__ - 1;
/*<       xa=(z(isup)-z(iinf))/alog(p(isup)/p(iinf)) >*/
    xa = (sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]) / log(
	    sixs_atm__1.p[isup - 1] / sixs_atm__1.p[iinf - 1]);
/*<       xb=z(isup)-xa*alog(p(isup)) >*/
    xb = sixs_atm__1.z__[isup - 1] - xa * log(sixs_atm__1.p[isup - 1]);
/*<       ps=exp((xpp-xb)/xa) >*/
    ps = exp((*xpp - xb) / xa);
/* interpolating temperature wator vapor and ozone profile versus altitud 
*/
/*<       xalt=xpp >*/
    xalt = *xpp;
/*<       xtemp=(t(isup)-t(iinf))/(z(isup)-z(iinf)) >*/
    xtemp = (sixs_atm__1.t[isup - 1] - sixs_atm__1.t[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*<       xtemp=xtemp*(xalt-z(iinf))+t(iinf) >*/
    xtemp = xtemp * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.t[iinf - 
	    1];
/*<       xwo=(wo(isup)-wo(iinf))/(z(isup)-z(iinf)) >*/
    xwo = (sixs_atm__1.wo[isup - 1] - sixs_atm__1.wo[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*<       xwo=xwo*(xalt-z(iinf))+wo(iinf) >*/
    xwo = xwo * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.wo[iinf - 1];
/*<       xwh=(wh(isup)-wh(iinf))/(z(isup)-z(iinf)) >*/
    xwh = (sixs_atm__1.wh[isup - 1] - sixs_atm__1.wh[iinf - 1]) / (
	    sixs_atm__1.z__[isup - 1] - sixs_atm__1.z__[iinf - 1]);
/*<       xwh=xwh*(xalt-z(iinf))+wh(iinf) >*/
    xwh = xwh * (xalt - sixs_atm__1.z__[iinf - 1]) + sixs_atm__1.wh[iinf - 1];
/* uptading atmospheric profile */
/*  last level: plane     , complete to 34 */
/*  with interpolated layers */
/*<       do i=1,iinf >*/
    i__1 = iinf;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         zpl(i)=z(i) >*/
	sixs_planesim__1.zpl[i__ - 1] = sixs_atm__1.z__[i__ - 1];
/*<         ppl(i)=p(i) >*/
	sixs_planesim__1.ppl[i__ - 1] = sixs_atm__1.p[i__ - 1];
/*<         tpl(i)=t(i) >*/
	sixs_planesim__1.tpl[i__ - 1] = sixs_atm__1.t[i__ - 1];
/*<         whpl(i)=wh(i) >*/
	sixs_planesim__1.whpl[i__ - 1] = sixs_atm__1.wh[i__ - 1];
/*<         wopl(i)=wo(i) >*/
	sixs_planesim__1.wopl[i__ - 1] = sixs_atm__1.wo[i__ - 1];
/*<       enddo >*/
    }
/*<       zpl(iinf+1)=xalt >*/
    sixs_planesim__1.zpl[iinf] = xalt;
/*<       ppl(iinf+1)=ps >*/
    sixs_planesim__1.ppl[iinf] = ps;
/*<       tpl(iinf+1)=xtemp >*/
    sixs_planesim__1.tpl[iinf] = xtemp;
/*<       whpl(iinf+1)=xwh >*/
    sixs_planesim__1.whpl[iinf] = xwh;
/*<       wopl(iinf+1)=xwo >*/
    sixs_planesim__1.wopl[iinf] = xwo;
/*<       do i=iinf+2,34 >*/
    for (i__ = iinf + 2; i__ <= 34; ++i__) {
/*<         zpl(i)=zpl(iinf+1) >*/
	sixs_planesim__1.zpl[i__ - 1] = sixs_planesim__1.zpl[iinf];
/*<         ppl(i)=ppl(iinf+1) >*/
	sixs_planesim__1.ppl[i__ - 1] = sixs_planesim__1.ppl[iinf];
/*<         tpl(i)=tpl(iinf+1) >*/
	sixs_planesim__1.tpl[i__ - 1] = sixs_planesim__1.tpl[iinf];
/*<         whpl(i)=whpl(iinf+1) >*/
	sixs_planesim__1.whpl[i__ - 1] = sixs_planesim__1.whpl[iinf];
/*<         wopl(i)=wopl(iinf+1) >*/
	sixs_planesim__1.wopl[i__ - 1] = sixs_planesim__1.wopl[iinf];
/*<       enddo >*/
    }
/* compute modified h2o and o3 integrated content */
/* compute conversion factor for rayleigh optical thickness computation */
/* ftray=rp/rt */
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
/*<       rt=0. >*/
    rt = 0.;
/*<       rp=0. >*/
    rp = 0.;
/*<       do k=1,33 >*/
    for (k = 1; k <= 33; ++k) {
/*<         roair=air*273.16*ppl(k)/(1013.25*tpl(k)) >*/
	roair = air * 273.16 * sixs_planesim__1.ppl[k - 1] / (
		sixs_planesim__1.tpl[k - 1] * 1013.25);
/*<         rmwh(k)=wh(k)/(roair*1000.) >*/
	rmwh[k - 1] = sixs_atm__1.wh[k - 1] / (roair * 1e3);
/*<         rmo3(k)=wo(k)/(roair*1000.) >*/
	rmo3[k - 1] = sixs_atm__1.wo[k - 1] / (roair * 1e3);
/*<         rt=rt+(p(k+1)/t(k+1)+p(k)/t(k))*(z(k+1)-z(k)) >*/
	rt += (sixs_atm__1.p[k] / sixs_atm__1.t[k] + sixs_atm__1.p[k - 1] / 
		sixs_atm__1.t[k - 1]) * (sixs_atm__1.z__[k] - sixs_atm__1.z__[
		k - 1]);
/*<         rp=rp+(ppl(k+1)/tpl(k+1)+ppl(k)/tpl(k))*(zpl(k+1)-zpl(k)) >*/
	rp += (sixs_planesim__1.ppl[k] / sixs_planesim__1.tpl[k] + 
		sixs_planesim__1.ppl[k - 1] / sixs_planesim__1.tpl[k - 1]) * (
		sixs_planesim__1.zpl[k] - sixs_planesim__1.zpl[k - 1]);
/*<       enddo >*/
    }
/*<       ftray=rp/rt >*/
    *ftray = rp / rt;
/*<       do k=2,33 >*/
    for (k = 2; k <= 33; ++k) {
/*<         ds=(ppl(k-1)-ppl(k))/ppl(1) >*/
	ds = (sixs_planesim__1.ppl[k - 2] - sixs_planesim__1.ppl[k - 1]) / 
		sixs_planesim__1.ppl[0];
/*<         uw=uw+((rmwh(k)+rmwh(k-1))/2.)*ds >*/
	*uw += (rmwh[k - 1] + rmwh[k - 2]) / 2. * ds;
/*<         uo3=uo3+((rmo3(k)+rmo3(k-1))/2.)*ds >*/
	*uo3 += (rmo3[k - 1] + rmo3[k - 2]) / 2. * ds;
/*<       enddo >*/
    }
/*<       uw=uw*ppl(1)*100./g >*/
    *uw = *uw * sixs_planesim__1.ppl[0] * 100. / g;
/*<       uo3=uo3*ppl(1)*100./g >*/
    *uo3 = *uo3 * sixs_planesim__1.ppl[0] * 100. / g;
/*<       uo3=1000.*uo3/ro3 >*/
    *uo3 = *uo3 * 1e3 / ro3;
/*<       return >*/
    return 0;
/*<       end >*/
} /* presplane_ */

#ifdef __cplusplus
	}
#endif
