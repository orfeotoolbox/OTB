/* KERNEL.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Common Block Declarations */

Extern struct {
    integer nquad;
} num_quad__;

#define num_quad__1 num_quad__

Extern struct {
    doublereal pha[1000], qha[1000], uha[1000], alphal[1001], betal[1001], 
	    gammal[1001], zetal[1001];
} sixs_polar__;

#define sixs_polar__1 sixs_polar__

/*<       subroutine kernel(is,mu,rm,xpl,psl,bp) >*/
/* Subroutine */ int kernel_(integer *is, integer *mu, doublereal *rm, 
	doublereal *xpl, doublereal *psl, doublereal *bp)
{
    /* System generated locals */
    integer rm_offset, xpl_offset, bp_dim1, bp_offset, psl_offset, i__1, i__2,
	     i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    doublereal a, b, c__;
    integer i__, j, k, l;
    doublereal x;
    integer ig, ij;
    doublereal bt;
    integer ip, lm, lp;
    doublereal xx;
    integer ip1;
    doublereal xdb, sbp, rac3;

/*<       integer mu >*/
/*<       real rm(-mu:mu) >*/
/*<       double precision xpl(-mu:mu),bp(0:mu,-mu:mu) >*/
/*<       integer is,ip1,j,i,k,ip,ig,l,lp,lm,ij >*/
/*<       double precision xdb,a,b,c,xx,rac3,x,bt,sbp >*/
/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/*<       double precision psl(-1:nqmax_p,-mu:mu) >*/
/* - to vary the number of quadratures */
/*<       ip1=nquad-3 >*/
    /* Parameter adjustments */
    bp_dim1 = *mu + 1;
    bp_offset = bp_dim1 * (-(*mu));
    bp -= bp_offset;
    psl_offset = -(*mu) * 1002 - 1;
    psl -= psl_offset;
    xpl_offset = -(*mu);
    xpl -= xpl_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;

    /* Function Body */
    ip1 = num_quad__1.nquad - 3;
/*<       rac3=dsqrt(3.D+00) >*/
    rac3 = sqrt(3.);
/*<       if(is.ne.0)go to 700 >*/
    if (*is != 0) {
	goto L700;
    }
/*<       do 25 j=0,mu >*/
    i__1 = *mu;
    for (j = 0; j <= i__1; ++j) {
/*<         c=dble(rm(j)) >*/
	c__ = rm[j];
/*<         psl(0,-j)=1. >*/
	psl[-j * 1002] = 1.;
/*<         psl(0,j)=1. >*/
	psl[j * 1002] = 1.;
/*<         psl(1,j)=c >*/
	psl[j * 1002 + 1] = c__;
/*<         psl(1,-j)=-c >*/
	psl[-j * 1002 + 1] = -c__;
/*<         xdb=(3.*c*c-1.)*0.5 >*/
	xdb = (c__ * 3. * c__ - 1.) * .5;
/*<         if (abs(xdb).lt.1.E-30) xdb=0.0 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         psl(2,-j)=xdb >*/
	psl[-j * 1002 + 2] = xdb;
/*<         psl(2,j)=xdb >*/
	psl[j * 1002 + 2] = xdb;
/*<    25 continue >*/
/* L25: */
    }
/*<       psl(1,0)=rm(0) >*/
    psl[1] = rm[0];
/*<       goto 501 >*/
    goto L501;

/*<   700 if(is.ne.1)go to 701 >*/
L700:
    if (*is != 1) {
	goto L701;
    }
/*<       do 26 j=0,mu >*/
    i__1 = *mu;
    for (j = 0; j <= i__1; ++j) {
/*<         c=dble(rm(j)) >*/
	c__ = rm[j];
/*<         x=1.-c*c >*/
	x = 1. - c__ * c__;
/*<         psl(0,j)=0. >*/
	psl[j * 1002] = 0.;
/*<         psl(0,-j)=0. >*/
	psl[-j * 1002] = 0.;
/*<         psl(1,-j)=sqrt(x*0.5) >*/
	psl[-j * 1002 + 1] = sqrt(x * .5);
/*<         psl(1,j)=sqrt(x*0.5) >*/
	psl[j * 1002 + 1] = sqrt(x * .5);
/*<         psl(2,j)=c*psl(1,j)*rac3 >*/
	psl[j * 1002 + 2] = c__ * psl[j * 1002 + 1] * rac3;
/*<         psl(2,-j)=-psl(2,j) >*/
	psl[-j * 1002 + 2] = -psl[j * 1002 + 2];
/*<    26 continue >*/
/* L26: */
    }
/*<       psl(2,0)=-psl(2,0) >*/
    psl[2] = -psl[2];
/*<       goto 501 >*/
    goto L501;

/*<   701 a=1 >*/
L701:
    a = 1.;
/*<       do 27 i=1,is >*/
    i__1 = *is;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         x=i >*/
	x = (doublereal) i__;
/*<         a=a*sqrt((i+is)/x)*0.5 >*/
	a = a * sqrt((i__ + *is) / x) * .5;
/*<  27   continue >*/
/* L27: */
    }
