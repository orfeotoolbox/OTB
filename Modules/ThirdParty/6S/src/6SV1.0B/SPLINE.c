/* SPLINE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine spline(x,y,n,yp1,ypn,y2) >*/
/* Subroutine */ int spline_(doublereal *x, doublereal *y, integer *n, 
	doublereal *yp1, doublereal *ypn, doublereal *y2)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i__, k;
    doublereal p, u[100], qn, un, sig;

/*<       parameter (nmax=100) >*/
/*<       integer n,i,k >*/
/*<       real x(n),y(n),y2(n),u(nmax) >*/
/*<       real yp1,ypn,sig,p,qn,un >*/
/*<       if (yp1.gt..99e30) then >*/
    /* Parameter adjustments */
    --y2;
    --y;
    --x;

    /* Function Body */
    if (*yp1 > 9.9e29) {
/*<         y2(1)=0. >*/
	y2[1] = 0.;
/*<         u(1)=0. >*/
	u[0] = 0.;
/*<       else >*/
    } else {
/*<         y2(1)=-0.5 >*/
	y2[1] = -.5;
/*<         u(1)=(3./(x(2)-x(1)))*((y(2)-y(1))/(x(2)-x(1))-yp1) >*/
	u[0] = 3. / (x[2] - x[1]) * ((y[2] - y[1]) / (x[2] - x[1]) - *yp1);
/*<       endif >*/
    }
/*<       do 11 i=2,n-1 >*/
    i__1 = *n - 1;
    for (i__ = 2; i__ <= i__1; ++i__) {
/*<         sig=(x(i)-x(i-1))/(x(i+1)-x(i-1)) >*/
	sig = (x[i__] - x[i__ - 1]) / (x[i__ + 1] - x[i__ - 1]);
/*<         p=sig*y2(i-1)+2. >*/
	p = sig * y2[i__ - 1] + 2.;
/*<         y2(i)=(sig-1.)/p >*/
	y2[i__] = (sig - 1.) / p;
/*<    >*/
	u[i__ - 1] = (((y[i__ + 1] - y[i__]) / (x[i__ + 1] - x[i__]) - (y[i__]
		 - y[i__ - 1]) / (x[i__] - x[i__ - 1])) * 6. / (x[i__ + 1] - 
		x[i__ - 1]) - sig * u[i__ - 2]) / p;
/*< 11    continue >*/
/* L11: */
    }
/*<       if (ypn.gt..99e30) then >*/
    if (*ypn > 9.9e29) {
/*<         qn=0. >*/
	qn = 0.;
/*<         un=0. >*/
	un = 0.;
/*<       else >*/
    } else {
/*<         qn=0.5 >*/
	qn = .5;
/*<         un=(3./(x(n)-x(n-1)))*(ypn-(y(n)-y(n-1))/(x(n)-x(n-1))) >*/
	un = 3. / (x[*n] - x[*n - 1]) * (*ypn - (y[*n] - y[*n - 1]) / (x[*n] 
		- x[*n - 1]));
/*<       endif >*/
    }
/*<       y2(n)=(un-qn*u(n-1))/(qn*y2(n-1)+1.) >*/
    y2[*n] = (un - qn * u[*n - 2]) / (qn * y2[*n - 1] + 1.);
/*<       do 12 k=n-1,1,-1 >*/
    for (k = *n - 1; k >= 1; --k) {
/*<         y2(k)=y2(k)*y2(k+1)+u(k) >*/
	y2[k] = y2[k] * y2[k + 1] + u[k - 1];
/*< 12    continue >*/
/* L12: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* spline_ */

#ifdef __cplusplus
	}
#endif
