/* OSPOL.f -- translated by f2c (version 19970805).
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

Extern struct {
    doublereal delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

Extern struct {
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

Extern struct {
    integer igmax;
} multorder_;

#define multorder_1 multorder_

/*<    >*/
/* Subroutine */ int ospol_(integer *iaer_prof__, doublereal *tamoy, 
	doublereal *trmoy, doublereal *pizmoy, doublereal *tamoyp, doublereal 
	*trmoyp, doublereal *palt, doublereal *phirad, integer *nt, integer *
	mu, integer *np, doublereal *rm, doublereal *gb, doublereal *rp, 
	doublereal *xli, doublereal *xlq, doublereal *xlu, doublereal *xlphim,
	 integer *nfi, integer *nfilut, doublereal *filut, doublereal *rolut, 
	doublereal *rolutq, doublereal *rolutu)
{
    /* System generated locals */
    integer rm_offset, gb_offset, xli_dim1, xli_offset, xlq_dim1, xlq_offset, 
	    xlu_dim1, xlu_offset, rolut_dim1, rolut_offset, rolutq_dim1, 
	    rolutq_offset, rolutu_dim1, rolutu_offset, filut_dim1, 
	    filut_offset, i__1, i__2, i__3, i__4;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal), acos(doublereal), sqrt(
	    doublereal), cos(doublereal), sin(doublereal);

    /* Local variables */
    doublereal roiavion[4], roqavion[4], rouavion[4], a, b, c__, d__, f, h__[
	    31];
    integer i__, j, k, l, m;
    doublereal x, y, z__, a1, d1, g1, i1[1581]	/* was [31][51] */, i2[1581]	
	    /* was [31][51] */, i3[51], i4[51];
    extern /* Subroutine */ int aero_prof__(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *);
    doublereal q1[1581]	/* was [31][51] */, q2[1581]	/* was [31][51] */, 
	    q3[51], q4[51], u1[1581]	/* was [31][51] */, u2[1581]	/* 
	    was [31][51] */, u3[51], u4[51], y1, r1;
    extern /* Subroutine */ int kernelpol_(integer *, integer *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal ca, dd, ha, ch[31];
    integer ig;
    doublereal bp[1326]	/* was [26][51] */;
    integer jj;
    doublereal ta, cr, in[153]	/* was [3][51] */, gr[1326]	/* was [26][
	    51] */, hr, gt[1326]	/* was [26][51] */, th, pi, qn[153]	
	    /* was [3][51] */;
    integer it;
    doublereal un[153]	/* was [3][51] */, tr;
    integer is;
    doublereal xx, zx, yy, ii1, ii2, sa1, sa2, sb1, sb2, sc1, sc2, xi1, qq1, 
	    zi1, qq2, xi2, xq1, uu1, zq1, xt1, xt2, uu2, zu1, xq2, xu1, xu2;
    integer ifi;
    doublereal acu, xdb, phi, arr[1326]	/* was [26][51] */, tap, art[1326]	
	    /* was [26][51] */, att[1326]	/* was [26][51] */;
    integer isp, itp;
    doublereal ron, spl, piz, xpl[51], xrl[51], trp, xtl[51];
    integer snt, ntp;
    doublereal xpj, xrj, xtj, ypj, yrj, ytj, xpk, xrk, xtk, ypk, yrk, ytk, 
	    acu2;
    integer mum1;
    doublereal ppp1, ppp2, aaaa, altc[31], bpjk, xdel[31], ydel[31], gtjk, 
	    gtkj, grjk, grkj, taup, beta0, xmus, beta2, bpjmk;
    integer index, iborm;
    doublereal grjmk, grkmj, gtjmk, gtkmj, ratio, arrjk, artjk, artkj, attjk, 
	    gamma2, alpha2;
    integer iplane;
    extern /* Subroutine */ int discre_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    doublereal arrjmk, artjmk, artkmj, attjmk, phimul, delta0s;

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/* - to vary the number of quadratures */
/*  dimension for gauss integration */
/*<       integer mu,np,nfi >*/
/*<       real rm(-mu:mu),gb(-mu:mu),rp(np),xlphim(nfi) >*/
/*  dimension for os computation */
/*<       real xli(-mu:mu,np),xlq(-mu:mu,np),xlu(-mu:mu,np) >*/
/* array for sos computation */
/* THOMAS */
/*      double precision xpl(-mu:mu),xrl(-mu:mu),xtl(-mu:mu) */
/*      double precision bp(0:mu,-mu:mu),gr(0:mu,-mu:mu),gt(0:mu,-mu:mu) 
*/
/*      double precision arr(0:mu,-mu:mu),art(0:mu,-mu:mu) */
/*      double precision att(0:mu,-mu:mu),altc(0:nt) */
/*      double precision xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt) */
/*      real i1(0:nt,-mu:mu),q1(0:nt,-mu:mu) */
/*      real u1(0:nt,-mu:mu),i2(0:nt,-mu:mu) */
/*      real q2(0:nt,-mu:mu),u2(0:nt,-mu:mu) */
/*      real i3(-mu:mu),q3(-mu:mu),u3(-mu:mu) */
/*      real i4(-mu:mu),q4(-mu:mu),u4(-mu:mu) */
/*      real in(0:2,-mu:mu),qn(0:2,-mu:mu),un(0:2,-mu:mu) */
/*<       double precision xpl(-25:25),xrl(-25:25),xtl(-25:25) >*/
/*<       double precision bp(0:25,-25:25),gr(0:25,-25:25),gt(0:25,-25:25) >*/
/*<       double precision arr(0:25,-25:25),art(0:25,-25:25) >*/
/*<       double precision att(0:25,-25:25),altc(0:30) >*/
/*<       double precision xdel(0:30),ydel(0:30),ch(0:30),h(0:30) >*/
/*<       real i1(0:30,-25:25),q1(0:30,-25:25) >*/
/*<       real u1(0:30,-25:25),i2(0:30,-25:25) >*/
/*<       real q2(0:30,-25:25),u2(0:30,-25:25) >*/
/*<       real i3(-25:25),q3(-25:25),u3(-25:25) >*/
/*<       real i4(-25:25),q4(-25:25),u4(-25:25) >*/
/*<       real in(0:2,-25:25),qn(0:2,-25:25),un(0:2,-25:25) >*/
/*<       real roIavion(-1:2),roQavion(-1:2),roUavion(-1:2) >*/
/* CCC Begin Variable for Look up table generation */
/* azimuth or scattering angle variable for LUT computation (rolut) */
/*azimuth table for look up table computation (filut), nb of fi in each ca
se (nfilut)*/
/*<       real    luttv,lutmuv,iscama,iscami,its,scaa >*/
/*<       integer nbisca >*/
/*<       real    rolut(mu,41),rolutq(mu,41),rolutu(mu,41) >*/
/*<       real    filut(mu,41) >*/
/*<       integer nfilut(mu) >*/
/* CCC End Variable for Look up table generation */
/*<       real tamoy,trmoy,pizmoy >*/
/*<       real tamoyp,trmoyp,palt,phirad >*/
/*<       real delta,sigma >*/
/*<       double precision hr,ta,tr,trp >*/
/*<       double precision ppp2,ppp1,ca,cr,ratio >*/
/*<       double precision tap,piz,acu,acu2,ha,xmus,zx,yy,dd >*/
/*<       double precision taup,th,xt1,xt2,pi,phi,aaaa,ron,spl >*/
/*<       double precision sa1,sa2,sb1,sb2,sc1,sc2 >*/
/*<       double precision beta0,beta2,gamma2,alpha2 >*/
/*<       double precision zi1,zq1,zu1,c,f,d,y >*/
/*<       double precision a1,d1,g1,y1,r1,delta0s >*/
/*<       integer snt >*/
/*<       integer nt,iwr,iplane,mum1,ntp,j,it,itp,i,l,m,iborm >*/
/*<       integer is,isp,ig,k,jj,index >*/
/*<       logical ier >*/
/*<       double precision xx,xdb,z,x,a,b >*/
/*<       double precision bpjk,bpjmk,gtjk,gtjmk,gtkj,gtkmj,grjk,grjmk >*/
/*<       double precision grkj,grkmj,arrjk,arrjmk,artjk,artjmk,artkj >*/
/*<       double precision artkmj,attjk,attjmk >*/
/*<       double precision ii1,ii2,qq1,qq2,uu1,uu2,xi1,xi2,xq1,xq2,xu1,xu2 >*/
/*<       double precision xpj,xrj,xtj,ypj,yrj,ytj,xpk,xrk,xtk,ypk,yrk,ytk >*/
/*<       integer igmax,iaer_prof >*/
/*<       common/sixs_del/delta,sigma >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       common /multorder/ igmax >*/
/* the optical thickness above plane are recomputed to give o.t above pla 
*/
/*<       snt=nt >*/
    /* Parameter adjustments */
    rolutu_dim1 = *mu;
    rolutu_offset = rolutu_dim1 + 1;
    rolutu -= rolutu_offset;
    rolutq_dim1 = *mu;
    rolutq_offset = rolutq_dim1 + 1;
    rolutq -= rolutq_offset;
    rolut_dim1 = *mu;
    rolut_offset = rolut_dim1 + 1;
    rolut -= rolut_offset;
    filut_dim1 = *mu;
    filut_offset = filut_dim1 + 1;
    filut -= filut_offset;
    --nfilut;
    gb_offset = -(*mu);
    gb -= gb_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;
    xlu_dim1 = *mu - (-(*mu)) + 1;
    xlu_offset = -(*mu) + xlu_dim1;
    xlu -= xlu_offset;
    xlq_dim1 = *mu - (-(*mu)) + 1;
    xlq_offset = -(*mu) + xlq_dim1;
    xlq -= xlq_offset;
    xli_dim1 = *mu - (-(*mu)) + 1;
    xli_offset = -(*mu) + xli_dim1;
    xli -= xli_offset;
    --rp;
    --xlphim;

    /* Function Body */
    snt = *nt;
/*<       hr=8.0 >*/
    hr = 8.;
/*<       ta=tamoy >*/
    ta = *tamoy;
/*<       tr=trmoy >*/
    tr = *trmoy;
/*<       trp=trmoy-trmoyp >*/
    trp = *trmoy - *trmoyp;
/*<       tap=tamoy-tamoyp >*/
    tap = *tamoy - *tamoyp;
/*<       piz=pizmoy >*/
    piz = *pizmoy;
/*<       iplane=0 >*/
    iplane = 0;
/*<       acu=1.e-20 >*/
    acu = 1e-20;
/*<       acu2=1.e-4 >*/
    acu2 = 1e-4;
/*<       mum1=mu-1 >*/
    mum1 = *mu - 1;
/* if plane observations recompute scale height for aerosol knowing: */
/* the aerosol optical depth as measure from the plane 	= tamoyp */
/* the rayleigh scale height = 			= hr (8km) */
/* the rayleigh optical depth  at plane level 		= trmoyp */
/* the altitude of the plane 				= palt */
/* the rayleigh optical depth for total atmos		= trmoy */
/* the aerosol optical depth for total atmos		= tamoy */
/* if not plane observations then ha is equal to 2.0km */
/* ntp local variable: if ntp=nt     no plane observation selected */
/*                        ntp=nt-1   plane observation selected */
/*     it's a mixing rayleigh+aerosol */
/*<       if(palt.le.900..and.palt.gt.0.0) then >*/
    if (*palt <= 900. && *palt > 0.) {
/*<         if (tap.gt.1.e-03) then >*/
	if (tap > .001) {
/*<           ha=-palt/log(tap/ta) >*/
	    ha = -(*palt) / log(tap / ta);
/*<         else >*/
	} else {
/*<           ha=2. >*/
	    ha = 2.;
/*<         endif >*/
	}
/*<         ntp=nt-1 >*/
	ntp = *nt - 1;
/*<       else >*/
    } else {
/*<         ha=2.0 >*/
	ha = 2.;
/*<         ntp=nt >*/
	ntp = *nt;
/*<       endif >*/
    }

/*<       xmus=-rm(0) >*/
    xmus = -rm[0];

/* compute mixing rayleigh, aerosol */
/* case 1: pure rayleigh */
/* case 2: pure aerosol */
/* case 3: mixing rayleigh-aerosol */

/*<       if((ta.le.acu2).and.(tr.gt.ta)) then >*/
    if (ta <= acu2 && tr > ta) {
/*<         do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<           h(j)=j*tr/ntp >*/
	    h__[j] = j * tr / ntp;
/*<           ch(j)=exp(-h(j)/xmus)/2. >*/
	    ch[j] = exp(-h__[j] / xmus) / 2.;
/*<           ydel(j)=1.0 >*/
	    ydel[j] = 1.;
/*<           xdel(j)=0.0 >*/
	    xdel[j] = 0.;
/*<           if (j.eq.0) then >*/
	    if (j == 0) {
/*<             altc(j)=300. >*/
		altc[j] = 300.;
/*<           else >*/
	    } else {
/*<             altc(j)=-log(h(j)/tr)*hr >*/
		altc[j] = -log(h__[j] / tr) * hr;
/*<           endif      >*/
	    }
/*<         enddo >*/
	}
/*<       endif >*/
    }
