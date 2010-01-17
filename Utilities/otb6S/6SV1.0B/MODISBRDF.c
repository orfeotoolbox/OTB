/* MODISBRDF.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int modisbrdf_(doublereal *p1, doublereal *p2, doublereal *
	p3, integer *mu, integer *np, doublereal *rm, doublereal *rp, 
	doublereal *brdfint)
{
    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;

    /* Builtin functions */
    double acos(doublereal), atan(doublereal), cos(doublereal), sin(
	    doublereal), tan(doublereal), sqrt(doublereal);

    /* Local variables */
    integer j, k;
    doublereal rossthick, pi, cfi, rfi, sfi, cts, ctv, rts, sts, rtv, stv, 
	    cpha, rpha, cost, tvar, sint, tanti, tantv, angdist, angtemp, 
	    angover, lispars, rosselt;

/*<       real p1,p2,p3,xmu,view >*/
/*<       real dts,dtv,dfs,dfv,dfi >*/
/*<       real rts,rtv,rfs,rfv,rfi,rpha >*/
/*<       real cts,ctv,cfi,cpha >*/
/*<       real sts,stv,sfi >*/
/*<       real tanti,tantv >*/
/*<       real cost,sint,tvar >*/
/*<       real rossthick,rosselt,lispars >*/
/*<       real angdist,angtemp,angover >*/
/*<       integer mu,np,k,j >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       rts=acos(rm(0)) >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
    rts = acos(rm[0]);
/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       rtv=acos(rm(j)) >*/
	    rtv = acos(rm[j]);
/*<       if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<          rfi=rm(-mu) >*/
		rfi = rm[-(*mu)];
/*<          else >*/
	    } else {
/*<          rfi=rp(k)+rm(-mu) >*/
		rfi = rp[k] + rm[-(*mu)];
/*<          endif >*/
	    }
/*<       rfi=abs(rfi) >*/
	    rfi = abs(rfi);
/*< 	cts=cos(rts) >*/
	    cts = cos(rts);
/*< 	ctv=cos(rtv) >*/
	    ctv = cos(rtv);
/*< 	sts=sin(rts) >*/
	    sts = sin(rts);
/*< 	stv=sin(rtv) >*/
	    stv = sin(rtv);
/*< 	cfi=cos(rfi) >*/
	    cfi = cos(rfi);
/*< 	sfi=sin(rfi) >*/
	    sfi = sin(rfi);
/*< 	cpha=cts*ctv+sts*stv*cfi >*/
	    cpha = cts * ctv + sts * stv * cfi;
/*< 	rpha=acos(cpha) >*/
	    rpha = acos(cpha);
/*< 	rosselt=(pi/2-rpha)*cpha+sin(rpha) >*/
	    rosselt = (pi / 2 - rpha) * cpha + sin(rpha);
/*< 	rossthick=(rosselt/(cts+ctv))-pi/4. >*/
	    rossthick = rosselt / (cts + ctv) - pi / 4.;
/*< 	tanti=tan(rts) >*/
	    tanti = tan(rts);
/*< 	tantv=tan(rtv) >*/
	    tantv = tan(rtv);
/*< 	angdist=tanti*tanti+tantv*tantv-2.*tanti*tantv*cfi >*/
	    angdist = tanti * tanti + tantv * tantv - tanti * 2. * tantv * 
		    cfi;
/*< 	angdist=sqrt(angdist) >*/
	    angdist = sqrt(angdist);
/*< 	angtemp=1./cts+1./ctv >*/
	    angtemp = 1. / cts + 1. / ctv;
/*< 	c >*/
	    cost = sqrt(angdist * angdist + tanti * tanti * tantv * tantv * 
		    sfi * sfi) * 2.;
/*< 	cost=cost/angtemp >*/
	    cost /= angtemp;
/*< 	if (cost.ge.1.) cost=1. >*/
	    if (cost >= 1.) {
		cost = 1.;
	    }
/*< 	if (cost.le.-1.) cost=-1. >*/
	    if (cost <= -1.) {
		cost = -1.;
	    }
/*< 	tvar=acos(cost) >*/
	    tvar = acos(cost);
/*< 	sint=sqrt(1.-cost*cost) >*/
	    sint = sqrt(1. - cost * cost);
/*< 	angover=(tvar-sint*cost)*angtemp/pi >*/
	    angover = (tvar - sint * cost) * angtemp / pi;
/*< 	lispars=angover-angtemp+0.5*(1.+cpha)/cts/ctv >*/
	    lispars = angover - angtemp + (cpha + 1.) * .5 / cts / ctv;
/*<       brdfint(j,k)=p1+p2*rossthick+p3*lispars >*/
	    brdfint[j + k * brdfint_dim1] = *p1 + *p2 * rossthick + *p3 * 
		    lispars;
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* modisbrdf_ */

#ifdef __cplusplus
	}
#endif
