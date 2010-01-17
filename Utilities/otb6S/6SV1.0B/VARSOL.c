/* VARSOL.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int varsol_(integer *jday, integer *month, doublereal *dsol)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double acos(doublereal), cos(doublereal);

    /* Local variables */
    integer j;
    doublereal pi, om;

/*<       real dsol,pi,om >*/
/*<       integer jday,month,j >*/
/*     calculation of the variability of the solar constant during the */
/*     year. */
/*     jday is the number of the day in the month */
/*     dsol is a multiplicative factor to apply to the mean value of */
/*     solar constant */
/*<       if (month.le.2) goto 1 >*/
    if (*month <= 2) {
	goto L1;
    }
/*<       if (month.gt.8) goto 2 >*/
    if (*month > 8) {
	goto L2;
    }
/*<       j=31*(month-1)-((month-1)/2)-2+jday >*/
    j = (*month - 1) * 31 - (*month - 1) / 2 - 2 + *jday;
/*<       goto 3 >*/
    goto L3;
/*<     1 j=31*(month-1)+jday >*/
L1:
    j = (*month - 1) * 31 + *jday;
/*<       goto 3 >*/
    goto L3;
/*<     2 j=31*(month-1)-((month-2)/2)-2+jday >*/
L2:
    j = (*month - 1) * 31 - (*month - 2) / 2 - 2 + *jday;
/*<     3 pi=2.*acos (0.) >*/
L3:
    pi = acos(0.) * 2.;
/*<       om=(.9856*float(j-4))*pi/180. >*/
    om = (doublereal) (j - 4) * .9856 * pi / 180.;
/*<       dsol=1./((1.-.01673*cos(om))**2) >*/
/* Computing 2nd power */
    d__1 = 1. - cos(om) * .01673;
    *dsol = 1. / (d__1 * d__1);
/*<       return >*/
    return 0;
/*<       end >*/
} /* varsol_ */

#ifdef __cplusplus
	}
#endif
