/* SPLIN2.f -- translated by f2c (version 19970805).
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

/*<       subroutine splin2(x1a,x2a,ya,y2a,m,n,x1,x2,y) >*/
/* Subroutine */ int splin2_(doublereal *x1a, doublereal *x2a, doublereal *ya,
	 doublereal *y2a, integer *m, integer *n, doublereal *x1, doublereal *
	x2, doublereal *y)
{
    /* System generated locals */
    integer ya_dim1, ya_offset, y2a_dim1, y2a_offset, i__1, i__2;

    /* Local variables */
    integer j, k;
    doublereal ytmp[100], y2tmp[100], yytmp[100];
    extern /* Subroutine */ int spline_(doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *, doublereal *), splint_(doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *)
	    ;

/*<       parameter (nn=100) >*/
/*<       integer m,n,j,k >*/
/*<       real x1,x2,y >*/
/*<       real x1a(m),x2a(n),ya(m,n),y2a(m,n),ytmp(nn),y2tmp(nn) >*/
/*<       real yytmp(nn) >*/
/*<       do 12 j=1,m >*/
    /* Parameter adjustments */
    --x1a;
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
/*<           y2tmp(k)=y2a(j,k) >*/
	    y2tmp[k - 1] = y2a[j + k * y2a_dim1];
/*< 11      continue >*/
/* L11: */
	}
/*<         call splint(x2a,ytmp,y2tmp,n,x2,yytmp(j)) >*/
	splint_(&x2a[1], ytmp, y2tmp, n, x2, &yytmp[j - 1]);
/*< 12    continue >*/
/* L12: */
    }
/*<       call spline(x1a,yytmp,m,1.e30,1.e30,y2tmp) >*/
    spline_(&x1a[1], yytmp, m, &c_b4, &c_b4, y2tmp);
/*<       call splint(x1a,yytmp,y2tmp,m,x1,y) >*/
    splint_(&x1a[1], yytmp, y2tmp, m, x1, y);
/*<       return >*/
    return 0;
/*<       end >*/
} /* splin2_ */

#ifdef __cplusplus
	}
#endif
