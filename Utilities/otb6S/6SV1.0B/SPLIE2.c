/* SPLIE2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Table of constant values */

static doublereal c_b4 = 1e30;

/*<       subroutine splie2(x2a,ya,m,n,y2a) >*/
/* Subroutine */ int splie2_(doublereal *x2a, doublereal *ya, integer *m, 
	integer *n, doublereal *y2a)
{
    /* System generated locals */
    integer ya_dim1, ya_offset, y2a_dim1, y2a_offset, i__1, i__2;

    /* Local variables */
    integer j, k;
    doublereal ytmp[100], y2tmp[100];
    extern /* Subroutine */ int spline_(doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *, doublereal *);

/*<       parameter (nn=100) >*/
/*<       integer m,n,j,k >*/
/*<       real x2a(n),ya(m,n),y2a(m,n),ytmp(nn),y2tmp(nn) >*/
/*<       do 13 j=1,m >*/
    /* Parameter adjustments */
    y2a_dim1 = *m;
    y2a_offset = y2a_dim1 + 1;
    y2a -= y2a_offset;
    ya_dim1 = *m;
    ya_offset = ya_dim1 + 1;
    ya -= ya_offset;
    --x2a;

    /* Function Body */
    i__1 = *m;
    for (j = 1; j <= i__1; ++j) {
/*<         do 11 k=1,n >*/
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
/*<           ytmp(k)=ya(j,k) >*/
	    ytmp[k - 1] = ya[j + k * ya_dim1];
/*< 11      continue >*/
/* L11: */
	}
/*<         call spline(x2a,ytmp,n,1.e30,1.e30,y2tmp) >*/
	spline_(&x2a[1], ytmp, n, &c_b4, &c_b4, y2tmp);
/*<         do 12 k=1,n >*/
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
/*<           y2a(j,k)=y2tmp(k) >*/
	    y2a[j + k * y2a_dim1] = y2tmp[k - 1];
/*< 12      continue >*/
/* L12: */
	}
/*< 13    continue >*/
/* L13: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* splie2_ */

#ifdef __cplusplus
	}
#endif
