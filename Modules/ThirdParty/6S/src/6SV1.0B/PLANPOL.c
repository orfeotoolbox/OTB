/* PLANPOL.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       Subroutine dirpopol(xq,xu,dirpol) >*/
/* Subroutine */ int dirpopol_(doublereal *xq, doublereal *xu, doublereal *
	dirpol)
{
    /* Builtin functions */
    double acos(doublereal), atan(doublereal);

    /* Local variables */
    doublereal pi;


/* This subroutine computes the plane of polarization */
/*<       real xu,xq,dirpol,pi >*/
/*<       pi=acos(-1.) >*/
    pi = acos(-1.);
/*<       if (abs(xq).lt.0.00001)then >*/
    if (abs(*xq) < 1e-5) {
/*< 	if (xu.gt.0.)then >*/
	if (*xu > 0.) {
/*< 	  dirpol=45.0 >*/
	    *dirpol = 45.;
/*< 	else >*/
	} else {
/*< 	  dirpol=-45.0 >*/
	    *dirpol = -45.;
/*< 	endif >*/
	}
/*< 	return >*/
	return 0;
/*<       endif >*/
    }
/*<       if (abs(xu).lt.0.00001)then >*/
    if (abs(*xu) < 1e-5) {
/*<         if (xq.gt.0.)then >*/
	if (*xq > 0.) {
/*< 	  dirpol=0.0 >*/
	    *dirpol = 0.;
/*< 	else >*/
	} else {
/*< 	  dirpol=90.0 >*/
	    *dirpol = 90.;
/*< 	endif >*/
	}
/*< 	return >*/
	return 0;
/*<       endif >*/
    }
/*<       if (xq.gt.0.)then >*/
    if (*xq > 0.) {
/*< 	dirpol=90./pi*atan(xu/xq) >*/
	*dirpol = 90. / pi * atan(*xu / *xq);
/*< 	return >*/
	return 0;
/*<       endif >*/
    }
/*<       if (xu.gt.0.)then >*/
    if (*xu > 0.) {
/*< 	dirpol=90.+90./pi*atan(xu/xq) >*/
	*dirpol = 90. / pi * atan(*xu / *xq) + 90.;
/*<       else >*/
    } else {
/*< 	dirpol=-90.+90./pi*atan(xu/xq) >*/
	*dirpol = 90. / pi * atan(*xu / *xq) - 90.;
/*<       endif >*/
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* dirpopol_ */

#ifdef __cplusplus
	}
#endif
