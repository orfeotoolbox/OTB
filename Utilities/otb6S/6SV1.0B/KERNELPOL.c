/* KERNELPOL.f -- translated by f2c (version 19970805).
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

/*<       subroutine kernelpol(is,mu,rm,xpl,xrl,xtl,bp,gr,gt,arr,art,att) >*/
/* Subroutine */ int kernelpol_(integer *is, integer *mu, doublereal *rm, 
	doublereal *xpl, doublereal *xrl, doublereal *xtl, doublereal *bp, 
	doublereal *gr, doublereal *gt, doublereal *arr, doublereal *art, 
	doublereal *att)
{
    /* System generated locals */
    integer rm_offset, xpl_offset, xrl_offset, xtl_offset, bp_dim1, bp_offset,
	     gr_dim1, gr_offset, gt_dim1, gt_offset, arr_dim1, arr_offset, 
	    art_dim1, art_offset, att_dim1, att_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    doublereal a, b, c__, d__, e, f;
    integer i__, j, k, l;
    doublereal x, r1, r2, r3;
    integer ig, ij, ip, lp, lm;
    doublereal xx;
    integer ip1;
    doublereal xdb, sbp, sgr, sgt, psl[51102]	/* was [1002][51] */, rsl[
	    51102]	/* was [1002][51] */, tsl[51102]	/* was [1002][
	    51] */, rac3, sarr, sart, satt;

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/* THOMAS */
/*      double precision psl(-1:nqmax_p,-mu:mu),rsl(-1:nqmax_p,-mu:mu) */
/*      double precision tsl(-1:nqmax_p,-mu:mu) */
/*<       double precision psl(-1:nqmax_p,-25:25),rsl(-1:nqmax_p,-25:25) >*/
/*<       double precision tsl(-1:nqmax_p,-25:25) >*/
/* - to vary the number of quadratures */
/*<       integer mu >*/
/*<       real rm(-mu:mu) >*/
/*<       double precision xpl(-mu:mu),xrl(-mu:mu),xtl(-mu:mu) >*/
/*<       double precision bp(0:mu,-mu:mu),gr(0:mu,-mu:mu),gt(0:mu,-mu:mu) >*/
/*<       double precision arr(0:mu,-mu:mu),art(0:mu,-mu:mu) >*/
/*<       double precision att(0:mu,-mu:mu) >*/
/*<       integer is,ip1,j,i,k,ip,ig,l,lp,lm,ij >*/
/*<       double precision xdb,a,b,c,d,e,f,xx,rac3,x >*/
/*<       double precision sbp,satt,sarr,sgr,sgt,sart >*/
/*<       double precision r1,r2,r3 >*/
/*<       ip1=nquad-3 >*/
    /* Parameter adjustments */
    att_dim1 = *mu + 1;
    att_offset = att_dim1 * (-(*mu));
    att -= att_offset;
    art_dim1 = *mu + 1;
    art_offset = art_dim1 * (-(*mu));
    art -= art_offset;
    arr_dim1 = *mu + 1;
    arr_offset = arr_dim1 * (-(*mu));
    arr -= arr_offset;
    gt_dim1 = *mu + 1;
    gt_offset = gt_dim1 * (-(*mu));
    gt -= gt_offset;
    gr_dim1 = *mu + 1;
    gr_offset = gr_dim1 * (-(*mu));
    gr -= gr_offset;
    bp_dim1 = *mu + 1;
    bp_offset = bp_dim1 * (-(*mu));
    bp -= bp_offset;
    xtl_offset = -(*mu);
    xtl -= xtl_offset;
    xrl_offset = -(*mu);
    xrl -= xrl_offset;
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
/*<         psl(0,j)=1.D+00 >*/
	psl[j * 1002 + 25051] = 1.;
/*<         psl(0,-j)=1.D+00 >*/
	psl[-j * 1002 + 25051] = 1.;
/*<         psl(1,j)=c >*/
	psl[j * 1002 + 25052] = c__;
/*<         psl(1,-j)=-c >*/
	psl[-j * 1002 + 25052] = -c__;
/*<         xdb=(3.D+00*c*c-1.D+00)*0.5D+00 >*/
	xdb = (c__ * 3. * c__ - 1.) * .5;
/*< 	if (abs(xdb).lt.1.e-30) xdb =0.0D+00 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         psl(2,j)=xdb >*/
	psl[j * 1002 + 25053] = xdb;
/*<         psl(2,-j)=xdb >*/
	psl[-j * 1002 + 25053] = xdb;
/*<         rsl(1,j)=0.0D+00 >*/
	rsl[j * 1002 + 25052] = 0.;
/*<         rsl(1,-j)=0.0D+00 >*/
	rsl[-j * 1002 + 25052] = 0.;
/*<         xdb=3.D+00*(1.D+00-c*c)/2.D+00/sqrt(6.D+00) >*/
	xdb = (1. - c__ * c__) * 3. / 2. / sqrt(6.);
/*< 	if (abs(xdb).lt.1.e-30) xdb =0.0D+00 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         rsl(2,j)=xdb >*/
	rsl[j * 1002 + 25053] = xdb;
/*<         rsl(2,-j)=xdb >*/
	rsl[-j * 1002 + 25053] = xdb;
/*<         tsl(1,j)=0.0D+00 >*/
	tsl[j * 1002 + 25052] = 0.;
/*<         tsl(1,-j)=0.0D+00 >*/
	tsl[-j * 1002 + 25052] = 0.;
/*<         tsl(2,j)=0.0D+00 >*/
	tsl[j * 1002 + 25053] = 0.;
/*<         tsl(2,-j)=0.0D+00 >*/
	tsl[-j * 1002 + 25053] = 0.;
/*<    25 continue >*/
/* L25: */
    }
