/* SPLINT.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine splint(xa,ya,y2a,n,x,y) >*/
/* Subroutine */ int splint_(real *xa, real *ya, real *y2a, integer *n, real *
	x, real *y)
{
    /* System generated locals */
    real r__1, r__2, r__3, r__4, r__5;

    /* Builtin functions */
    /* Subroutine */ int s_paus(char *, ftnlen);

    /* Local variables */
    real a, b, h__;
    integer k, khi, klo;

/*<       integer n,klo,khi,k >*/
/*<       real xa(n),ya(n),y2a(n) >*/
/*<       real x,y,h,a,b >*/
/*<       klo=1 >*/
    /* Parameter adjustments */
    --y2a;
    --ya;
    --xa;

    /* Function Body */
    klo = 1;
/*<       khi=n >*/
    khi = *n;
/*< 1     if (khi-klo.gt.1) then >*/
L1:
    if (khi - klo > 1) {
/*<         k=(khi+klo)/2 >*/
	k = (khi + klo) / 2;
/*<         if(xa(k).gt.x)then >*/
	if (xa[k] > *x) {
/*<           khi=k >*/
	    khi = k;
/*<         else >*/
	} else {
/*<           klo=k >*/
	    klo = k;
/*<         endif >*/
	}
/*<       goto 1 >*/
	goto L1;
/*<       endif >*/
    }
/*<       h=xa(khi)-xa(klo) >*/
    h__ = xa[khi] - xa[klo];
/*<       if (h.eq.0.) pause 'bad xa input.' >*/
    if (h__ == (float)0.) {
	s_paus("bad xa input.", 13L);
    }
/*<       a=(xa(khi)-x)/h >*/
    a = (xa[khi] - *x) / h__;
/*<       b=(x-xa(klo))/h >*/
    b = (*x - xa[klo]) / h__;
/*<    >*/
/* Computing 3rd power */
    r__1 = a, r__2 = r__1;
/* Computing 3rd power */
    r__3 = b, r__4 = r__3;
/* Computing 2nd power */
    r__5 = h__;
    *y = a * ya[klo] + b * ya[khi] + ((r__2 * (r__1 * r__1) - a) * y2a[klo] + 
	    (r__4 * (r__3 * r__3) - b) * y2a[khi]) * (r__5 * r__5) / (float)
	    6.;
/*<       return >*/
    return 0;
/*<       end >*/
} /* splint_ */

#ifdef __cplusplus
	}
#endif
