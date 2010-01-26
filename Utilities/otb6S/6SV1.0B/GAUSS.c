/* GAUSS.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine gauss(x1,x2,x,w,n) >*/
/* Subroutine */ int gauss_(doublereal *x1, doublereal *x2, doublereal *x, 
	doublereal *w, integer *n)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double cos(doublereal);

    /* Local variables */
    integer i__, j, m;
    doublereal z__, p1, p2, p3, z1, pp, xl, xm;

/*<       integer n >*/
/*<       real x1,x2,x(n),w(n) >*/
/*<       double precision xm,xl,z,p1,p2,p3,pp,z1 >*/
/*<       integer m,i,j >*/
/*<       parameter (eps=3.d-14) >*/
/*<       m=(n+1)/2 >*/
    /* Parameter adjustments */
    --w;
    --x;

    /* Function Body */
    m = (*n + 1) / 2;
/*<       xm=0.5d0*(x2+x1) >*/
    xm = (*x2 + *x1) * .5;
/*<       xl=0.5d0*(x2-x1) >*/
    xl = (*x2 - *x1) * .5;
/*<       do 12 i=1,m >*/
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         z=cos(3.141592654d0*(i-.25d0)/(n+.5d0)) >*/
	z__ = cos((i__ - .25) * 3.141592654 / (*n + .5));
/*< 1       continue >*/
L1:
/*<           p1=1.d0 >*/
	p1 = 1.;
/*<           p2=0.d0 >*/
	p2 = 0.;
/*<           do 11 j=1,n >*/
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/*<             p3=p2 >*/
	    p3 = p2;
/*<             p2=p1 >*/
	    p2 = p1;
/*<             p1=((2.d0*j-1.d0)*z*p2-(j-1.d0)*p3)/j >*/
	    p1 = ((j * 2. - 1.) * z__ * p2 - (j - 1.) * p3) / j;
/*< 11        continue >*/
/* L11: */
	}
/*<           pp=n*(z*p1-p2)/(z*z-1.d0) >*/
	pp = *n * (z__ * p1 - p2) / (z__ * z__ - 1.);
/*<           z1=z >*/
	z1 = z__;
/*<           z=z1-p1/pp >*/
	z__ = z1 - p1 / pp;
/*<         if(abs(z-z1).gt.eps)go to 1 >*/
	if ((d__1 = z__ - z1, abs(d__1)) > 3e-14) {
	    goto L1;
	}
/*<         if (abs(z).lt.eps) z=0. >*/
	if (abs(z__) < 3e-14) {
	    z__ = 0.;
	}
/*<         x(i)=xm-xl*z >*/
	x[i__] = xm - xl * z__;
/*<         x(n+1-i)=xm+xl*z >*/
	x[*n + 1 - i__] = xm + xl * z__;
/*<         w(i)=2.d0*xl/((1.d0-z*z)*pp*pp) >*/
	w[i__] = xl * 2. / ((1. - z__ * z__) * pp * pp);
/*<         w(n+1-i)=w(i) >*/
	w[*n + 1 - i__] = w[i__];
/*< 12    continue >*/
/* L12: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* gauss_ */

#ifdef __cplusplus
	}
#endif