/*<       psl(1,0)=rm(0) >*/
    psl[25052] = rm[0];
/*<       rsl(1,0)=0.0D+00 >*/
    rsl[25052] = 0.;
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
/*<         x=1.d+00-c*c >*/
	x = 1. - c__ * c__;
/*<         psl(0,j)=0.D+00 >*/
	psl[j * 1002 + 25051] = 0.;
/*<         psl(0,-j)=0.D+00 >*/
	psl[-j * 1002 + 25051] = 0.;
/*<         psl(1,j)=dsqrt(x*0.5D+00) >*/
	psl[j * 1002 + 25052] = sqrt(x * .5);
/*<         psl(1,-j)=dsqrt(x*0.5d+00) >*/
	psl[-j * 1002 + 25052] = sqrt(x * .5);
/*<         psl(2,j)=c*psl(1,j)*rac3 >*/
	psl[j * 1002 + 25053] = c__ * psl[j * 1002 + 25052] * rac3;
/*<         psl(2,-j)=-psl(2,j) >*/
	psl[-j * 1002 + 25053] = -psl[j * 1002 + 25053];
/*<         rsl(1,j)=0.0d+00 >*/
	rsl[j * 1002 + 25052] = 0.;
/*<         rsl(1,-j)=0.0d+00 >*/
	rsl[-j * 1002 + 25052] = 0.;
/*<         rsl(2,j)=-c*dsqrt(x)*0.5d+00 >*/
	rsl[j * 1002 + 25053] = -c__ * sqrt(x) * .5;
/*<         rsl(2,-j)=-rsl(2,j) >*/
	rsl[-j * 1002 + 25053] = -rsl[j * 1002 + 25053];
/*<         tsl(1,j)=0.0d+00 >*/
	tsl[j * 1002 + 25052] = 0.;
/*<         tsl(1,-j)=0.0d+00 >*/
	tsl[-j * 1002 + 25052] = 0.;
/*<         tsl(2,j)=-dsqrt(x)*0.5d+00 >*/
	tsl[j * 1002 + 25053] = -sqrt(x) * .5;
/*<         tsl(2,-j)=-dsqrt(x)*0.5d+00 >*/
	tsl[-j * 1002 + 25053] = -sqrt(x) * .5;
/*<    26 continue >*/
/* L26: */
    }
