/* WALTBRDF.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int waltbrdf_(doublereal *a, doublereal *ap, doublereal *b, 
	doublereal *c__, integer *mu, integer *np, doublereal *rm, doublereal 
	*rp, doublereal *brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;

    /* Builtin functions */
    double acos(doublereal), cos(doublereal);

    /* Local variables */
    integer j, k;
    doublereal fi, ts, tv, phi, xmu, view;

/* this model can be found in applied optics vol 24 / no 3/ pp 383-387 */
/* but it has to be modified (slightly) to match reciprocity principle */
/*<       integer mu,np,k,j >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real a,ap,b,c >*/
/*<       real xmu,ts,view,tv,fi,phi >*/
/*<       xmu=rm(0) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
    xmu = rm[0];
/*<       ts=acos(xmu) >*/
    ts = acos(xmu);
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       view=rm(j) >*/
	    view = rm[j];
/*<       tv=acos(view) >*/
	    tv = acos(view);
/*<       if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<          fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<          else >*/
	    } else {
/*<          fi=rp(k)+rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<          endif >*/
	    }
/*<       phi=fi >*/
	    phi = fi;
/*<       brdfint(j,k)=a*(ts*ts*tv*tv)+ap*(ts*ts+tv*tv)+b*ts*tv*cos(phi)+c >*/
	    brdfint[j + k * brdfint_dim1] = *a * (ts * ts * tv * tv) + *ap * (
		    ts * ts + tv * tv) + *b * ts * tv * cos(phi) + *c__;
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* waltbrdf_ */

#ifdef __cplusplus
	}
#endif
