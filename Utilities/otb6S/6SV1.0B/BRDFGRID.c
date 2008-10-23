/* BRDFGRID.f -- translated by f2c (version 19970805).
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

static integer c__10 = 10;
static integer c__13 = 13;

/*<    >*/
/* Subroutine */ int brdfgrid_(integer *mu, integer *np, doublereal *rm, 
	doublereal *rp, doublereal *brdfdat, doublereal *angmu, doublereal *
	angphi, doublereal *brdfint)
{
    /* System generated locals */
    integer brdfint_dim1, brdfint_offset, rm_offset, i__1, i__2;

    /* Local variables */
    doublereal brdftemp[130]	/* was [10][13] */, gaussphi;
    integer j, k;
    doublereal y;
    extern /* Subroutine */ int splie2_(doublereal *, doublereal *, integer *,
	     integer *, doublereal *), splin2_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal gaussmu;

/*<       integer mu,np >*/
/*<    >*/
/*<       real brdftemp(10,13) >*/
/*<       real gaussmu,gaussphi,y >*/
/*<       integer j,k >*/
/*<       do 10 j=1,np >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;
    brdfdat -= 11;
    --angmu;
    --angphi;

    /* Function Body */
    i__1 = *np;
    for (j = 1; j <= i__1; ++j) {
/*<       do 10 k=1,mu >*/
	i__2 = *mu;
	for (k = 1; k <= i__2; ++k) {
/*<    10 brdfint(k,j)=0. >*/
/* L10: */
	    brdfint[k + j * brdfint_dim1] = 0.;
	}
    }
/*<       call splie2(angphi,brdfdat,10,13,brdftemp) >*/
    splie2_(&angphi[1], &brdfdat[11], &c__10, &c__13, brdftemp);
/*<       do 1 j=1,np >*/
    i__2 = *np;
    for (j = 1; j <= i__2; ++j) {
/*<       do 2 k=1,mu >*/
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<       gaussmu=rm(k) >*/
	    gaussmu = rm[k];
/*<       gaussphi=rp(j) >*/
	    gaussphi = rp[j];
/*<    >*/
	    splin2_(&angmu[1], &angphi[1], &brdfdat[11], brdftemp, &c__10, &
		    c__13, &gaussmu, &gaussphi, &y);
/*<       brdfint(k,j)=y >*/
	    brdfint[k + j * brdfint_dim1] = y;
/*<    2  continue >*/
/* L2: */
	}
/*<    1  continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* brdfgrid_ */

#ifdef __cplusplus
	}
#endif