/*<       if((tr.le.acu2).and.(ta.gt.tr)) then >*/
    if (tr <= acu2 && ta > tr) {
/*<         do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<           h(j)=j*ta/ntp >*/
	    h__[j] = j * ta / ntp;
/*<           ch(j)=exp(-h(j)/xmus)/2. >*/
	    ch[j] = exp(-h__[j] / xmus) / 2.;
/*<           ydel(j)=0.0 >*/
	    ydel[j] = 0.;
/*<           xdel(j)=piz >*/
	    xdel[j] = piz;
/*<           if (j.eq.0) then >*/
	    if (j == 0) {
/*<             altc(j)=300. >*/
		altc[j] = 300.;
/*<           else >*/
	    } else {
/*<             altc(j)=-log(h(j)/ta)*ha >*/
		altc[j] = -log(h__[j] / ta) * ha;
/*<           endif >*/
	    }
/*        write(6,901) j,altc(j),h(j) */
/*<         enddo >*/
	}
/*<       endif >*/
    }

/*<       if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.0)then >*/
    if (tr > acu2 && ta > acu2 && *iaer_prof__ == 0) {
/*<         ydel(0)=1.0 >*/
	ydel[0] = 1.;
/*<         xdel(0)=0.0 >*/
	xdel[0] = 0.;
/*<         h(0)=0. >*/
	h__[0] = 0.;
/*<         ch(0)=0.5 >*/
	ch[0] = .5;
/*<         altc(0)=300. >*/
	altc[0] = 300.;
/*<         zx=300. >*/
	zx = 300.;
/*<         iplane=0 >*/
	iplane = 0;
/*<         do 14 it=0,ntp >*/
	i__1 = ntp;
	for (it = 0; it <= i__1; ++it) {
/*<           if (it.eq.0) then >*/
	    if (it == 0) {
/*<             yy=0. >*/
		yy = 0.;
/*<             dd=0. >*/
		dd = 0.;
/*<             goto 111 >*/
		goto L111;
/*<           endif >*/
	    }
/*<           yy=h(it-1) >*/
	    yy = h__[it - 1];
/*<           dd=ydel(it-1) >*/
	    dd = ydel[it - 1];
/*<  111      ppp2=300.0 >*/
L111:
	    ppp2 = 300.;
/*<           ppp1=0.0 >*/
	    ppp1 = 0.;
/*<           itp=it >*/
	    itp = it;
/*<           call discre(ta,ha,tr,hr,itp,ntp,yy,dd,ppp2,ppp1,zx) >*/
	    discre_(&ta, &ha, &tr, &hr, &itp, &ntp, &yy, &dd, &ppp2, &ppp1, &
		    zx);
/*<           if(ier)return >*/
	    if (sixs_ier__1.ier) {
		return 0;
	    }
/*<           xx=-zx/ha >*/
	    xx = -zx / ha;
/*<           if (xx.le.-20) then >*/
	    if (xx <= -20.) {
/*<             ca=0. >*/
		ca = 0.;
/*<           else >*/
	    } else {
/*<             ca=ta*dexp(xx) >*/
		ca = ta * exp(xx);
/*<           endif >*/
	    }
/*<           xx=-zx/hr >*/
	    xx = -zx / hr;
/*<           cr=tr*dexp(xx) >*/
	    cr = tr * exp(xx);
/*<           h(it)=cr+ca >*/
	    h__[it] = cr + ca;
/*<           altc(it)=zx >*/
	    altc[it] = zx;
/*<           ch(it)=exp(-h(it)/xmus)/2. >*/
	    ch[it] = exp(-h__[it] / xmus) / 2.;
/*<           cr=cr/hr >*/
	    cr /= hr;
/*<           ca=ca/ha >*/
	    ca /= ha;
/*<           ratio=cr/(cr+ca) >*/
	    ratio = cr / (cr + ca);
/*<           xdel(it)=(1.e+00-ratio)*piz >*/
	    xdel[it] = (1. - ratio) * piz;
/*<           ydel(it)=ratio >*/
	    ydel[it] = ratio;
/*<   14    continue >*/
/* L14: */
	}
/*<        endif >*/
    }
/*<  901    Format(i2,f10.5,f10.5,f10.5) >*/
/* L901: */
/*<       if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.1)then >*/
    if (tr > acu2 && ta > acu2 && *iaer_prof__ == 1) {
/*<    >*/
	aero_prof__(&ta, &piz, &tr, &hr, &ntp, &xmus, h__, ch, ydel, xdel, 
		altc);
/*<       endif >*/
    }
/* update plane layer if necessary */
/*<       if (ntp.eq.(nt-1)) then >*/
    if (ntp == *nt - 1) {
/* compute position of the plane layer */
/*<         taup=tap+trp >*/
	taup = tap + trp;
/*<         iplane=-1 >*/
	iplane = -1;
/*<         do i=0,ntp >*/
	i__1 = ntp;
	for (i__ = 0; i__ <= i__1; ++i__) {
/*<           if (taup.ge.h(i)) iplane=i >*/
	    if (taup >= h__[i__]) {
		iplane = i__;
	    }
/*<         enddo >*/
	}
/* update the layer from the end to the position to update if necessar
y */
/*<         th=0.0005 >*/
	th = 5e-4;
/*<         xt1=abs(h(iplane)-taup) >*/
	xt1 = (d__1 = h__[iplane] - taup, abs(d__1));
/*<         xt2=abs(h(iplane+1)-taup) >*/
	xt2 = (d__1 = h__[iplane + 1] - taup, abs(d__1));
/*<         if ((xt1.gt.th).and.(xt2.gt.th)) then >*/
	if (xt1 > th && xt2 > th) {
/*<           do i=nt,iplane+1,-1 >*/
	    i__1 = iplane + 1;
	    for (i__ = *nt; i__ >= i__1; --i__) {
/*<             xdel(i)=xdel(i-1) >*/
		xdel[i__] = xdel[i__ - 1];
/*<             ydel(i)=ydel(i-1) >*/
		ydel[i__] = ydel[i__ - 1];
/*<             h(i)=h(i-1) >*/
		h__[i__] = h__[i__ - 1];
/*<             altc(i)=altc(i-1) >*/
		altc[i__] = altc[i__ - 1];
/*<             ch(i)=ch(i-1) >*/
		ch[i__] = ch[i__ - 1];
/*<           enddo >*/
	    }
/*<         else >*/
	} else {
/*<           nt=ntp >*/
	    *nt = ntp;
/*<           if (xt2.lt.xt1) iplane=iplane+1 >*/
	    if (xt2 < xt1) {
		++iplane;
	    }
/*<         endif >*/
	}
/*<         h(iplane)=taup >*/
	h__[iplane] = taup;
/*<         if ( tr.gt.acu2.and.ta.gt.acu2) then >*/
	if (tr > acu2 && ta > acu2) {
/*<           ca=ta*exp(-palt/ha) >*/
	    ca = ta * exp(-(*palt) / ha);
/*<           cr=tr*exp(-palt/hr) >*/
	    cr = tr * exp(-(*palt) / hr);
/*<           h(iplane)=ca+cr >*/
	    h__[iplane] = ca + cr;
/*<           cr=cr/hr >*/
	    cr /= hr;
/*<           ca=ca/ha >*/
	    ca /= ha;
/*<           ratio=cr/(cr+ca) >*/
	    ratio = cr / (cr + ca);
/*<           xdel(iplane)=(1.e+00-ratio)*piz >*/
	    xdel[iplane] = (1. - ratio) * piz;
/*<           ydel(iplane)=ratio >*/
	    ydel[iplane] = ratio;
/*<           altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<           ch(iplane)=exp(-h(iplane)/xmus)/2. >*/
	    ch[iplane] = exp(-h__[iplane] / xmus) / 2.;
/*<         endif >*/
	}
/*<         if ( tr.gt.acu2.and.ta.le.acu2) then >*/
	if (tr > acu2 && ta <= acu2) {
/*<           ydel(iplane)=1. >*/
	    ydel[iplane] = 1.;
/*<           xdel(iplane)=0. >*/
	    xdel[iplane] = 0.;
/*<           altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<         endif >*/
	}
/*<         if ( tr.le.acu2.and.ta.gt.acu2) then >*/
	if (tr <= acu2 && ta > acu2) {
/*<           ydel(iplane)=0. >*/
	    ydel[iplane] = 0.;
/*<           xdel(iplane)=1.*piz >*/
	    xdel[iplane] = piz * 1.;
/*<           altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<         endif >*/
	}
/*<       endif >*/
    }


/*     print *,ha,hr,palt,ta,tr,tap,trp */
/*     do i=0,nt */
/*       print *,i,h(i),ch(i),xdel(i),ydel(i),altc(i) */
/*     enddo */

/*<       pi=acos(-1.) >*/
    pi = acos(-1.);
/*<       phi=phirad >*/
    phi = *phirad;