/*<       psl(2,0)=-psl(2,0) >*/
    psl[25053] = -psl[25053];
/*<       rsl(2,0)=-rsl(2,0) >*/
    rsl[25053] = -rsl[25053];
/*<       rsl(1,0)=0.0d+00 >*/
    rsl[25052] = 0.;
/*<       tsl(1,0)=0.0d+00 >*/
    tsl[25052] = 0.;
/*<       goto 501 >*/
    goto L501;

/*<   701 a=1.0d+00 >*/
L701:
    a = 1.;
/*<       do 27 i=1,is >*/
    i__1 = *is;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         x=i >*/
	x = (doublereal) i__;
/*<         a=a*dsqrt((i+is)/x)*0.5d+00 >*/
	a = a * sqrt((i__ + *is) / x) * .5;
/*<  27   continue >*/
/* L27: */
    }
/*<       b=a*dsqrt(is/(is+1.d+00))*dsqrt((is-1.d+00)/(is+2.d+00)) >*/
    b = a * sqrt(*is / (*is + 1.)) * sqrt((*is - 1.) / (*is + 2.));
/*<       do 28 j=0,mu >*/
    i__1 = *mu;
    for (j = 0; j <= i__1; ++j) {
/*<         c=dble(rm(j)) >*/
	c__ = rm[j];
/*<         xx=1.d+00-c*c >*/
	xx = 1. - c__ * c__;
/*<         psl(is-1,j)=0.d+00 >*/
	psl[*is - 1 + j * 1002 + 25051] = 0.;
/*<         rsl(is-1,j)=0.d+00 >*/
	rsl[*is - 1 + j * 1002 + 25051] = 0.;
/*<         tsl(is-1,j)=0.d+00 >*/
	tsl[*is - 1 + j * 1002 + 25051] = 0.;
/*<         xdb=a*xx**(is*0.5d+00) >*/
	d__1 = *is * .5;
	xdb = a * pow_dd(&xx, &d__1);
/*< 	if (abs(xdb).lt.1.e-30) xdb =0.0d+00 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         psl(is,-j)=xdb >*/
	psl[*is + -j * 1002 + 25051] = xdb;
/*<         psl(is,j)=xdb >*/
	psl[*is + j * 1002 + 25051] = xdb;
/*<         xdb=b*(1.+c*c)*xx**(is*0.5-1.d+00) >*/
	d__1 = *is * .5 - 1.;
	xdb = b * (c__ * c__ + 1.) * pow_dd(&xx, &d__1);
/*< 	if (abs(xdb).lt.1.e-30) xdb =0.0d+00 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         rsl(is,-j)=xdb >*/
	rsl[*is + -j * 1002 + 25051] = xdb;
/*<         rsl(is,j)=xdb >*/
	rsl[*is + j * 1002 + 25051] = xdb;
/*<         xdb=2.d+00*b*c*xx**(is*0.5-1.d+00) >*/
	d__1 = *is * .5 - 1.;
	xdb = b * 2. * c__ * pow_dd(&xx, &d__1);
/*< 	if (abs(xdb).lt.1.e-30) xdb =0.0d+00 >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<         tsl(is,-j)=-xdb >*/
	tsl[*is + -j * 1002 + 25051] = -xdb;
/*<         tsl(is,j)=xdb >*/
	tsl[*is + j * 1002 + 25051] = xdb;
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
/*<         a=(2*l+1.d+00)/sqrt((l+is+1.d+00)*(l-is+1.d+00)) >*/
	a = ((l << 1) + 1.) / sqrt((l + *is + 1.) * (l - *is + 1.));
/*<         b=dsqrt(1.d+00*(l+is)*(l-is))/(2.*l+1.d+00) >*/
	b = sqrt((l + *is) * 1. * (l - *is)) / (l * 2. + 1.);
/*<         d=(l+1.d+00)*(2*l+1.d+00) >*/
	d__ = (l + 1.) * ((l << 1) + 1.);
/*<   	d=d/dsqrt((l+3.d+00)*(l-1)*(l+is+1.d+00)*(l-is+1.)) >*/
	d__ /= sqrt((l + 3.) * (l - 1) * (l + *is + 1.) * (l - *is + 1.));
/*< 	e=dsqrt((l+2.d+00)*(l-2.)*(l+is)*(l-is))/(l*(2.*l+1.)) >*/
	e = sqrt((l + 2.) * (l - 2.) * (l + *is) * (l - *is)) / (l * (l * 2. 
		+ 1.));
/*< 	f=2.d+00*is/(l*(l+1.)) >*/
	f = *is * 2. / (l * (l + 1.));
/*<         do 31 j=0,mu >*/
	i__2 = *mu;
	for (j = 0; j <= i__2; ++j) {
/*<           c=dble(rm(j)) >*/
	    c__ = rm[j];
/*<           xdb=a*(c*psl(l,j)-b*psl(lm,j)) >*/
	    xdb = a * (c__ * psl[l + j * 1002 + 25051] - b * psl[lm + j * 
		    1002 + 25051]);
/*< 	  if (abs(xdb).lt.1.e-30) xdb =0.0 >*/
	    if (abs(xdb) < 1e-30) {
		xdb = 0.;
	    }
/*<           psl(lp,j)=xdb >*/
	    psl[lp + j * 1002 + 25051] = xdb;
/*<           xdb=d*(c*rsl(l,j)-f*tsl(l,j)-e*rsl(lm,j)) >*/
	    xdb = d__ * (c__ * rsl[l + j * 1002 + 25051] - f * tsl[l + j * 
		    1002 + 25051] - e * rsl[lm + j * 1002 + 25051]);
/*< 	  if (abs(xdb).lt.1.e-30) xdb =0.0 >*/
	    if (abs(xdb) < 1e-30) {
		xdb = 0.;
	    }
/*<           rsl(lp,j)=xdb >*/
	    rsl[lp + j * 1002 + 25051] = xdb;
/*<           xdb=d*(c*tsl(l,j)-f*rsl(l,j)-e*tsl(lm,j)) >*/
	    xdb = d__ * (c__ * tsl[l + j * 1002 + 25051] - f * rsl[l + j * 
		    1002 + 25051] - e * tsl[lm + j * 1002 + 25051]);
/*< 	  if (abs(xdb).lt.1.e-30) xdb =0.0 >*/
	    if (abs(xdb) < 1e-30) {
		xdb = 0.;
	    }
/*<           tsl(lp,j)=xdb >*/
	    tsl[lp + j * 1002 + 25051] = xdb;
/*<           if(j.eq.0) go to 31 >*/
	    if (j == 0) {
		goto L31;
	    }
/*<           psl(lp,-j)=ig*psl(lp,j) >*/
	    psl[lp + -j * 1002 + 25051] = ig * psl[lp + j * 1002 + 25051];
/*<           rsl(lp,-j)=ig*rsl(lp,j) >*/
	    rsl[lp + -j * 1002 + 25051] = ig * rsl[lp + j * 1002 + 25051];
/*<           tsl(lp,-j)=-ig*tsl(lp,j) >*/
	    tsl[lp + -j * 1002 + 25051] = -ig * tsl[lp + j * 1002 + 25051];
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
	xpl[j] = psl[j * 1002 + 25053];
/*<         xrl(j)=rsl(2,j) >*/
	xrl[j] = rsl[j * 1002 + 25053];
/*<         xtl(j)=tsl(2,j) >*/
	xtl[j] = tsl[j * 1002 + 25053];
/*      write(6,*) "xpl,xrl,xtl ",xpl(j),xrl(j),xtl(j),j */
/*<  1005 continue >*/
/* L1005: */
    }
/*      stop */
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
/*< 	  sgr=0. >*/
	    sgr = 0.;
/*< 	  sgt=0. >*/
	    sgt = 0.;
/*< 	  satt=0. >*/
	    satt = 0.;
/*< 	  sarr=0. >*/
	    sarr = 0.;
/*< 	  sart=0. >*/
	    sart = 0.;
/*<           if(is.gt.ij) goto 1 >*/
	    if (*is > ij) {
		goto L1;
	    }
/* 	  write(6,*) "is,ij ",is,ij */
/*<           do 33 l=is,ij >*/
	    i__3 = ij;
	    for (l = *is; l <= i__3; ++l) {
/*< 	    r1=tsl(l,j)*tsl(l,k) >*/
		r1 = tsl[l + j * 1002 + 25051] * tsl[l + k * 1002 + 25051];
/*< 	    r2=rsl(l,j)*rsl(l,k) >*/
		r2 = rsl[l + j * 1002 + 25051] * rsl[l + k * 1002 + 25051];
/*< 	    r3=psl(l,j)*gammal(l) >*/
		r3 = psl[l + j * 1002 + 25051] * sixs_polar__1.gammal[l];
/*<             sbp=sbp+psl(l,j)*psl(l,k)*betal(l) >*/
		sbp += psl[l + j * 1002 + 25051] * psl[l + k * 1002 + 25051] *
			 sixs_polar__1.betal[l];
/*<             sgr=sgr+rsl(l,k)*r3 >*/
		sgr += rsl[l + k * 1002 + 25051] * r3;
/*<             sgt=sgt+tsl(l,k)*r3 >*/
		sgt += tsl[l + k * 1002 + 25051] * r3;
/* 	  if (is.eq.3.and.j.eq.1)then */
/* 	      write(6,*)j,k,l,gammal(l),psl(l,j),tsl(l,k) */
/* 	      endif */
/*<    >*/
		satt = satt + r1 * sixs_polar__1.alphal[l] + r2 * 
			sixs_polar__1.zetal[l];
/*<    >*/
		sarr = sarr + r1 * sixs_polar__1.zetal[l] + r2 * 
			sixs_polar__1.alphal[l];
/*<    >*/
		sart = sart + tsl[l + j * 1002 + 25051] * rsl[l + k * 1002 + 
			25051] * sixs_polar__1.alphal[l] + rsl[l + j * 1002 + 
			25051] * tsl[l + k * 1002 + 25051] * 
			sixs_polar__1.zetal[l];
/*<   33      continue >*/
/* L33: */
	    }
/*<  1        continue >*/
L1:
/*<  	  if (abs(sbp).lt.1.e-30) sbp =0.0 >*/
	    if (abs(sbp) < 1e-30) {
		sbp = 0.;
	    }
/*<           bp(j,k)=sbp >*/
	    bp[j + k * bp_dim1] = sbp;
/*<  	  if (abs(sgr).lt.1.e-30) sgr =0.0 >*/
	    if (abs(sgr) < 1e-30) {
		sgr = 0.;
	    }
/*<           gr(j,k)=sgr >*/
	    gr[j + k * gr_dim1] = sgr;
/*<  	  if (abs(sgt).lt.1.e-30) sgt =0.0 >*/
	    if (abs(sgt) < 1e-30) {
		sgt = 0.;
	    }
/*<           gt(j,k)=sgt >*/
	    gt[j + k * gt_dim1] = sgt;
/*<  	  if (abs(satt).lt.1.e-30) satt =0.0 >*/
	    if (abs(satt) < 1e-30) {
		satt = 0.;
	    }
/*<           att(j,k)=satt >*/
	    att[j + k * att_dim1] = satt;
/*<  	  if (abs(sart).lt.1.e-30) sart =0.0 >*/
	    if (abs(sart) < 1e-30) {
		sart = 0.;
	    }
/*<           art(j,k)=sart >*/
	    art[j + k * art_dim1] = sart;
/*<  	  if (abs(sarr).lt.1.e-30) sarr =0.0 >*/
	    if (abs(sarr) < 1e-30) {
		sarr = 0.;
	    }
/*<           arr(j,k)=sarr >*/
	    arr[j + k * arr_dim1] = sarr;
/*<    32 continue >*/
/* L32: */
	}
    }
/*<    35 continue >*/
/* L35: */
/*      stop */
/*<       return >*/
    return 0;
/*<       end >*/
} /* kernelpol_ */

#ifdef __cplusplus
	}
#endif
