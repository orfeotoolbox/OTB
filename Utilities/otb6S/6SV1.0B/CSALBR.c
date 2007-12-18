/* CSALBR.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine csalbr(xtau,xalb) >*/
/* Subroutine */ int csalbr_(doublereal *xtau, doublereal *xalb)
{
    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    extern doublereal fintexp3_(doublereal *);

/*<       real xtau,xalb,fintexp3 >*/
/*<       xalb=(3*xtau-fintexp3(xtau)*(4+2*xtau)+2*exp(-xtau)) >*/
    *xalb = *xtau * 3 - fintexp3_(xtau) * (*xtau * 2 + 4) + exp(-(*xtau)) * 2;
/*<       xalb=xalb/(4.+3*xtau) >*/
    *xalb /= *xtau * 3 + 4.;
/*<       return >*/
    return 0;
/*<       end >*/
} /* csalbr_ */

/*<       real function fintexp3(xtau) >*/
doublereal fintexp3_(doublereal *xtau)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    extern doublereal fintexp1_(doublereal *);
    doublereal xx;

/*<       real xx,xtau,fintexp1 >*/
/*<       xx=(exp(-xtau)*(1.-xtau)+xtau*xtau*fintexp1(xtau))/2. >*/
    xx = (exp(-(*xtau)) * (1. - *xtau) + *xtau * *xtau * fintexp1_(xtau)) / 
	    2.;
/*<       fintexp3=xx >*/
    ret_val = xx;
/*<       return >*/
    return ret_val;
/*<       end >*/
} /* fintexp3_ */

/*<       real function fintexp1(xtau) >*/
doublereal fintexp1_(doublereal *xtau)
{
    /* Initialized data */

    static doublereal a[6] = { -.57721566,.99999193,-.24991055,.05519968,
	    -.00976004,.00107857 };

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    integer i__;
    doublereal xx, xftau;

/* accuracy 2e-07... for 0<xtau<1 */
/*<       real xx,a(0:5),xtau,xftau >*/
/*<       integer i >*/
/*<    >*/
/*<       xx=a(0) >*/
    xx = a[0];
/*<       xftau=1. >*/
    xftau = 1.;
/*<       do i=1,5 >*/
    for (i__ = 1; i__ <= 5; ++i__) {
/*<       xftau=xftau*xtau >*/
	xftau *= *xtau;
/*<       xx=xx+a(i)*xftau >*/
	xx += a[i__] * xftau;
/*<       enddo >*/
    }
/*<       fintexp1=xx-log(xtau) >*/
    ret_val = xx - log(*xtau);
/*<       return >*/
    return ret_val;
/*<       end >*/
} /* fintexp1_ */

#ifdef __cplusplus
	}
#endif