/*<       do 716 l=1,np >*/
    i__1 = *np;
    for (l = 1; l <= i__1; ++l) {
/*<         do 716 m=-mu,mu >*/
	i__2 = *mu;
	for (m = -(*mu); m <= i__2; ++m) {
/*< 	  xli(m,l)=0. >*/
	    xli[m + l * xli_dim1] = 0.;
/*< 	  xlq(m,l)=0. >*/
	    xlq[m + l * xlq_dim1] = 0.;
/*< 	  xlu(m,l)=0. >*/
	    xlu[m + l * xlu_dim1] = 0.;
/*<  716  continue >*/
/* L716: */
	}
    }
/*<       do ifi=1,nfi >*/
    i__2 = *nfi;
    for (ifi = 1; ifi <= i__2; ++ifi) {
/*<       xlphim(ifi)=0. >*/
	xlphim[ifi] = 0.;
/*<       enddo >*/
    }
/* CC initialization of look up table variable */
/*<       do i=1,mu >*/
    i__2 = *mu;
    for (i__ = 1; i__ <= i__2; ++i__) {
/*<       do j=1,41 >*/
	for (j = 1; j <= 41; ++j) {
/*<       rolut(i,j)=0. >*/
	    rolut[i__ + j * rolut_dim1] = 0.;
/*<       rolutq(i,j)=0. >*/
	    rolutq[i__ + j * rolutq_dim1] = 0.;
/*<       rolutu(i,j)=0. >*/
	    rolutu[i__ + j * rolutu_dim1] = 0.;
/*<       enddo >*/
	}
/*<       enddo >*/
    }

/*     ************ incident angle mus ******* */


/*<       aaaa=delta/(2-delta) >*/
    aaaa = sixs_del__1.delta / (2 - sixs_del__1.delta);
/*<       ron=(1-aaaa)/(1+2*aaaa) >*/
    ron = (1 - aaaa) / (aaaa * 2 + 1);
/*  TEST FOR COMPARING TO SIMPLE APPROXIMATION */
/*     WRITE(6,*) "WARNING DEPOLARIZATION=0" */
/*     ron=1.0 */

/*     rayleigh phase function */

/*<       beta0=1. >*/
    beta0 = 1.;
/*<       beta2=0.5*ron >*/
    beta2 = ron * .5;
/*<       gamma2=-ron*sqrt(1.5) >*/
    gamma2 = -ron * sqrt(1.5);
/*<       alpha2=3.*ron >*/
    alpha2 = ron * 3.;

/*     fourier decomposition */

/*<       do 17 j=-mu,mu >*/
    i__2 = *mu;
    for (j = -(*mu); j <= i__2; ++j) {
/*<         i4(j)=0. >*/
	i4[j + 25] = 0.;
/*<         q4(j)=0. >*/
	q4[j + 25] = 0.;
/*<         u4(j)=0. >*/
	u4[j + 25] = 0.;
/*<    17 continue >*/
/* L17: */
    }
/*<       iborm=nquad >*/
    iborm = num_quad__1.nquad;
/*<       if (ta.le.acu2) iborm=2 >*/
    if (ta <= acu2) {
	iborm = 2;
    }
/*<       if( abs (xmus-1.000000) .lt.1.e-06)iborm=0 >*/
    if ((d__1 = xmus - 1., abs(d__1)) < 1e-6) {
	iborm = 0;
    }
/*<       do 24 is=0,iborm >*/
    i__2 = iborm;
    for (is = 0; is <= i__2; ++is) {

/*    primary scattering */

/*<         ig=1 >*/
	ig = 1;
/*<         do k=0,3 >*/
	for (k = 0; k <= 3; ++k) {
/*<           roIavion(k-1)=0. >*/
	    roiavion[k] = 0.;
/*<           roQavion(k-1)=0. >*/
	    roqavion[k] = 0.;
/*<           roUavion(k-1)=0. >*/
	    rouavion[k] = 0.;
/*<         enddo >*/
	}
/*<         do 16 j=-mu,mu >*/
	i__1 = *mu;
	for (j = -(*mu); j <= i__1; ++j) {
/*<           i3(j)=0. >*/
	    i3[j + 25] = 0.;
/*<           q3(j)=0. >*/
	    q3[j + 25] = 0.;
/*<           u3(j)=0. >*/
	    u3[j + 25] = 0.;
/*<    16   continue >*/
/* L16: */
	}

/*     kernel computations + mixture rayleigh(1)-aerosol(2) */

/*<         isp=is >*/
	isp = is;
/*<         call kernelpol(isp,mu,rm,xpl,xrl,xtl,bp,gr,gt,arr,art,att) >*/
	kernelpol_(&isp, mu, &rm[rm_offset], xpl, xrl, xtl, bp, gr, gt, arr, 
		art, att);
/*<         if(is.gt.0)beta0=0. >*/
	if (is > 0) {
	    beta0 = 0.;
	}
/*<         do 100 j=-mu,mu >*/
	i__1 = *mu;
	for (j = -(*mu); j <= i__1; ++j) {
/*<           if(is-2)200,200,201 >*/
	    if (is - 2 <= 0) {
		goto L200;
	    } else {
		goto L201;
	    }
/*<  200      spl=xpl(0) >*/
L200:
	    spl = xpl[25];
/*<           sa1=beta0+beta2*xpl(j)*spl >*/
	    sa1 = beta0 + beta2 * xpl[j + 25] * spl;
/*<           sa2=bp(0,j) >*/
	    sa2 = bp[j * 26 + 650];
/*<           sb1=gamma2*xrl(j)*spl >*/
	    sb1 = gamma2 * xrl[j + 25] * spl;
/*<           sb2=gr(0,j) >*/
	    sb2 = gr[j * 26 + 650];
/*<           sc1=gamma2*xtl(j)*spl >*/
	    sc1 = gamma2 * xtl[j + 25] * spl;
/*<           sc2=gt(0,j) >*/
	    sc2 = gt[j * 26 + 650];
/*<           goto 202 >*/
	    goto L202;
/*<  201      sa2=bp(0,j) >*/
L201:
	    sa2 = bp[j * 26 + 650];
/*<           sa1=0. >*/
	    sa1 = 0.;
/*<           sb2=gr(0,j) >*/
	    sb2 = gr[j * 26 + 650];
/*<           sb1=0. >*/
	    sb1 = 0.;
/*<           sc2=gt(0,j) >*/
	    sc2 = gt[j * 26 + 650];
/*<           sc1=0. >*/
	    sc1 = 0.;

/*     primary scattering source function at every level within th
e layer */

/*<  202      do 101 k=0,nt >*/
L202:
	    i__3 = *nt;
	    for (k = 0; k <= i__3; ++k) {
/*<             c=ch(k) >*/
		c__ = ch[k];
/*<             a=ydel(k) >*/
		a = ydel[k];
/*<             b=xdel(k) >*/
		b = xdel[k];
/*<             i2(k,j)=c*(sa2*b+sa1*a) >*/
		i2[k + j * 31 + 775] = c__ * (sa2 * b + sa1 * a);
/*<             q2(k,j)=c*(sb2*b+sb1*a) >*/
		q2[k + j * 31 + 775] = c__ * (sb2 * b + sb1 * a);
/*<             u2(k,j)=-c*(sc2*b+sc1*a) >*/
		u2[k + j * 31 + 775] = -c__ * (sc2 * b + sc1 * a);
/*<   101     continue >*/
/* L101: */
	    }
/*<   100   continue >*/
/* L100: */
	}

/*     vertical integration, primary upward radiation */

/*<         do 108 k=1,mu >*/
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<            i1(nt,k)=0. >*/
	    i1[*nt + k * 31 + 775] = 0.;
/*<            q1(nt,k)=0. >*/
	    q1[*nt + k * 31 + 775] = 0.;
/*<            u1(nt,k)=0. >*/
	    u1[*nt + k * 31 + 775] = 0.;
/*<            zi1=i1(nt,k) >*/
	    zi1 = i1[*nt + k * 31 + 775];
/*<            zq1=q1(nt,k) >*/
	    zq1 = q1[*nt + k * 31 + 775];
/*<            zu1=u1(nt,k) >*/
	    zu1 = u1[*nt + k * 31 + 775];
/*<            yy=rm(k) >*/
	    yy = rm[k];
/*<            do 108 i=nt-1,0,-1 >*/
	    for (i__ = *nt - 1; i__ >= 0; --i__) {
/*<               jj=i+1 >*/
		jj = i__ + 1;
/*<               f=h(jj)-h(i) >*/
		f = h__[jj] - h__[i__];
/*<               c=exp(-f/yy) >*/
		c__ = exp(-f / yy);
/*<               d=1.0e+00-c >*/
		d__ = 1. - c__;
/*<               xx=h(i)-h(jj)*c >*/
		xx = h__[i__] - h__[jj] * c__;
/*<               a=(i2(jj,k)-i2(i,k))/f >*/
		a = (i2[jj + k * 31 + 775] - i2[i__ + k * 31 + 775]) / f;
/*<               b=i2(i,k)-a*h(i) >*/
		b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<               zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<               i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<               a=(q2(jj,k)-q2(i,k))/f >*/
		a = (q2[jj + k * 31 + 775] - q2[i__ + k * 31 + 775]) / f;
/*<               b=q2(i,k)-a*h(i) >*/
		b = q2[i__ + k * 31 + 775] - a * h__[i__];
/*<               zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zq1 = c__ * zq1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<               q1(i,k)=zq1 >*/
		q1[i__ + k * 31 + 775] = zq1;
/*<               a=(u2(jj,k)-u2(i,k))/f >*/
		a = (u2[jj + k * 31 + 775] - u2[i__ + k * 31 + 775]) / f;
/*<               b=u2(i,k)-a*h(i) >*/
		b = u2[i__ + k * 31 + 775] - a * h__[i__];
/*<               zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zu1 = c__ * zu1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<               u1(i,k)=zu1 >*/
		u1[i__ + k * 31 + 775] = zu1;
/*<   108   continue >*/
/* L108: */
	    }
	}

/*     vertical integration, primary downward radiation */