/*<       b=a*sqrt(is/(is+1.))*sqrt((is-1.)/(is+2.)) >*/
    b = a * sqrt(*is / (*is + 1.)) * sqrt((*is - 1.) / (*is + 2.));
/*<       do 28 j=0,mu >*/
    i__1 = *mu;
    for (j = 0; j <= i__1; ++j) {
/*<         c=dble(rm(j)) >*/
	c__ = rm[j];
/*<         xx=1.-c*c >*/
	xx = 1. - c__ * c__;
/*<         psl(is-1,j)=0. >*/
	psl[*is - 1 + j * 1002] = 0.;
/*<         xdb=a*xx**(is*0.5) >*/
	d__1 = *is * .5;
	xdb = a * pow_dd(&xx, &d__1);
/*<         if (abs(xdb).lt.1.E-30) xdb=0.0 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         psl(is,-j)=xdb >*/
	psl[*is + -j * 1002] = xdb;
/*<         psl(is,j)=xdb >*/
	psl[*is + j * 1002] = xdb;
/*<    28 continue >*/
/* L28: */
    }
/*<   501 k=2 >*/
L501:
    k = 2;
/*<       ip=ip1 >*/
    ip = ip1;
/*<       if(is.gt.2)k=is >*/
    if (*is > 2) {
	k = *is;
    }
/*<       if(k.eq.ip)goto 502 >*/
    if (k == ip) {
	goto L502;
    }
/*<       ig=-1 >*/
    ig = -1;
/*<       if(is.eq.1)ig=1 >*/
    if (*is == 1) {
	ig = 1;
    }
/*<       do 30 l=k,ip-1 >*/
    i__1 = ip - 1;
    for (l = k; l <= i__1; ++l) {
/*<         lp=l+1 >*/
	lp = l + 1;
/*<         lm=l-1 >*/
	lm = l - 1;
/*<         a=(2*l+1.)/sqrt((l+is+1.)*(l-is+1.)) >*/
	a = ((l << 1) + 1.) / sqrt((l + *is + 1.) * (l - *is + 1.));
/*<         b=sqrt(float((l+is)*(l-is)))/(2.*l+1.) >*/
	b = sqrt((doublereal) ((l + *is) * (l - *is))) / (l * 2. + 1.);
/*<         do 31 j=0,mu >*/
	i__2 = *mu;
	for (j = 0; j <= i__2; ++j) {
/*<           c=dble(rm(j)) >*/
	    c__ = rm[j];
/*<           xdb=a*(c*psl(l,j)-b*psl(lm,j)) >*/
	    xdb = a * (c__ * psl[l + j * 1002] - b * psl[lm + j * 1002]);
/*<           if (abs(xdb).lt.1.E-30) xdb=0. >*/
	    if (abs(xdb) < 1e-30) {
		xdb = 0.;
	    }
/*<           psl(lp,j)=xdb >*/
	    psl[lp + j * 1002] = xdb;
/*<           if(j.eq.0) go to 31 >*/
	    if (j == 0) {
		goto L31;
	    }
/*<           psl(lp,-j)=ig*psl(lp,j) >*/
	    psl[lp + -j * 1002] = ig * psl[lp + j * 1002];
/*<    31   continue >*/
L31:
	    ;
	}
/*<         ig=-ig >*/
	ig = -ig;
/*<    30 continue >*/
/* L30: */
    }
/*<   502 continue >*/
L502:
/*<       do 1005 j=-mu,mu >*/
    i__1 = *mu;
    for (j = -(*mu); j <= i__1; ++j) {
/*<         xpl(j)=psl(2,j) >*/
	xpl[j] = psl[j * 1002 + 2];
/*<  1005 continue >*/
/* L1005: */
    }
/*<       ij=ip1 >*/
    ij = ip1;
/*<       do 32 j=0,mu >*/
    i__1 = *mu;
    for (j = 0; j <= i__1; ++j) {
/*<         do 32 k=-mu,mu >*/
	i__2 = *mu;
	for (k = -(*mu); k <= i__2; ++k) {
/*<           sbp=0. >*/
	    sbp = 0.;
/*<           if(is.gt.ij) goto 1 >*/
	    if (*is > ij) {
		goto L1;
	    }
/*<           do 33 l=is,ij >*/
	    i__3 = ij;
	    for (l = *is; l <= i__3; ++l) {
/*<             bt=betal(l) >*/
		bt = sixs_polar__1.betal[l];
/*<             sbp=sbp+dble(psl(l,j))*psl(l,k)*bt >*/
		sbp += psl[l + j * 1002] * psl[l + k * 1002] * bt;
/*<   33      continue >*/
/* L33: */
	    }
/*<  1        continue >*/
L1:
/*<           if (abs(sbp).lt.1.E-30) sbp=0. >*/
	    if (abs(sbp) < 1e-30) {
		sbp = 0.;
	    }
/*<           bp(j,k)=sbp >*/
	    bp[j + k * bp_dim1] = sbp;
/*<    32 continue >*/
/* L32: */
	}
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* kernel_ */

#ifdef __cplusplus
	}
#endif
