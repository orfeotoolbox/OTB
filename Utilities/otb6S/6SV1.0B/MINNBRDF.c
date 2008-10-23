/* MINNBRDF.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int minnbrdf_(doublereal *par1, doublereal *par2, integer *
	mu, integer *np, doublereal *rm, doublereal *brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    integer j, k;
    doublereal xmu, view;

/*<       real par1,par2,xmu,view >*/
/*<       integer mu,np,k,j >*/
/*<       real rm(-mu:mu),brdfint(-mu:mu,np) >*/
/*<       xmu=rm(0) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;

    /* Function Body */
    xmu = rm[0];
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       view=rm(j) >*/
	    view = rm[j];
/*<       brdfint(j,k)=0.5*par2*(par1+1.)*((xmu*view)**(par1-1)) >*/
	    d__1 = xmu * view;
	    d__2 = *par1 - 1;
	    brdfint[j + k * brdfint_dim1] = *par2 * .5 * (*par1 + 1.) * 
		    pow_dd(&d__1, &d__2);
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* minnbrdf_ */

#ifdef __cplusplus
	}
#endif