/*<         do 109 k=-mu,-1 >*/
	for (k = -(*mu); k <= -1; ++k) {
/*<           i1(0,k)=0. >*/
	    i1[k * 31 + 775] = 0.;
/*<           q1(0,k)=0. >*/
	    q1[k * 31 + 775] = 0.;
/*<           u1(0,k)=0. >*/
	    u1[k * 31 + 775] = 0.;
/*<           zi1=i1(0,k) >*/
	    zi1 = i1[k * 31 + 775];
/*<           zq1=q1(0,k) >*/
	    zq1 = q1[k * 31 + 775];
/*<           zu1=u1(0,k) >*/
	    zu1 = u1[k * 31 + 775];
/*<           yy=rm(k) >*/
	    yy = rm[k];
/*<           do 109 i=1,nt >*/
	    i__1 = *nt;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/*<             jj=i-1 >*/
		jj = i__ - 1;
/*<             f=h(i)-h(jj) >*/
		f = h__[i__] - h__[jj];
/*<             c=exp(f/yy) >*/
		c__ = exp(f / yy);
/*<             d=1.0e+00-c >*/
		d__ = 1. - c__;
/*<             xx=h(i)-h(jj)*c >*/
		xx = h__[i__] - h__[jj] * c__;
/*<             a=(i2(i,k)-i2(jj,k))/f >*/
		a = (i2[i__ + k * 31 + 775] - i2[jj + k * 31 + 775]) / f;
/*<             b=i2(i,k)-a*h(i) >*/
		b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<             a=(q2(i,k)-q2(jj,k))/f >*/
		a = (q2[i__ + k * 31 + 775] - q2[jj + k * 31 + 775]) / f;
/*<             b=q2(i,k)-a*h(i) >*/
		b = q2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zq1 = c__ * zq1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             q1(i,k)=zq1 >*/
		q1[i__ + k * 31 + 775] = zq1;
/*<             a=(u2(i,k)-u2(jj,k))/f >*/
		a = (u2[i__ + k * 31 + 775] - u2[jj + k * 31 + 775]) / f;
/*<             b=u2(i,k)-a*h(i) >*/
		b = u2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zu1 = c__ * zu1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             u1(i,k)=zu1 >*/
		u1[i__ + k * 31 + 775] = zu1;
/*<   109   continue >*/
/* L109: */
	    }
	}

/*     in(2,?) is inialized with scattering computed at n-2 */
/*     i3 is inialized with primary scattering */

/*<         do 20 k=-mu,mu >*/
	i__1 = *mu;
	for (k = -(*mu); k <= i__1; ++k) {
/*<           if(k) 21,20,23 >*/
	    if (k < 0) {
		goto L21;
	    } else if (k == 0) {
		goto L20;
	    } else {
		goto L23;
	    }
/*<    21     index=nt >*/
L21:
	    index = *nt;
/*<           go to 25 >*/
	    goto L25;
/*<    23     index=0 >*/
L23:
	    index = 0;
/*<    25     continue >*/
L25:
/*<           in(1,k)=i1(index,k) >*/
	    in[k * 3 + 76] = i1[index + k * 31 + 775];
/*<           in(2,k)=i1(index,k) >*/
	    in[k * 3 + 77] = i1[index + k * 31 + 775];
/*<           i3(k)=i1(index,k) >*/
	    i3[k + 25] = i1[index + k * 31 + 775];
/*<           qn(1,k)=q1(index,k) >*/
	    qn[k * 3 + 76] = q1[index + k * 31 + 775];
/*<           qn(2,k)=q1(index,k) >*/
	    qn[k * 3 + 77] = q1[index + k * 31 + 775];
/*<           q3(k)=q1(index,k) >*/
	    q3[k + 25] = q1[index + k * 31 + 775];
/*<           un(1,k)=u1(index,k) >*/
	    un[k * 3 + 76] = u1[index + k * 31 + 775];
/*<           un(2,k)=u1(index,k) >*/
	    un[k * 3 + 77] = u1[index + k * 31 + 775];
/*<           u3(k)=u1(index,k) >*/
	    u3[k + 25] = u1[index + k * 31 + 775];
/*<    20   continue >*/
L20:
	    ;
	}
/*<         roIavion(2)=i1(iplane,mu) >*/
	roiavion[3] = i1[iplane + *mu * 31 + 775];
/*<         roIavion(-1)=i1(iplane,mu) >*/
	roiavion[0] = i1[iplane + *mu * 31 + 775];
/*<         roQavion(2)=q1(iplane,mu) >*/
	roqavion[3] = q1[iplane + *mu * 31 + 775];
/*<         roQavion(-1)=q1(iplane,mu) >*/
	roqavion[0] = q1[iplane + *mu * 31 + 775];
/*<         roUavion(2)=u1(iplane,mu) >*/
	rouavion[3] = u1[iplane + *mu * 31 + 775];
/*<         roUavion(-1)=u1(iplane,mu) >*/
	rouavion[0] = u1[iplane + *mu * 31 + 775];

/*     loop on successive order */

/*<   503   ig=ig+1 >*/
L503:
	++ig;
/*       WRITE(6,*) "WARNING ONLY PRIMARY SCATTERING" */
/*       goto 505 */

/*     successive orders */

/*     multiple scattering source function at every level within the l
aye */

/*     if is < ou = 2 kernels are a mixing of aerosols and molecules k
ern */
/*     if is >2 aerosols kernels only */

/*<         if(is-2)210,210,211 >*/
	if (is - 2 <= 0) {
	    goto L210;
	} else {
	    goto L211;
	}
/*<   210   do455 k=1,mu >*/
L210:
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<           xpk=xpl(k) >*/
	    xpk = xpl[k + 25];
/*<           xrk=xrl(k) >*/
	    xrk = xrl[k + 25];
/*<           xtk=xtl(k) >*/
	    xtk = xtl[k + 25];
/*<           ypk=xpl(-k) >*/
	    ypk = xpl[-k + 25];
/*<           yrk=xrl(-k) >*/
	    yrk = xrl[-k + 25];
/*<           ytk=xtl(-k) >*/
	    ytk = xtl[-k + 25];
/*<           do 455 i=0,nt >*/
	    i__3 = *nt;
	    for (i__ = 0; i__ <= i__3; ++i__) {
/*<             ii1=0. >*/
		ii1 = 0.;
/*<             ii2=0. >*/
		ii2 = 0.;
/*<             qq1=0. >*/
		qq1 = 0.;
/*<             qq2=0. >*/
		qq2 = 0.;
/*<             uu1=0. >*/
		uu1 = 0.;
/*<             uu2=0. >*/
		uu2 = 0.;
/*<             x=xdel(i) >*/
		x = xdel[i__];
/*<             y=ydel(i) >*/
		y = ydel[i__];
/*<             do477 j=1,mu >*/
		i__4 = *mu;
		for (j = 1; j <= i__4; ++j) {
/*<               z=gb(j) >*/
		    z__ = gb[j];
/*<               xpj=xpl(j) >*/
		    xpj = xpl[j + 25];
/*<               xrj=xrl(j) >*/
		    xrj = xrl[j + 25];
/*<               xtj=xtl(j) >*/
		    xtj = xtl[j + 25];
/*<               ypj=xpl(-j) >*/
		    ypj = xpl[-j + 25];
/*<               yrj=xrl(-j) >*/
		    yrj = xrl[-j + 25];
/*<               ytj=xtl(-j) >*/
		    ytj = xtl[-j + 25];
/*<               xi1=i1(i,j) >*/
		    xi1 = i1[i__ + j * 31 + 775];
/*<               xi2=i1(i,-j) >*/
		    xi2 = i1[i__ + -j * 31 + 775];
/*<               xq1=q1(i,j) >*/
		    xq1 = q1[i__ + j * 31 + 775];
/*<               xq2=q1(i,-j) >*/
		    xq2 = q1[i__ + -j * 31 + 775];
/*<               xu1=u1(i,j) >*/
		    xu1 = u1[i__ + j * 31 + 775];
/*<               xu2=u1(i,-j) >*/
		    xu2 = u1[i__ + -j * 31 + 775];
/*<               bpjk=bp(j,k)*x+y*(beta0+beta2*xpj*xpk) >*/
		    bpjk = bp[j + k * 26 + 650] * x + y * (beta0 + beta2 * 
			    xpj * xpk);
/*<               bpjmk=bp(j,-k)*x+y*(beta0+beta2*xpj*ypk) >*/
		    bpjmk = bp[j + -k * 26 + 650] * x + y * (beta0 + beta2 * 
			    xpj * ypk);
/*<               gtjk=gt(j,k)*x+y*gamma2*xpj*xtk >*/
		    gtjk = gt[j + k * 26 + 650] * x + y * gamma2 * xpj * xtk;
/*<               gtjmk=gt(j,-k)*x+y*gamma2*xpj*ytk >*/
		    gtjmk = gt[j + -k * 26 + 650] * x + y * gamma2 * xpj * 
			    ytk;
/*<               gtkj=gt(k,j)*x+y*gamma2*xpk*xtj >*/
		    gtkj = gt[k + j * 26 + 650] * x + y * gamma2 * xpk * xtj;
/*<               gtkmj=gt(k,-j)*x+y*gamma2*xpk*ytj >*/
		    gtkmj = gt[k + -j * 26 + 650] * x + y * gamma2 * xpk * 
			    ytj;
/*<               grjk=gr(j,k)*x+y*gamma2*xpj*xrk >*/
		    grjk = gr[j + k * 26 + 650] * x + y * gamma2 * xpj * xrk;
/*<               grjmk=gr(j,-k)*x+y*gamma2*xpj*yrk >*/
		    grjmk = gr[j + -k * 26 + 650] * x + y * gamma2 * xpj * 
			    yrk;
/*<               grkj=gr(k,j)*x+y*gamma2*xpk*xrj >*/
		    grkj = gr[k + j * 26 + 650] * x + y * gamma2 * xpk * xrj;
/*<               grkmj=gr(k,-j)*x+y*gamma2*xpk*yrj >*/
		    grkmj = gr[k + -j * 26 + 650] * x + y * gamma2 * xpk * 
			    yrj;
/*<               arrjk=arr(j,k)*x+y*alpha2*xrj*xrk >*/
		    arrjk = arr[j + k * 26 + 650] * x + y * alpha2 * xrj * 
			    xrk;
/*<               arrjmk=arr(j,-k)*x+y*alpha2*xrj*yrk >*/
		    arrjmk = arr[j + -k * 26 + 650] * x + y * alpha2 * xrj * 
			    yrk;
/*<               artjk=art(j,k)*x+y*alpha2*xtj*xrk >*/
		    artjk = art[j + k * 26 + 650] * x + y * alpha2 * xtj * 
			    xrk;
/*<               artjmk=art(j,-k)*x+y*alpha2*xtj*yrk >*/
		    artjmk = art[j + -k * 26 + 650] * x + y * alpha2 * xtj * 
			    yrk;
/*<               artkj=art(k,j)*x+y*alpha2*xtk*xrj >*/
		    artkj = art[k + j * 26 + 650] * x + y * alpha2 * xtk * 
			    xrj;
/*<               artkmj=art(k,-j)*x+y*alpha2*xtk*yrj >*/
		    artkmj = art[k + -j * 26 + 650] * x + y * alpha2 * xtk * 
			    yrj;
/*<               attjk=att(j,k)*x+y*alpha2*xtj*xtk >*/
		    attjk = att[j + k * 26 + 650] * x + y * alpha2 * xtj * 
			    xtk;
/*<               attjmk=att(j,-k)*x+y*alpha2*xtj*ytk >*/
		    attjmk = att[j + -k * 26 + 650] * x + y * alpha2 * xtj * 
			    ytk;
/*<               xdb=xi1*bpjk+xi2*bpjmk+xq1*grkj+xq2*grkmj >*/
		    xdb = xi1 * bpjk + xi2 * bpjmk + xq1 * grkj + xq2 * grkmj;
/*<               xdb=xdb-xu1*gtkj-xu2*gtkmj >*/
		    xdb = xdb - xu1 * gtkj - xu2 * gtkmj;
/*<               ii2=ii2+xdb*z >*/
		    ii2 += xdb * z__;
/*<               xdb=xi1*bpjmk+xi2*bpjk+xq1*grkmj+xq2*grkj >*/
		    xdb = xi1 * bpjmk + xi2 * bpjk + xq1 * grkmj + xq2 * grkj;
/*< 	      xdb=xdb+xu1*gtkmj+xu2*gtkj >*/
		    xdb = xdb + xu1 * gtkmj + xu2 * gtkj;
/*<               ii1=ii1+xdb*z >*/
		    ii1 += xdb * z__;
/*<               xdb=xi1*grjk+xi2*grjmk+xq1*arrjk+xq2*arrjmk >*/
		    xdb = xi1 * grjk + xi2 * grjmk + xq1 * arrjk + xq2 * 
			    arrjmk;
/*<               xdb=xdb-xu1*artjk+xu2*artjmk >*/
		    xdb = xdb - xu1 * artjk + xu2 * artjmk;
/*< 	      qq2=qq2+xdb*z >*/
		    qq2 += xdb * z__;
/*< 	      xdb=xi1*grjmk+xi2*grjk+xq1*arrjmk+xq2*arrjk >*/
		    xdb = xi1 * grjmk + xi2 * grjk + xq1 * arrjmk + xq2 * 
			    arrjk;
/*< 	      xdb=xdb-xu1*artjmk+xu2*artjk >*/
		    xdb = xdb - xu1 * artjmk + xu2 * artjk;
/*< 	      qq1=qq1+xdb*z >*/
		    qq1 += xdb * z__;
/*<               xdb=xi1*gtjk-xi2*gtjmk+xq1*artkj+xq2*artkmj >*/
		    xdb = xi1 * gtjk - xi2 * gtjmk + xq1 * artkj + xq2 * 
			    artkmj;
/*<               xdb=xdb-xu1*attjk-xu2*attjmk >*/
		    xdb = xdb - xu1 * attjk - xu2 * attjmk;
/*< 	      uu2=uu2-xdb*z >*/
		    uu2 -= xdb * z__;
/*<               xdb=xi1*gtjmk-xi2*gtjk-xq1*artkmj-xq2*artkj >*/
		    xdb = xi1 * gtjmk - xi2 * gtjk - xq1 * artkmj - xq2 * 
			    artkj;
/*<               xdb=xdb-xu1*attjmk-xu2*attjk >*/
		    xdb = xdb - xu1 * attjmk - xu2 * attjk;
/*< 	      uu1=uu1-xdb*z >*/
		    uu1 -= xdb * z__;
/*<  477        continue >*/
/* L477: */
		}
/*<             if (abs(ii2).lt.1.E-30) ii2=0. >*/
		if (abs(ii2) < 1e-30) {
		    ii2 = 0.;
		}
/*<             if (abs(ii1).lt.1.E-30) ii1=0. >*/
		if (abs(ii1) < 1e-30) {
		    ii1 = 0.;
		}
/*<             if (abs(qq2).lt.1.E-30) qq2=0. >*/
		if (abs(qq2) < 1e-30) {
		    qq2 = 0.;
		}
/*<             if (abs(qq1).lt.1.E-30) qq1=0. >*/
		if (abs(qq1) < 1e-30) {
		    qq1 = 0.;
		}
/*<             if (abs(uu2).lt.1.E-30) uu2=0. >*/
		if (abs(uu2) < 1e-30) {
		    uu2 = 0.;
		}
/*<             if (abs(uu1).lt.1.E-30) uu1=0. >*/
		if (abs(uu1) < 1e-30) {
		    uu1 = 0.;
		}
/*<             i2(i,k)=ii2 >*/
		i2[i__ + k * 31 + 775] = ii2;
/*<             i2(i,-k)=ii1 >*/
		i2[i__ + -k * 31 + 775] = ii1;
/*<             q2(i,k)=qq2 >*/
		q2[i__ + k * 31 + 775] = qq2;
/*<             q2(i,-k)=qq1 >*/
		q2[i__ + -k * 31 + 775] = qq1;
/*<             u2(i,k)=uu2 >*/
		u2[i__ + k * 31 + 775] = uu2;
/*<             u2(i,-k)=uu1 >*/
		u2[i__ + -k * 31 + 775] = uu1;
/*<  455    continue >*/
/* L455: */
	    }
	}
