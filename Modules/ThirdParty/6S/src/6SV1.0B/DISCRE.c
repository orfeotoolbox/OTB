/* DISCRE.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int discre_(doublereal *ta, doublereal *ha, doublereal *tr, 
	doublereal *hr, integer *it, integer *nt, doublereal *yy, doublereal *
	dd, doublereal *ppp2, doublereal *ppp1, doublereal *zx)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    doublereal y1, y2, y3, x2, dt, xd, ti, xx;
    extern /* Subroutine */ int print_error__(char *, ftnlen);
    doublereal delta, ecart;

/*<       double precision ta,ha,tr,hr,yy,dd,ppp2,ppp1,zx,dt,ti >*/
/*<       double precision y1,y2,y3,x2,xd,delta,ecart >*/
/*<       double precision xx >*/
/*<       integer it,nt >*/
/*<       if (ha.ge.7.) then >*/
    if (*ha >= 7.) {
/*<    >*/
	print_error__("check aerosol measurements or plane altitude", 44L);
/*<           return >*/
	return 0;
/*<           endif >*/
    }
/*<       if (it.eq.0) then >*/
    if (*it == 0) {
/*<          dt=1.e-17 >*/
	dt = 1e-17;
/*<          else >*/
    } else {
/*<          dt=2.*(ta+tr-yy)/(nt-it+1.) >*/
	dt = (*ta + *tr - *yy) * 2. / (*nt - *it + 1.);
/*<       endif >*/
    }
/*<   99  dt=dt/2. >*/
L99:
    dt /= 2.;
/*<       ti=yy+dt >*/
    ti = *yy + dt;
/*<       y1=ppp2 >*/
    y1 = *ppp2;
/*<       y3=ppp1 >*/
    y3 = *ppp1;
/*<   706 y2=(y1+y3)*0.5 >*/
L706:
    y2 = (y1 + y3) * .5;
/*<       xx=-y2/ha >*/
    xx = -y2 / *ha;
/*<       if (xx.lt.-18) then >*/
    if (xx < -18.) {
/*<          x2=tr*exp(-y2/hr) >*/
	x2 = *tr * exp(-y2 / *hr);
/*<          else >*/
    } else {
/*<          x2=ta*dexp(xx)+tr*exp(-y2/hr) >*/
	x2 = *ta * exp(xx) + *tr * exp(-y2 / *hr);
/*<          endif >*/
    }
/*<       xd=abs(ti-x2) >*/
    xd = (d__1 = ti - x2, abs(d__1));
/*<       if(xd.lt.0.00001) go to 705 >*/
    if (xd < 1e-5) {
	goto L705;
    }
/*<       if(ti-x2) 701,703,703 >*/
    if (ti - x2 >= 0.) {
	goto L703;
    } else {
	goto L701;
    }
/*<   701 y3=y2 >*/
L701:
    y3 = y2;
/*<       go to 706 >*/
    goto L706;
/*<   703 y1=y2 >*/
L703:
    y1 = y2;
/*<       go to 706 >*/
    goto L706;
/*<   705 zx=y2 >*/
L705:
    *zx = y2;
/*<       delta=1./(1.+ta*hr/tr/ha*exp((zx-ppp1)*(1./hr-1./ha))) >*/
    delta = 1. / (*ta * *hr / *tr / *ha * exp((*zx - *ppp1) * (1. / *hr - 1. /
	     *ha)) + 1.);
/*<       ecart=0 >*/
    ecart = 0.;
/*<       if(dd.ne.0) ecart=abs((dd-delta)/dd) >*/
    if (*dd != 0.) {
	ecart = (d__1 = (*dd - delta) / *dd, abs(d__1));
    }
/*<       if((ecart.gt.0.75).and.(it.ne.0)) go to 99 >*/
    if (ecart > .75 && *it != 0) {
	goto L99;
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* discre_ */

#ifdef __cplusplus
	}
#endif