/*<         goto 213 >*/
	goto L213;
/*<  211    do45 k=1,mu >*/
L211:
	i__3 = *mu;
	for (k = 1; k <= i__3; ++k) {
/*<           do 45 i=0,nt >*/
	    i__1 = *nt;
	    for (i__ = 0; i__ <= i__1; ++i__) {
/*<             ii1=0. >*/
		ii1 = 0.;
/*<             ii2=0. >*/
		ii2 = 0.;
/*<             qq1=0. >*/
		qq1 = 0.;
/*<             qq2=0. >*/
		qq2 = 0.;
/*<             uu1=0. >*/
		uu1 = 0.;
/*<             uu2=0. >*/
		uu2 = 0.;
/*<             x=xdel(i) >*/
		x = xdel[i__];
/*<             do47 j=1,mu >*/
		i__4 = *mu;
		for (j = 1; j <= i__4; ++j) {
/*<               z=gb(j) >*/
		    z__ = gb[j];
/*<               xi1=i1(i,j) >*/
		    xi1 = i1[i__ + j * 31 + 775];
/*<               xi2=i1(i,-j) >*/
		    xi2 = i1[i__ + -j * 31 + 775];
/*<               xq1=q1(i,j) >*/
		    xq1 = q1[i__ + j * 31 + 775];
/*<               xq2=q1(i,-j) >*/
		    xq2 = q1[i__ + -j * 31 + 775];
/*<               xu1=u1(i,j) >*/
		    xu1 = u1[i__ + j * 31 + 775];
/*<               xu2=u1(i,-j) >*/
		    xu2 = u1[i__ + -j * 31 + 775];
/*<               bpjk=bp(j,k)*x >*/
		    bpjk = bp[j + k * 26 + 650] * x;
/*<               bpjmk=bp(j,-k)*x >*/
		    bpjmk = bp[j + -k * 26 + 650] * x;
/*<               gtjk=gt(j,k)*x >*/
		    gtjk = gt[j + k * 26 + 650] * x;
/*<               gtjmk=gt(j,-k)*x >*/
		    gtjmk = gt[j + -k * 26 + 650] * x;
/*<               gtkj=gt(k,j)*x >*/
		    gtkj = gt[k + j * 26 + 650] * x;
/*<               gtkmj=gt(k,-j)*x >*/
		    gtkmj = gt[k + -j * 26 + 650] * x;
/*<               grjk=gr(j,k)*x >*/
		    grjk = gr[j + k * 26 + 650] * x;
/*<               grjmk=gr(j,-k)*x >*/
		    grjmk = gr[j + -k * 26 + 650] * x;
/*<               grkj=gr(k,j)*x >*/
		    grkj = gr[k + j * 26 + 650] * x;
/*<               grkmj=gr(k,-j)*x >*/
		    grkmj = gr[k + -j * 26 + 650] * x;
/*< 	      arrjk=arr(j,k)*x >*/
		    arrjk = arr[j + k * 26 + 650] * x;
/*< 	      arrjmk=arr(j,-k)*x >*/
		    arrjmk = arr[j + -k * 26 + 650] * x;
/*< 	      artjk=art(j,k)*x >*/
		    artjk = art[j + k * 26 + 650] * x;
/*< 	      artjmk=art(j,-k)*x >*/
		    artjmk = art[j + -k * 26 + 650] * x;
/*< 	      artkj=art(k,j)*x >*/
		    artkj = art[k + j * 26 + 650] * x;
/*< 	      artkmj=art(k,-j)*x >*/
		    artkmj = art[k + -j * 26 + 650] * x;
/*< 	      attjk=att(j,k)*x >*/
		    attjk = att[j + k * 26 + 650] * x;
/*< 	      attjmk=att(j,-k)*x >*/
		    attjmk = att[j + -k * 26 + 650] * x;
/*<               xdb=xi1*bpjk+xi2*bpjmk+xq1*grkj+xq2*grkmj >*/
		    xdb = xi1 * bpjk + xi2 * bpjmk + xq1 * grkj + xq2 * grkmj;
/*<               xdb=xdb-xu1*gtkj-xu2*gtkmj >*/
		    xdb = xdb - xu1 * gtkj - xu2 * gtkmj;
/*<               ii2=ii2+xdb*z >*/
		    ii2 += xdb * z__;
/*<               xdb=xi1*bpjmk+xi2*bpjk+xq1*grkmj+xq2*grkj >*/
		    xdb = xi1 * bpjmk + xi2 * bpjk + xq1 * grkmj + xq2 * grkj;
/*< 	      xdb=xdb+xu1*gtkmj+xu2*gtkj >*/
		    xdb = xdb + xu1 * gtkmj + xu2 * gtkj;
/*<               ii1=ii1+xdb*z >*/
		    ii1 += xdb * z__;
/*<               xdb=xi1*grjk+xi2*grjmk+xq1*arrjk+xq2*arrjmk >*/
		    xdb = xi1 * grjk + xi2 * grjmk + xq1 * arrjk + xq2 * 
			    arrjmk;
/*<               xdb=xdb-xu1*artjk+xu2*artjmk >*/
		    xdb = xdb - xu1 * artjk + xu2 * artjmk;
/*< 	      qq2=qq2+xdb*z >*/
		    qq2 += xdb * z__;
/*< 	      xdb=xi1*grjmk+xi2*grjk+xq1*arrjmk+xq2*arrjk >*/
		    xdb = xi1 * grjmk + xi2 * grjk + xq1 * arrjmk + xq2 * 
			    arrjk;
/*< 	      xdb=xdb-xu1*artjmk+xu2*artjk >*/
		    xdb = xdb - xu1 * artjmk + xu2 * artjk;
/*< 	      qq1=qq1+xdb*z >*/
		    qq1 += xdb * z__;
/*<               xdb=xi1*gtjk-xi2*gtjmk+xq1*artkj+xq2*artkmj >*/
		    xdb = xi1 * gtjk - xi2 * gtjmk + xq1 * artkj + xq2 * 
			    artkmj;
/*<               xdb=xdb-xu1*attjk-xu2*attjmk >*/
		    xdb = xdb - xu1 * attjk - xu2 * attjmk;
/*< 	      uu2=uu2-xdb*z >*/
		    uu2 -= xdb * z__;
/*<               xdb=xi1*gtjmk-xi2*gtjk-xq1*artkmj-xq2*artkj >*/
		    xdb = xi1 * gtjmk - xi2 * gtjk - xq1 * artkmj - xq2 * 
			    artkj;
/*<               xdb=xdb-xu1*attjmk-xu2*attjk >*/
		    xdb = xdb - xu1 * attjmk - xu2 * attjk;
/*< 	      uu1=uu1-xdb*z >*/
		    uu1 -= xdb * z__;
/*<    47       continue >*/
/* L47: */
		}
/*<             if (abs(ii2).lt.1.E-30) ii2=0. >*/
		if (abs(ii2) < 1e-30) {
		    ii2 = 0.;
		}
/*<             if (abs(ii1).lt.1.E-30) ii1=0. >*/
		if (abs(ii1) < 1e-30) {
		    ii1 = 0.;
		}
/*<             if (abs(qq2).lt.1.E-30) qq2=0. >*/
		if (abs(qq2) < 1e-30) {
		    qq2 = 0.;
		}
/*<             if (abs(qq1).lt.1.E-30) qq1=0. >*/
		if (abs(qq1) < 1e-30) {
		    qq1 = 0.;
		}
/*<             if (abs(uu2).lt.1.E-30) uu2=0. >*/
		if (abs(uu2) < 1e-30) {
		    uu2 = 0.;
		}
/*<             if (abs(uu1).lt.1.E-30) uu1=0. >*/
		if (abs(uu1) < 1e-30) {
		    uu1 = 0.;
		}
/*<             i2(i,k)=ii2 >*/
		i2[i__ + k * 31 + 775] = ii2;
/*<             i2(i,-k)=ii1 >*/
		i2[i__ + -k * 31 + 775] = ii1;
/*<             q2(i,k)=qq2 >*/
		q2[i__ + k * 31 + 775] = qq2;
/*<             q2(i,-k)=qq1 >*/
		q2[i__ + -k * 31 + 775] = qq1;
/*<             u2(i,k)=uu2 >*/
		u2[i__ + k * 31 + 775] = uu2;
/*<             u2(i,-k)=uu1 >*/
		u2[i__ + -k * 31 + 775] = uu1;
/*<    45   continue >*/
/* L45: */
	    }
	}

/*     vertical integration, upward radiation */

/*<  213    do 48 k=1,mu >*/
L213:
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<           i1(nt,k)=0. >*/
	    i1[*nt + k * 31 + 775] = 0.;
/*<           q1(nt,k)=0. >*/
	    q1[*nt + k * 31 + 775] = 0.;
/*<           u1(nt,k)=0. >*/
	    u1[*nt + k * 31 + 775] = 0.;
/*<           zi1=i1(nt,k) >*/
	    zi1 = i1[*nt + k * 31 + 775];
/*<           zq1=q1(nt,k) >*/
	    zq1 = q1[*nt + k * 31 + 775];
/*<           zu1=u1(nt,k) >*/
	    zu1 = u1[*nt + k * 31 + 775];
/*<           yy=rm(k) >*/
	    yy = rm[k];
/*<           do 48 i=nt-1,0,-1 >*/
	    for (i__ = *nt - 1; i__ >= 0; --i__) {
/*<             jj=i+1 >*/
		jj = i__ + 1;
/*<             f=h(jj)-h(i) >*/
		f = h__[jj] - h__[i__];
/*<             c=exp(-f/yy) >*/
		c__ = exp(-f / yy);
/*<             d=1.e+00-c >*/
		d__ = 1. - c__;
/*<             xx=h(i)-h(jj)*c >*/
		xx = h__[i__] - h__[jj] * c__;
/*<             a=(i2(jj,k)-i2(i,k))/f >*/
		a = (i2[jj + k * 31 + 775] - i2[i__ + k * 31 + 775]) / f;
/*<             b=i2(i,k)-a*h(i) >*/
		b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zi1).le.1.E-20) zi1=0. >*/
		if (abs(zi1) <= 1e-20) {
		    zi1 = 0.;
		}
/*<             i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<             a=(q2(jj,k)-q2(i,k))/f >*/
		a = (q2[jj + k * 31 + 775] - q2[i__ + k * 31 + 775]) / f;
/*<             b=q2(i,k)-a*h(i) >*/
		b = q2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zq1 = c__ * zq1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zq1).le.1.E-20) zq1=0. >*/
		if (abs(zq1) <= 1e-20) {
		    zq1 = 0.;
		}
/*<             q1(i,k)=zq1 >*/
		q1[i__ + k * 31 + 775] = zq1;
/*<             a=(u2(jj,k)-u2(i,k))/f >*/
		a = (u2[jj + k * 31 + 775] - u2[i__ + k * 31 + 775]) / f;
/*<             b=u2(i,k)-a*h(i) >*/
		b = u2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zu1 = c__ * zu1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zu1).le.1.E-20) zu1=0. >*/
		if (abs(zu1) <= 1e-20) {
		    zu1 = 0.;
		}
/*<             u1(i,k)=zu1 >*/
		u1[i__ + k * 31 + 775] = zu1;
/*<    48   continue >*/
/* L48: */
	    }
	}

/*     vertical integration, downward radiation */

/*<         do 50 k=-mu,-1 >*/
	for (k = -(*mu); k <= -1; ++k) {
/*<           i1(0,k)=0. >*/
	    i1[k * 31 + 775] = 0.;
/*<           q1(0,k)=0. >*/
	    q1[k * 31 + 775] = 0.;
/*<           u1(0,k)=0. >*/
	    u1[k * 31 + 775] = 0.;
/*<           zi1=i1(0,k) >*/
	    zi1 = i1[k * 31 + 775];
/*<           zq1=q1(0,k) >*/
	    zq1 = q1[k * 31 + 775];
/*<           zu1=u1(0,k) >*/
	    zu1 = u1[k * 31 + 775];
/*<           yy=rm(k) >*/
	    yy = rm[k];
/*<           do 50 i=1,nt >*/
	    i__1 = *nt;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/*<             jj=i-1 >*/
		jj = i__ - 1;
/*<             f=h(i)-h(jj) >*/
		f = h__[i__] - h__[jj];
/*<             c=exp(f/yy) >*/
		c__ = exp(f / yy);
/*<             d=1.e+00-c >*/
		d__ = 1. - c__;
/*<             xx=h(i)-h(jj)*c >*/
		xx = h__[i__] - h__[jj] * c__;
/*<             a=(i2(i,k)-i2(jj,k))/f >*/
		a = (i2[i__ + k * 31 + 775] - i2[jj + k * 31 + 775]) / f;
/*<             b=i2(i,k)-a*h(i) >*/
		b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zi1).le.1.E-20) zi1=0. >*/
		if (abs(zi1) <= 1e-20) {
		    zi1 = 0.;
		}
/*<             i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<             a=(q2(i,k)-q2(jj,k))/f >*/
		a = (q2[i__ + k * 31 + 775] - q2[jj + k * 31 + 775]) / f;
/*<             b=q2(i,k)-a*h(i) >*/
		b = q2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zq1 = c__ * zq1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zq1).le.1.E-20) zq1=0. >*/
		if (abs(zq1) <= 1e-20) {
		    zq1 = 0.;
		}
/*<             q1(i,k)=zq1 >*/
		q1[i__ + k * 31 + 775] = zq1;
/*<             a=(u2(i,k)-u2(jj,k))/f >*/
		a = (u2[i__ + k * 31 + 775] - u2[jj + k * 31 + 775]) / f;
/*<             b=u2(i,k)-a*h(i) >*/
		b = u2[i__ + k * 31 + 775] - a * h__[i__];
/*<             zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zu1 = c__ * zu1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<             if (abs(zu1).le.1.E-20) zu1=0. >*/
		if (abs(zu1) <= 1e-20) {
		    zu1 = 0.;
		}
/*<             u1(i,k)=zu1 >*/
		u1[i__ + k * 31 + 775] = zu1;
/*<    50   continue >*/
/* L50: */
	    }
	}

/*     in(0,?) is the nieme scattering order */

/*<         do 30 k=-mu,mu >*/
	i__1 = *mu;
	for (k = -(*mu); k <= i__1; ++k) {
/*<           if(k) 31,30,33 >*/
	    if (k < 0) {
		goto L31;
	    } else if (k == 0) {
		goto L30;
	    } else {
		goto L33;
	    }
/*<    31     index=nt >*/
L31:
	    index = *nt;
/*<           go to 34 >*/
	    goto L34;
/*<    33     index=0 >*/
L33:
	    index = 0;
/*<    34     continue >*/
L34:
/*<           in(0,k)=i1(index,k) >*/
	    in[k * 3 + 75] = i1[index + k * 31 + 775];
/*<           qn(0,k)=q1(index,k) >*/
	    qn[k * 3 + 75] = q1[index + k * 31 + 775];
/*<           un(0,k)=u1(index,k) >*/
	    un[k * 3 + 75] = u1[index + k * 31 + 775];
/*<    30   continue >*/
L30:
	    ;
	}
/*<         roIavion(0)=i1(iplane,mu) >*/
	roiavion[1] = i1[iplane + *mu * 31 + 775];
/*<         roQavion(0)=Q1(iplane,mu) >*/
	roqavion[1] = q1[iplane + *mu * 31 + 775];
/*<         roUavion(0)=U1(iplane,mu) >*/
	rouavion[1] = u1[iplane + *mu * 31 + 775];

/*   convergence test (geometrical serie) */

/*<         if(ig.gt.2) then >*/
	if (ig > 2) {
/*<           z=0. >*/
	    z__ = 0.;
/*<             a1=abs(roIavion(2)) >*/
	    a1 = abs(roiavion[3]);
/*<             d1=abs(roIavion(1)) >*/
	    d1 = abs(roiavion[2]);
/*<             g1=abs(roIavion(0)) >*/
	    g1 = abs(roiavion[1]);
/*< 	    r1=abs(roIavion(-1)) >*/
	    r1 = abs(roiavion[0]);
/*<             if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then >*/
	    if (a1 >= acu && d1 >= acu && r1 >= acu) {
/*<               a1=roIavion(2) >*/
		a1 = roiavion[3];
/*<               d1=roIavion(1) >*/
		d1 = roiavion[2];
/*<               g1=roIavion(0) >*/
		g1 = roiavion[1];
/*< 	      r1=roIavion(-1) >*/
		r1 = roiavion[0];
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1)) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / r1);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<             endif >*/
	    }
/*<             a1=abs(roQavion(2)) >*/
	    a1 = abs(roqavion[3]);
/*<             d1=abs(roQavion(1)) >*/
	    d1 = abs(roqavion[2]);
/*<             g1=abs(roQavion(0)) >*/
	    g1 = abs(roqavion[1]);
/*<             r1=abs(roQavion(-1)) >*/
	    r1 = abs(roqavion[0]);
/*<             if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then >*/
	    if (a1 >= acu && d1 >= acu && r1 >= acu) {
/*<               a1=roQavion(2) >*/
		a1 = roqavion[3];
/*<               d1=roQavion(1) >*/
		d1 = roqavion[2];
/*<               g1=roQavion(0) >*/
		g1 = roqavion[1];
/*<               r1=roQavion(-1) >*/
		r1 = roqavion[0];
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1)) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / r1);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<             endif >*/
	    }
/*<             a1=abs(roUavion(2)) >*/
	    a1 = abs(rouavion[3]);
/*<             d1=abs(roUavion(1)) >*/
	    d1 = abs(rouavion[2]);
/*<             g1=abs(roUavion(0)) >*/
	    g1 = abs(rouavion[1]);
/*<             r1=abs(roUavion(-1)) >*/
	    r1 = abs(rouavion[0]);
/*<             if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then >*/
	    if (a1 >= acu && d1 >= acu && r1 >= acu) {
/*<               a1=roUavion(2) >*/
		a1 = rouavion[3];
/*<               d1=roUavion(1) >*/
		d1 = rouavion[2];
/*<               g1=roUavion(0) >*/
		g1 = rouavion[1];
/*<               r1=roUavion(-1) >*/
		r1 = rouavion[0];
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1)) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / r1);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<             endif >*/
	    }
/*<             do 99 l=-mu,mu >*/
	    i__1 = *mu;
	    for (l = -(*mu); l <= i__1; ++l) {
/*<               if (l.eq.0) goto 99 >*/
		if (l == 0) {
		    goto L99;
		}
/*<               a1=in(2,l) >*/
		a1 = in[l * 3 + 77];
/*<               d1=in(1,l) >*/
		d1 = in[l * 3 + 76];
/*<               g1=in(0,l) >*/
		g1 = in[l * 3 + 75];
/*<               if(abs(a1).le.acu) go to 98 >*/
		if (abs(a1) <= acu) {
		    goto L98;
		}
/*<               if(abs(d1).le.acu) go to 98 >*/
		if (abs(d1) <= acu) {
		    goto L98;
		}
/*<               if(abs(i3(l)).le.acu) go to 98 >*/
		if ((d__1 = i3[l + 25], abs(d__1)) <= acu) {
		    goto L98;
		}
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l))) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / i3[l + 25]);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<  98           a1=qn(2,l) >*/
L98:
		a1 = qn[l * 3 + 77];
/*<               d1=qn(1,l) >*/
		d1 = qn[l * 3 + 76];
/*<               g1=qn(0,l) >*/
		g1 = qn[l * 3 + 75];
/*<               if(abs(a1).le.acu) go to 97 >*/
		if (abs(a1) <= acu) {
		    goto L97;
		}
/*<               if(abs(d1).le.acu) go to 97 >*/
		if (abs(d1) <= acu) {
		    goto L97;
		}
/*<               if(abs(q3(l)).le.acu) go to 97 >*/
		if ((d__1 = q3[l + 25], abs(d__1)) <= acu) {
		    goto L97;
		}
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/q3(l))) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / q3[l + 25]);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<  97           a1=un(2,l) >*/
L97:
		a1 = un[l * 3 + 77];
/*<               d1=un(1,l) >*/
		d1 = un[l * 3 + 76];
/*<               g1=un(0,l) >*/
		g1 = un[l * 3 + 75];
/*<               if(abs(a1).le.acu) go to 99 >*/
		if (abs(a1) <= acu) {
		    goto L99;
		}
/*<               if(abs(d1).le.acu) go to 99 >*/
		if (abs(d1) <= acu) {
		    goto L99;
		}
/*<               if(abs(u3(l)).le.acu) go to 99 >*/
		if ((d__1 = u3[l + 25], abs(d__1)) <= acu) {
		    goto L99;
		}
/*<               y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/u3(l))) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / u3[l + 25]);
/*<               y=abs(y) >*/
		y = abs(y);
/*<               z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<   99        continue >*/
L99:
		;
	    }
/*<           if(z.lt.0.01) then >*/
	    if (z__ < .01) {

/*     successful test (geometrical serie) */
/*<             do 606 l=-mu,mu >*/
		i__1 = *mu;
		for (l = -(*mu); l <= i__1; ++l) {
/*<               y1=1. >*/
		    y1 = 1.;
/*<               d1=in(1,l) >*/
		    d1 = in[l * 3 + 76];
/*<               if(abs(d1).le.acu) go to 605 >*/
		    if (abs(d1) <= acu) {
			goto L605;
		    }
/*<               g1=in(0,l) >*/
		    g1 = in[l * 3 + 75];
/*<               if(abs(g1-d1).le.acu) go to 605 >*/
		    if ((d__1 = g1 - d1, abs(d__1)) <= acu) {
			goto L605;
		    }
/*<               y1=1-g1/d1 >*/
		    y1 = 1 - g1 / d1;
/*<               g1=g1/y1 >*/
		    g1 /= y1;
/*<               i3(l)=i3(l)+g1 >*/
		    i3[l + 25] += g1;
/*<  605          y1=1. >*/
L605:
		    y1 = 1.;
/*<               d1=qn(1,l) >*/
		    d1 = qn[l * 3 + 76];
/*<               if(abs(d1).le.acu) go to 604 >*/
		    if (abs(d1) <= acu) {
			goto L604;
		    }
/*<               g1=qn(0,l) >*/
		    g1 = qn[l * 3 + 75];
/*<               if(abs(g1-d1).le.acu) go to 604 >*/
		    if ((d__1 = g1 - d1, abs(d__1)) <= acu) {
			goto L604;
		    }
/*<               y1=1-g1/d1 >*/
		    y1 = 1 - g1 / d1;
/*<               g1=g1/y1 >*/
		    g1 /= y1;
/*<               q3(l)=q3(l)+g1 >*/
		    q3[l + 25] += g1;
/*<  604          y1=1. >*/
L604:
		    y1 = 1.;
/*<               d1=un(1,l) >*/
		    d1 = un[l * 3 + 76];
/*<               if(abs(d1).le.acu) go to 606	 >*/
		    if (abs(d1) <= acu) {
			goto L606;
		    }
/*<               g1=un(0,l) >*/
		    g1 = un[l * 3 + 75];
/*<               if(abs(g1-d1).le.acu) go to 606 >*/
		    if ((d__1 = g1 - d1, abs(d__1)) <= acu) {
			goto L606;
		    }
/*<               y1=1-g1/d1 >*/
		    y1 = 1 - g1 / d1;
/*<               g1=g1/y1 >*/
		    g1 /= y1;
/*<               u3(l)=u3(l)+g1 >*/
		    u3[l + 25] += g1;
/*<   606       continue >*/
L606:
		    ;
		}
/*<             y1=1. >*/
		y1 = 1.;
/*<             d1=roIavion(1) >*/
		d1 = roiavion[2];
/*<             if(abs(d1).ge.acu) then >*/
		if (abs(d1) >= acu) {
/*<               g1=roIavion(0) >*/
		    g1 = roiavion[1];
/*<               if(abs(g1-d1).ge.acu) then >*/
		    if ((d__1 = g1 - d1, abs(d__1)) >= acu) {
/*<                 y1=1-g1/d1 >*/
			y1 = 1 - g1 / d1;
/*<                 g1=g1/y1 >*/
			g1 /= y1;
/*<               endif >*/
		    }
/*<               roIavion(-1)=roIavion(-1)+g1 >*/
		    roiavion[0] += g1;
/*<             endif >*/
		}
/*<             y1=1. >*/
		y1 = 1.;
/*<             d1=roQavion(1) >*/
		d1 = roqavion[2];
/*<             if(abs(d1).ge.acu) then >*/
		if (abs(d1) >= acu) {
/*<               g1=roQavion(0) >*/
		    g1 = roqavion[1];
/*<               if(abs(g1-d1).ge.acu) then >*/
		    if ((d__1 = g1 - d1, abs(d__1)) >= acu) {
/*<                 y1=1-g1/d1 >*/
			y1 = 1 - g1 / d1;
/*<                 g1=g1/y1 >*/
			g1 /= y1;
/*<               endif >*/
		    }
/*<               roQavion(-1)=roQavion(-1)+g1 >*/
		    roqavion[0] += g1;
/*<             endif >*/
		}
/*<             y1=1. >*/
		y1 = 1.;
/*<             d1=roUavion(1) >*/
		d1 = rouavion[2];
/*<             if(abs(d1).ge.acu) then >*/
		if (abs(d1) >= acu) {
/*<               g1=roUavion(0) >*/
		    g1 = rouavion[1];
/*<               if(abs(g1-d1).ge.acu) then >*/
		    if ((d__1 = g1 - d1, abs(d__1)) >= acu) {
/*<                 y1=1-g1/d1 >*/
			y1 = 1 - g1 / d1;
/*<                 g1=g1/y1 >*/
			g1 /= y1;
/*<               endif >*/
		    }
/*<               roUavion(-1)=roUavion(-1)+g1 >*/
		    rouavion[0] += g1;
/*<             endif >*/
		}
/*<             go to 505 >*/
		goto L505;
/*<           endif >*/
	    }

/*     in(2,?) is the (n-2)ieme scattering order */

/*<           do 26 k=-mu,mu >*/
	    i__1 = *mu;
	    for (k = -(*mu); k <= i__1; ++k) {
/*<             in(2,k)=in(1,k) >*/
		in[k * 3 + 77] = in[k * 3 + 76];
/*<             qn(2,k)=qn(1,k) >*/
		qn[k * 3 + 77] = qn[k * 3 + 76];
/*<             un(2,k)=un(1,k) >*/
		un[k * 3 + 77] = un[k * 3 + 76];
/*<    26     continue >*/
/* L26: */
	    }
/*<           roIavion(2)=roIavion(1) >*/
	    roiavion[3] = roiavion[2];
/*<           roQavion(2)=roQavion(1) >*/
	    roqavion[3] = roqavion[2];
/*<           roUavion(2)=roUavion(1) >*/
	    rouavion[3] = rouavion[2];
/*<         endif >*/
	}

/*     in(1,?) is the (n-1)ieme scattering order */

/*<         do 27 k=-mu,mu >*/
	i__1 = *mu;
	for (k = -(*mu); k <= i__1; ++k) {
/*<           in(1,k)=in(0,k) >*/
	    in[k * 3 + 76] = in[k * 3 + 75];
/*<           qn(1,k)=qn(0,k) >*/
	    qn[k * 3 + 76] = qn[k * 3 + 75];
/*<           un(1,k)=un(0,k) >*/
	    un[k * 3 + 76] = un[k * 3 + 75];
/*<    27   continue >*/
/* L27: */
	}
/*<         roIavion(1)=roIavion(0) >*/
	roiavion[2] = roiavion[1];
/*<         roQavion(1)=roQavion(0) >*/
	roqavion[2] = roqavion[1];
/*<         roUavion(1)=roUavion(0) >*/
	rouavion[2] = rouavion[1];

/*     sum of the n-1 orders */
/*<         do 610 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<           i3(l)=i3(l)+in(0,l) >*/
	    i3[l + 25] += in[l * 3 + 75];
/*<           q3(l)=q3(l)+qn(0,l) >*/
	    q3[l + 25] += qn[l * 3 + 75];
/*<           u3(l)=u3(l)+un(0,l) >*/
	    u3[l + 25] += un[l * 3 + 75];
/*<   610   continue >*/
/* L610: */
	}
/*<         roIavion(-1)=roIavion(-1)+roIavion(0) >*/
	roiavion[0] += roiavion[1];
/*<         roQavion(-1)=roQavion(-1)+roQavion(0) >*/
	roqavion[0] += roqavion[1];
/*<         roUavion(-1)=roUavion(-1)+roUavion(0) >*/
	rouavion[0] += rouavion[1];

/*     stop if order n is less than 1% of the sum */
/*<           z=0. >*/
	z__ = 0.;
/*<           do 612 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<             if (abs(i3(l)).ge.acu) then >*/
	    if ((d__1 = i3[l + 25], abs(d__1)) >= acu) {
/*<               y=abs(in(0,l)/i3(l)) >*/
		y = (d__1 = in[l * 3 + 75] / i3[l + 25], abs(d__1));
/*<               z=dmax1(z,dble(y)) >*/
		z__ = max(z__,y);
/*<             endif >*/
	    }
/*<             if (abs(q3(l)).ge.acu) then >*/
	    if ((d__1 = q3[l + 25], abs(d__1)) >= acu) {
/*<               y=abs(qn(0,l)/q3(l)) >*/
		y = (d__1 = qn[l * 3 + 75] / q3[l + 25], abs(d__1));
/*<               z=dmax1(z,dble(y)) >*/
		z__ = max(z__,y);
/*<             endif >*/
	    }
/*<             if (abs(u3(l)).ge.acu) then >*/
	    if ((d__1 = u3[l + 25], abs(d__1)) >= acu) {
/*<               y=abs(un(0,l)/u3(l)) >*/
		y = (d__1 = un[l * 3 + 75] / u3[l + 25], abs(d__1));
/*<               z=dmax1(z,dble(y)) >*/
		z__ = max(z__,y);
/*<             endif >*/
	    }
/*<   612     continue >*/
/* L612: */
	}
/*       if(z.lt.0.00001) go to 505   ###originally */
/*       if(z.lt.0.001) go to 505     ###6sV4.0 choice */
/*        if(z.lt.0.01) go to 505 */
/*<        if(z.lt.0.00000001) go to 505 ! - like in a modified file  >*/
	if (z__ < 1e-8) {
	    goto L505;
	}

/*      stop if order n is greater than 20 in any case */
/*       WRITE(6,*) "WRITE WARNING PRIMARY SCATTERING" */
/*       goto 505 */
/*<         if(ig-igmax) 503,503,505 >*/
	if (ig - multorder_1.igmax <= 0) {
	    goto L503;
	} else {
	    goto L505;
	}
/*<   505   continue >*/
L505:

/*     sum of the fourier component s */

/*<         delta0s=1 >*/
	delta0s = 1.;
/*<         if(is.ne.0) delta0s=2 >*/
	if (is != 0) {
	    delta0s = 2.;
	}
/*<         do 613 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<           i4(l)=i4(l)+abs(delta0s*i3(l)) >*/
	    i4[l + 25] += (d__1 = delta0s * i3[l + 25], abs(d__1));
/*<           q4(l)=q4(l)+abs(q3(l)) >*/
	    q4[l + 25] += (d__1 = q3[l + 25], abs(d__1));
/*<           u4(l)=u4(l)+abs(u3(l)) >*/
	    u4[l + 25] += (d__1 = u3[l + 25], abs(d__1));
/*<   613   continue >*/
/* L613: */
	}

/*     stop of the fourier decomposition */

/*<         do 614 l=1,np >*/
	i__1 = *np;
	for (l = 1; l <= i__1; ++l) {
/*<           phi=rp(l) >*/
	    phi = rp[l];
/*<           do 614 m=-mum1,mum1 >*/
	    i__3 = mum1;
	    for (m = -mum1; m <= i__3; ++m) {
/*<             if(m.gt.0) then >*/
		if (m > 0) {
/*<               xli(m,l)=xli(m,l)+delta0s*i3(m)*cos(is*(phi+pi)) >*/
		    xli[m + l * xli_dim1] += delta0s * i3[m + 25] * cos(is * (
			    phi + pi));
/*<               xlq(m,l)=xlq(m,l)+delta0s*q3(m)*cos(is*(phi+pi)) >*/
		    xlq[m + l * xlq_dim1] += delta0s * q3[m + 25] * cos(is * (
			    phi + pi));
/*<               xlu(m,l)=xlu(m,l)+delta0s*u3(m)*sin(is*(phi+pi)) >*/
		    xlu[m + l * xlu_dim1] += delta0s * u3[m + 25] * sin(is * (
			    phi + pi));
/*<             else >*/
		} else {
/*<               xli(m,l)=xli(m,l)+delta0s*i3(m)*cos(is*phi) >*/
		    xli[m + l * xli_dim1] += delta0s * i3[m + 25] * cos(is * 
			    phi);
/*<               xlq(m,l)=xlq(m,l)+delta0s*q3(m)*cos(is*phi) >*/
		    xlq[m + l * xlq_dim1] += delta0s * q3[m + 25] * cos(is * 
			    phi);
/*<               xlu(m,l)=xlu(m,l)+delta0s*u3(m)*sin(is*phi) >*/
		    xlu[m + l * xlu_dim1] += delta0s * u3[m + 25] * sin(is * 
			    phi);
/*<             endif >*/
		}
/*<  614    continue >*/
/* L614: */
	    }
	}
/* Look up table generation */
/*<       do m=1,mu >*/
	i__3 = *mu;
	for (m = 1; m <= i__3; ++m) {
/*<       do l=1,nfilut(m) >*/
	    i__1 = nfilut[m];
	    for (l = 1; l <= i__1; ++l) {
/*<       phimul=filut(m,l)*pi/180. >*/
		phimul = filut[m + l * filut_dim1] * pi / 180.;
/*<       rolut(m,l)=rolut(m,l)+delta0s*i3(m)*cos(is*(phimul+pi)) >*/
		rolut[m + l * rolut_dim1] += delta0s * i3[m + 25] * cos(is * (
			phimul + pi));
/*<       rolutq(m,l)=rolutq(m,l)+delta0s*q3(m)*cos(is*(phimul+pi)) >*/
		rolutq[m + l * rolutq_dim1] += delta0s * q3[m + 25] * cos(is *
			 (phimul + pi));
/*<       rolutu(m,l)=rolutu(m,l)+delta0s*u3(m)*sin(is*(phimul+pi)) >*/
		rolutu[m + l * rolutu_dim1] += delta0s * u3[m + 25] * sin(is *
			 (phimul + pi));
/*<       enddo >*/
	    }
/*<       enddo >*/
	}
/* end of look up table generation */
/*<         if(is.eq.0) then >*/
	if (is == 0) {
/*<           do k=1,mum1 >*/
	    i__3 = mum1;
	    for (k = 1; k <= i__3; ++k) {
/*<             xli(0,1)=xli(0,1)+rm(k)*gb(k)*i3(-k) >*/
		xli[xli_dim1] += rm[k] * gb[k] * i3[-k + 25];
/*<             xlq(0,1)=xlq(0,1)+rm(k)*gb(k)*q3(-k) >*/
		xlq[xlq_dim1] += rm[k] * gb[k] * q3[-k + 25];
/*<             xlu(0,1)=xlu(0,1)+rm(k)*gb(k)*u3(-k) >*/
		xlu[xlu_dim1] += rm[k] * gb[k] * u3[-k + 25];
/*<           enddo >*/
	    }
/*<         endif >*/
	}
/*<         xli(mu,1)=xli(mu,1)+delta0s*i3(mu)*cos(is*(phirad+pi)) >*/
	xli[*mu + xli_dim1] += delta0s * i3[*mu + 25] * cos(is * (*phirad + 
		pi));
/*<         xlq(mu,1)=xlq(mu,1)+delta0s*q3(mu)*cos(is*(phirad+pi)) >*/
	xlq[*mu + xlq_dim1] += delta0s * q3[*mu + 25] * cos(is * (*phirad + 
		pi));
/*<         xlu(mu,1)=xlu(mu,1)+delta0s*u3(mu)*sin(is*(phirad+pi)) >*/
	xlu[*mu + xlu_dim1] += delta0s * u3[*mu + 25] * sin(is * (*phirad + 
		pi));
/*<         xli(-mu,1)=xli(-mu,1)+delta0s*roIavion(-1)*cos(is*(phirad+pi)) >*/
	xli[-(*mu) + xli_dim1] += delta0s * roiavion[0] * cos(is * (*phirad + 
		pi));
/*<         xlq(-mu,1)=xlq(-mu,1)+delta0s*roQavion(-1)*cos(is*(phirad+pi)) >*/
	xlq[-(*mu) + xlq_dim1] += delta0s * roqavion[0] * cos(is * (*phirad + 
		pi));
/*<         xlu(-mu,1)=xlu(-mu,1)+delta0s*roUavion(-1)*sin(is*(phirad+pi)) >*/
	xlu[-(*mu) + xlu_dim1] += delta0s * rouavion[0] * sin(is * (*phirad + 
		pi));
/*<         do ifi=1,nfi >*/
	i__3 = *nfi;
	for (ifi = 1; ifi <= i__3; ++ifi) {
/*<         phimul=(ifi-1)*pi/(nfi-1) >*/
	    phimul = (ifi - 1) * pi / (*nfi - 1);
/*<         xlphim(ifi)=xlphim(ifi)+delta0s*roIavion(-1)*cos(is*(phimul+pi)) >*/
	    xlphim[ifi] += delta0s * roiavion[0] * cos(is * (phimul + pi));
/*<         enddo >*/
	}
/*<           z=0. >*/
	z__ = 0.;
/*<           do 616 l=-mu,mu >*/
	i__3 = *mu;
	for (l = -(*mu); l <= i__3; ++l) {
/*<             if (abs(i4(l)).lt.acu) goto 617 >*/
	    if ((d__1 = i4[l + 25], abs(d__1)) < acu) {
		goto L617;
	    }
/*<             x=abs(delta0s*i3(l)/i4(l)) >*/
	    x = (d__1 = delta0s * i3[l + 25] / i4[l + 25], abs(d__1));
/*<             z=dmax1(z,x) >*/
	    z__ = max(z__,x);
/*<  617        if (abs(q4(l)).lt.acu) goto 618 >*/
L617:
	    if ((d__1 = q4[l + 25], abs(d__1)) < acu) {
		goto L618;
	    }
/*<             x=abs(q3(l)/q4(l)) >*/
	    x = (d__1 = q3[l + 25] / q4[l + 25], abs(d__1));
/*<             z=dmax1(z,x) >*/
	    z__ = max(z__,x);
/*<  618        if (abs(u4(l)).lt.acu) goto 616 >*/
L618:
	    if ((d__1 = u4[l + 25], abs(d__1)) < acu) {
		goto L616;
	    }
/*<             x=abs(u3(l)/u4(l)) >*/
	    x = (d__1 = u3[l + 25] / u4[l + 25], abs(d__1));
/*<             z=dmax1(z,x) >*/
	    z__ = max(z__,x);
/*<   616     continue >*/
L616:
	    ;
	}
/*       if(z.gt.0.0001) go to 24     #### originally */
/*       if(z.gt.0.001) go to 24    #### 6s choice */
/*<          if(z.gt.0.0005) go to 24  >*/
	if (z__ > 5e-4) {
	    goto L24;
	}
/*<        goto 243  >*/
	goto L243;
/*<    24 continue >*/
L24:
	;
    }
/*<   243 continue >*/
L243:
/*<       nt=snt >*/
    *nt = snt;
/*<       return >*/
    return 0;
/*<       end >*/
} /* ospol_ */

#ifdef __cplusplus
	}
#endif
