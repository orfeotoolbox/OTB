/* ISO.f -- translated by f2c (version 19970805).
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

/* Table of constant values */

static integer c__0 = 0;

/*<    >*/
/* Subroutine */ int iso_(integer *iaer_prof__, doublereal *tamoy, doublereal 
	*trmoy, doublereal *pizmoy, doublereal *tamoyp, doublereal *trmoyp, 
	doublereal *palt, integer *nt, integer *mu, doublereal *rm, 
	doublereal *gb, doublereal *xf)
{
    /* System generated locals */
    integer rm_offset, gb_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal a, b, c__, d__, f, h__[31];
    integer i__, j, k, l, m;
    doublereal x, y, z__, a1, d1, g1, i1[1581]	/* was [31][51] */, i2[1581]	
	    /* was [31][51] */, i3[51];
    extern /* Subroutine */ int aero_prof__(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *);
    doublereal y1, ca, dd, ha, ch[31];
    integer ig;
    doublereal bp[1326]	/* was [26][51] */;
    integer jj;
    doublereal ta, cr, in[51], hr, th;
    integer it;
    doublereal tr, xx, zx, yy, ii1, ii2, xi1, xi2, zi1, xt1, xt2, acu, tap;
    integer itp;
    doublereal psl[51102]	/* was [1002][51] */, ron, xpj, piz, xpl[51], 
	    xpk, trp, ypk;
    integer snt, ntp;
    doublereal xxx, acu2, inm1[51], inm2[51], ppp1, ppp2, aaaa, altc[31], 
	    bpjk, xdel[31], ydel[31], taup, beta0, xmus, beta2, bpjmk;
    integer index;
    doublereal ratio;
    integer iplane;
    extern /* Subroutine */ int discre_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), kernel_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal tavion, tavion0, tavion1, tavion2;

/*<       include "paramdef.inc"      >*/
/*<       double precision psl(-1:1000,-25:25) >*/
/* THOMAS */
/*      double precision psl(-1:nqmax_p,-mu:mu) */
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*  dimension for gauss integration */
/*<       integer mu >*/
/*<       real rm(-mu:mu),gb(-mu:mu) >*/
/*  dimension for os computation */
/*<       real  xf(-1:1) >*/
/* array for sos computation */
/* THOMAS */
/*      double precision xpl(-mu:mu),bp(0:mu,-mu:mu),ch(0:nt) */
/*      double precision xdel(0:nt),ydel(0:nt),h(0:nt),altc(0:nt) */
/*      double precision i1(0:nt,-mu:mu),i2(0:nt,-mu:mu),i3(-mu:mu) */
/*      double precision in(-mu:mu),inm1(-mu:mu),inm2(-mu:mu) */
/*<       double precision xpl(-25:25),bp(0:25,-25:25),ch(0:30) >*/
/*<       double precision xdel(0:30),ydel(0:30),h(0:30),altc(0:30) >*/
/*<       double precision i1(0:30,-25:25),i2(0:30,-25:25),i3(-25:25) >*/
/*<       double precision in(-25:25),inm1(-25:25),inm2(-25:25) >*/
/*<       double precision acu,acu2,ta,piz >*/
/*<       double precision tr,trp,tap,hr,ha,zx,yy,dd,ppp2,ppp1,ca >*/
/*<       double precision cr,ratio,taup,th,xt1,xt2,aaaa,ron,beta0,beta2 >*/
/*<       double precision tavion0,tavion1,tavion2,tavion,zi1,xpk,ypk,x,y >*/
/*<       double precision z,xi1,xi2,bpjk,bpjmk,f,a,b,c,d,xx,a1,d1,g1 >*/
/*<       double precision y1,xpj,xxx,ii1,ii2 >*/
/*<       Real tamoy,trmoy,pizmoy >*/
/*<       Real tamoyp,trmoyp,palt >*/
/*<       Real delta,sigma >*/
/*<       integer snt,nt,iplane,ntp,j,it,itp,i,ig,k,index,iwr,m >*/
/*<       integer jj,l >*/
/*<       logical ier >*/
/*<       integer igmax,iaer_prof >*/
/*<       common/sixs_del/delta,sigma >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       common /multorder/ igmax >*/
/*<       snt=nt >*/
    /* Parameter adjustments */
    gb_offset = -(*mu);
    gb -= gb_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;
    ++xf;

    /* Function Body */
    snt = *nt;
/*<       iplane=0 >*/
    iplane = 0;
/*<       acu=1.e-20 >*/
    acu = 1e-20;
/*<       acu2=1.e-3 >*/
    acu2 = .001;
/*<       ta=tamoy >*/
    ta = *tamoy;
/*<       piz=pizmoy >*/
    piz = *pizmoy;
/*<       tr=trmoy >*/
    tr = *trmoy;
/*<       do 615 m=-1,1 >*/
    for (m = -1; m <= 1; ++m) {
/*<  615  xf(m)=0. >*/
/* L615: */
	xf[m] = 0.;
    }

/*     molecular ratio within the layer */
/*     computations are performed assuming a scale of 8km for */
/*     molecules and 2km for aerosols */

/* the optical thickness above plane are recomputed to give o.t above pla 
*/
/*<       trp=trmoy-trmoyp >*/
    trp = *trmoy - *trmoyp;
/*<       tap=tamoy-tamoyp >*/
    tap = *tamoy - *tamoyp;
/*     print *, 'tamoy,trmoy,pizmoy,tap,trp,palt,nt' */
/*     print *,tamoy,trmoy,pizmoy,tap,trp,palt,nt */
/*<       acu=1.e-20 >*/
    acu = 1e-20;
/* if plane observations recompute scale height for aerosol knowing: */
/* the aerosol optical depth as measure from the plane 	= tamoyp */
/* the rayleigh scale   height = 			= hr (8km) */
/* the rayleigh optical depth  at plane level 		= trmoyp */
/* the altitude of the plane 				= palt */
/* the rayleigh optical depth for total atmos		= trmoy */
/* the aerosol  optical depth for total atmos		= tamoy */
/* if not plane observations then ha is equal to 2.0km */
/* ntp local variable: if ntp=nt     no plane observation selected */
/*                        ntp=nt-1   plane observation selected */
/*<       hr=8.0 >*/
    hr = 8.;
/*     it's a mixing rayleigh+aerosol */
/*<       if(palt.le.900..and.palt.gt.0.0)then >*/
    if (*palt <= 900. && *palt > 0.) {
/*<       if (tap.gt.1.e-03) then >*/
	if (tap > .001) {
/*<          ha=-palt/log(tap/ta) >*/
	    ha = -(*palt) / log(tap / ta);
/*<          else >*/
	} else {
/*<          ha=2. >*/
	    ha = 2.;
/*<          endif >*/
	}
/*<       ntp=nt-1 >*/
	ntp = *nt - 1;
/*<       else >*/
    } else {
/*<       ha=2.0 >*/
	ha = 2.;
/*<       ntp=nt >*/
	ntp = *nt;
/*<       endif >*/
    }

/*<       ta=tamoy >*/
    ta = *tamoy;
/*<       tr=trmoy >*/
    tr = *trmoy;
/*<       piz=pizmoy >*/
    piz = *pizmoy;

/* compute mixing rayleigh, aerosol */
/* case 1: pure rayleigh */
/* case 2: pure aerosol */
/* case 3: mixing rayleigh-aerosol */

/*<       if((ta.le.acu2).and.(tr.gt.ta)) then >*/
    if (ta <= acu2 && tr > ta) {
/*<       do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<       h(j)=j*tr/ntp >*/
	    h__[j] = j * tr / ntp;
/*<       ydel(j)=1.0 >*/
	    ydel[j] = 1.;
/*<       xdel(j)=0.0 >*/
	    xdel[j] = 0.;
/*<       enddo >*/
	}
/*<       endif >*/
    }
/*<       if((tr.le.acu2).and.(ta.gt.tr)) then >*/
    if (tr <= acu2 && ta > tr) {
/*<       do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<       h(j)=j*ta/ntp >*/
	    h__[j] = j * ta / ntp;
/*<       ydel(j)=0.0 >*/
	    ydel[j] = 0.;
/*<       xdel(j)=piz >*/
	    xdel[j] = piz;
/*<       enddo >*/
	}
/*<       endif >*/
    }

/*<       if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.0)then >*/
    if (tr > acu2 && ta > acu2 && *iaer_prof__ == 0) {
/*<       ydel(0)=1.0 >*/
	ydel[0] = 1.;
/*<       xdel(0)=0.0 >*/
	xdel[0] = 0.;
/*<       h(0)=0. >*/
	h__[0] = 0.;
/*<       altc(0)=300. >*/
	altc[0] = 300.;
/*<       zx=300. >*/
	zx = 300.;
/*<       iplane=0 >*/
	iplane = 0;
/*<       do 14 it=0,ntp >*/
	i__1 = ntp;
	for (it = 0; it <= i__1; ++it) {
/*<       if (it.eq.0) then >*/
	    if (it == 0) {
/*<          yy=0. >*/
		yy = 0.;
/*<          dd=0. >*/
		dd = 0.;
/*<          goto 111 >*/
		goto L111;
/*<       endif >*/
	    }
/*<       yy=h(it-1) >*/
	    yy = h__[it - 1];
/*<       dd=ydel(it-1) >*/
	    dd = ydel[it - 1];
/*<  111  ppp2=300.0 >*/
L111:
	    ppp2 = 300.;
/*<       ppp1=0.0 >*/
	    ppp1 = 0.;
/*<       itp=it >*/
	    itp = it;
/*<    >*/
	    discre_(&ta, &ha, &tr, &hr, &itp, &ntp, &yy, &dd, &ppp2, &ppp1, &
		    zx);
/*<       if(ier)return >*/
	    if (sixs_ier__1.ier) {
		return 0;
	    }
/*<       xxx=-zx/ha >*/
	    xxx = -zx / ha;
/*<       if (xxx.lt.-18) then >*/
	    if (xxx < -18.) {
/*<          ca=0. >*/
		ca = 0.;
/*<          else >*/
	    } else {
/*<          ca=ta*dexp(xxx) >*/
		ca = ta * exp(xxx);
/*<          endif >*/
	    }
/*<       xxx=-zx/hr >*/
	    xxx = -zx / hr;
/*<       cr=tr*dexp(xxx) >*/
	    cr = tr * exp(xxx);
/*<       h(it)=cr+ca >*/
	    h__[it] = cr + ca;
/*<       altc(it)=zx >*/
	    altc[it] = zx;
/*     print *,it,cr,ca,h(it),zx */
/*<       cr=cr/hr >*/
	    cr /= hr;
/*<       ca=ca/ha >*/
	    ca /= ha;
/*<       ratio=cr/(cr+ca) >*/
	    ratio = cr / (cr + ca);
/*<       xdel(it)=(1.e+00-ratio)*piz >*/
	    xdel[it] = (1. - ratio) * piz;
/*<       ydel(it)=ratio >*/
	    ydel[it] = ratio;
/*<   14  continue >*/
/* L14: */
	}
/*<       endif >*/
    }
/*<       if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.1)then      >*/
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
/*<          taup=tap+trp >*/
	taup = tap + trp;
/*<          iplane=-1 >*/
	iplane = -1;
/*<          do i=0,ntp >*/
	i__1 = ntp;
	for (i__ = 0; i__ <= i__1; ++i__) {
/*<          if (taup.ge.h(i)) iplane=i >*/
	    if (taup >= h__[i__]) {
		iplane = i__;
	    }
/*<          enddo >*/
	}
/* update the layer from the end to the position to update if necessar
y */
/*<          th=0.005 >*/
	th = .005;
/*<          xt1=abs(h(iplane)-taup) >*/
	xt1 = (d__1 = h__[iplane] - taup, abs(d__1));
/*<          xt2=abs(h(iplane+1)-taup) >*/
	xt2 = (d__1 = h__[iplane + 1] - taup, abs(d__1));
/*<          if ((xt1.gt.th).and.(xt2.gt.th)) then >*/
	if (xt1 > th && xt2 > th) {
/*<          do i=nt,iplane+1,-1 >*/
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
/*<          enddo >*/
	    }
/*<          else >*/
	} else {
/*<          nt=ntp >*/
	    *nt = ntp;
/*<          if (xt2.lt.xt1) iplane=iplane+1 >*/
	    if (xt2 < xt1) {
		++iplane;
	    }
/*<          endif >*/
	}
/*<          h(iplane)=taup >*/
	h__[iplane] = taup;
/*<          if ( tr.gt.acu2.and.ta.gt.acu2) then >*/
	if (tr > acu2 && ta > acu2) {
/*<          ca=ta*exp(-palt/ha) >*/
	    ca = ta * exp(-(*palt) / ha);
/*<          cr=tr*exp(-palt/hr) >*/
	    cr = tr * exp(-(*palt) / hr);
/*<          cr=cr/hr >*/
	    cr /= hr;
/*<          ca=ca/ha >*/
	    ca /= ha;
/*<          ratio=cr/(cr+ca) >*/
	    ratio = cr / (cr + ca);
/*<          xdel(iplane)=(1.e+00-ratio)*piz >*/
	    xdel[iplane] = (1. - ratio) * piz;
/*<          ydel(iplane)=ratio >*/
	    ydel[iplane] = ratio;
/*<          altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<          endif >*/
	}
/*<          if ( tr.gt.acu2.and.ta.le.acu2) then >*/
	if (tr > acu2 && ta <= acu2) {
/*<          ydel(iplane)=1. >*/
	    ydel[iplane] = 1.;
/*<          xdel(iplane)=0. >*/
	    xdel[iplane] = 0.;
/*<          altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<          endif >*/
	}
/*<          if ( tr.le.acu2.and.ta.gt.acu2) then >*/
	if (tr <= acu2 && ta > acu2) {
/*<          ydel(iplane)=0. >*/
	    ydel[iplane] = 0.;
/*<          xdel(iplane)=1.*piz >*/
	    xdel[iplane] = piz * 1.;
/*<          altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<          endif >*/
	}
/*<       endif >*/
    }

/*     print *,ha,hr,palt,tamoy,trmoy,tap,trp */
/*     do i=0,nt */
/*     print *,i,h(i),xdel(i),ydel(i),altc(i) */
/*     enddo */

/*<       aaaa=delta/(2-delta) >*/
    aaaa = sixs_del__1.delta / (2 - sixs_del__1.delta);
/*<       ron=(1-aaaa)/(1+2*aaaa) >*/
    ron = (1 - aaaa) / (aaaa * 2 + 1);

/*     rayleigh phase function */

/*<       beta0=1. >*/
    beta0 = 1.;
/*<       beta2=0.5*ron >*/
    beta2 = ron * .5;

/*    primary scattering */

/*<       ig=1 >*/
    ig = 1;
/*<       tavion0=0. >*/
    tavion0 = 0.;
/*<       tavion1=0. >*/
    tavion1 = 0.;
/*<       tavion2=0. >*/
    tavion2 = 0.;
/*<       tavion=0. >*/
    tavion = 0.;
/*<       do 16 j=-mu,mu >*/
    i__1 = *mu;
    for (j = -(*mu); j <= i__1; ++j) {
/*<       i3(j)=0. >*/
	i3[j + 25] = 0.;
/*<    16 continue >*/
/* L16: */
    }

/*     kernel computations */

/*<       call kernel(0,mu,rm,xpl,psl,bp) >*/
    kernel_(&c__0, mu, &rm[rm_offset], xpl, psl, bp);
/*<       do 100 j=-mu,mu >*/
    i__1 = *mu;
    for (j = -(*mu); j <= i__1; ++j) {
/*<       do 101 k=0,nt >*/
	i__2 = *nt;
	for (k = 0; k <= i__2; ++k) {
/*<       i2(k,j)=0.0000 >*/
	    i2[k + j * 31 + 775] = 0.;
/*<   101 continue >*/
/* L101: */
	}
/*<   100 continue >*/
/* L100: */
    }

/*     vertical integration, primary upward radiation */

/*<       do 108 k=1,mu >*/
    i__1 = *mu;
    for (k = 1; k <= i__1; ++k) {
/*<       i1(nt,k)=1.0 >*/
	i1[*nt + k * 31 + 775] = 1.;
/*<       zi1=i1(nt,k) >*/
	zi1 = i1[*nt + k * 31 + 775];
/*<       yy=rm(k) >*/
	yy = rm[k];
/*<       do 108 i=nt-1,0,-1 >*/
	for (i__ = *nt - 1; i__ >= 0; --i__) {
/*<       i1(i,k)=exp(-(ta+tr-h(i))/yy) >*/
	    i1[i__ + k * 31 + 775] = exp(-(ta + tr - h__[i__]) / yy);
/*<   108 continue >*/
/* L108: */
	}
    }

/*     vertical integration, primary downward radiation */

/*<       do 109 k=-mu,-1 >*/
    for (k = -(*mu); k <= -1; ++k) {
/*<       do 109 i=0,nt >*/
	i__1 = *nt;
	for (i__ = 0; i__ <= i__1; ++i__) {
/*<       i1(i,k)=0.00 >*/
	    i1[i__ + k * 31 + 775] = 0.;
/*<   109 continue >*/
/* L109: */
	}
    }

/*     inm2 is inialized with scattering computed at n-2 */
/*     i3 is inialized with primary scattering */

/*<       do 20 k=-mu,mu >*/
    i__1 = *mu;
    for (k = -(*mu); k <= i__1; ++k) {
/*<       if(k) 21,20,23 >*/
	if (k < 0) {
	    goto L21;
	} else if (k == 0) {
	    goto L20;
	} else {
	    goto L23;
	}
/*<    21 index=nt >*/
L21:
	index = *nt;
/*<       go to 25 >*/
	goto L25;
/*<    23 index=0 >*/
L23:
	index = 0;
/*<    25 continue >*/
L25:
/*<       inm1(k)=i1(index,k) >*/
	inm1[k + 25] = i1[index + k * 31 + 775];
/*<       inm2(k)=i1(index,k) >*/
	inm2[k + 25] = i1[index + k * 31 + 775];
/*<       i3(k)=i1(index,k) >*/
	i3[k + 25] = i1[index + k * 31 + 775];
/*<    20 continue >*/
L20:
	;
    }
/*<       tavion=i1(iplane,mu) >*/
    tavion = i1[iplane + *mu * 31 + 775];
/*<       tavion2=i1(iplane,mu) >*/
    tavion2 = i1[iplane + *mu * 31 + 775];

/*     loop on successive order */

/*<   503 ig=ig+1 >*/
L503:
    ++ig;
/*     write(6,*) 'ig ',ig */

/*     successive orders */

/*     multiple scattering source function at every level within the laye 
*/


/*<       do455 k=1,mu >*/
    i__1 = *mu;
    for (k = 1; k <= i__1; ++k) {
/*<       xpk=xpl(k) >*/
	xpk = xpl[k + 25];
/*<       ypk=xpl(-k) >*/
	ypk = xpl[-k + 25];
/*<       do 455 i=0,nt >*/
	i__2 = *nt;
	for (i__ = 0; i__ <= i__2; ++i__) {
/*<       ii1=0. >*/
	    ii1 = 0.;
/*<       ii2=0. >*/
	    ii2 = 0.;
/*<       x=xdel(i) >*/
	    x = xdel[i__];
/*<       y=ydel(i) >*/
	    y = ydel[i__];
/*<       do477 j=1,mu >*/
	    i__3 = *mu;
	    for (j = 1; j <= i__3; ++j) {
/*<       xpj=xpl(j) >*/
		xpj = xpl[j + 25];
/*<       z=gb(j) >*/
		z__ = gb[j];
/*<       xi1=i1(i,j) >*/
		xi1 = i1[i__ + j * 31 + 775];
/*<       xi2=i1(i,-j) >*/
		xi2 = i1[i__ + -j * 31 + 775];
/*<       bpjk=bp(j,k)*x+y*(beta0+beta2*xpj*xpk) >*/
		bpjk = bp[j + k * 26 + 650] * x + y * (beta0 + beta2 * xpj * 
			xpk);
/*<       bpjmk=bp(j,-k)*x+y*(beta0+beta2*xpj*ypk) >*/
		bpjmk = bp[j + -k * 26 + 650] * x + y * (beta0 + beta2 * xpj *
			 ypk);
/*<       ii2=ii2+z*(xi1*bpjk+xi2*bpjmk) >*/
		ii2 += z__ * (xi1 * bpjk + xi2 * bpjmk);
/*<       ii1=ii1+z*(xi1*bpjmk+xi2*bpjk) >*/
		ii1 += z__ * (xi1 * bpjmk + xi2 * bpjk);
/*<  477  continue >*/
/* L477: */
	    }
/*<       i2(i,k)=ii2 >*/
	    i2[i__ + k * 31 + 775] = ii2;
/*<       i2(i,-k)=ii1 >*/
	    i2[i__ + -k * 31 + 775] = ii1;
/*<  455  continue >*/
/* L455: */
	}
    }

/*     vertical integration, upward radiation */

/*<       do 48 k=1,mu >*/
    i__2 = *mu;
    for (k = 1; k <= i__2; ++k) {
/*<       i1(nt,k)=0.0 >*/
	i1[*nt + k * 31 + 775] = 0.;
/*<       zi1=i1(nt,k) >*/
	zi1 = i1[*nt + k * 31 + 775];
/*<       yy=rm(k) >*/
	yy = rm[k];
/*<       do 48 i=nt-1,0,-1 >*/
	for (i__ = *nt - 1; i__ >= 0; --i__) {
/*<       jj=i+1 >*/
	    jj = i__ + 1;
/*<       f=h(jj)-h(i) >*/
	    f = h__[jj] - h__[i__];
/*<       a=(i2(jj,k)-i2(i,k))/f >*/
	    a = (i2[jj + k * 31 + 775] - i2[i__ + k * 31 + 775]) / f;
/*<       b=i2(i,k)-a*h(i) >*/
	    b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<       c=exp(-f/yy) >*/
	    c__ = exp(-f / yy);
/*<       d=1.e+00-c >*/
	    d__ = 1. - c__;
/*<       xx=h(i)-h(jj)*c >*/
	    xx = h__[i__] - h__[jj] * c__;
/*<       zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
	    zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<       i1(i,k)=zi1 >*/
	    i1[i__ + k * 31 + 775] = zi1;
/*<    48 continue >*/
/* L48: */
	}
    }

/*     vertical integration, downward radiation */

/*<       do 50 k=-mu,-1 >*/
    for (k = -(*mu); k <= -1; ++k) {
/*<       i1(0,k)=0. >*/
	i1[k * 31 + 775] = 0.;
/*<       zi1=i1(0,k) >*/
	zi1 = i1[k * 31 + 775];
/*<       yy=rm(k) >*/
	yy = rm[k];
/*<       do 50 i=1,nt >*/
	i__2 = *nt;
	for (i__ = 1; i__ <= i__2; ++i__) {
/*<       jj=i-1 >*/
	    jj = i__ - 1;
/*<       f=h(i)-h(jj) >*/
	    f = h__[i__] - h__[jj];
/*<       c=exp(f/yy) >*/
	    c__ = exp(f / yy);
/*<       d=1.e+00-c >*/
	    d__ = 1. - c__;
/*<       a=(i2(i,k)-i2(jj,k))/f >*/
	    a = (i2[i__ + k * 31 + 775] - i2[jj + k * 31 + 775]) / f;
/*<       b=i2(i,k)-a*h(i) >*/
	    b = i2[i__ + k * 31 + 775] - a * h__[i__];
/*<       xx=h(i)-h(jj)*c >*/
	    xx = h__[i__] - h__[jj] * c__;
/*<       zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
	    zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<       i1(i,k)=zi1 >*/
	    i1[i__ + k * 31 + 775] = zi1;
/*<    50 continue >*/
/* L50: */
	}
    }

/*     in is the nieme scattering order */

/*<       do 30 k=-mu,mu >*/
    i__2 = *mu;
    for (k = -(*mu); k <= i__2; ++k) {
/*<       if(k) 31,30,33 >*/
	if (k < 0) {
	    goto L31;
	} else if (k == 0) {
	    goto L30;
	} else {
	    goto L33;
	}
/*<    31 index=nt >*/
L31:
	index = *nt;
/*<       go to 34 >*/
	goto L34;
/*<    33 index=0 >*/
L33:
	index = 0;
/*<    34 continue >*/
L34:
/*<       in(k)=i1(index,k) >*/
	in[k + 25] = i1[index + k * 31 + 775];
/*<    30 continue >*/
L30:
	;
    }
/*<       tavion0=i1(iplane,mu) >*/
    tavion0 = i1[iplane + *mu * 31 + 775];

/*   convergence test (geometrical serie) */

/*<       if(ig.gt.2) then >*/
    if (ig > 2) {
/*<       z=0. >*/
	z__ = 0.;
/*<       a1=tavion2 >*/
	a1 = tavion2;
/*<       d1=tavion1 >*/
	d1 = tavion1;
/*<       g1=tavion0 >*/
	g1 = tavion0;
/*<       if (a1.ge.acu.and.d1.ge.acu.and.tavion.ge.acu) then >*/
	if (a1 >= acu && d1 >= acu && tavion >= acu) {
/*<          y=((g1/d1-d1/a1)/((1.-g1/d1)**2)*(g1/tavion)) >*/
/* Computing 2nd power */
	    d__1 = 1. - g1 / d1;
	    y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / tavion);
/*<          y=abs(y) >*/
	    y = abs(y);
/*<          z=max(y,z) >*/
	    z__ = max(y,z__);
/*<       endif >*/
	}
/*<       do 99 l=-mu,mu >*/
	i__2 = *mu;
	for (l = -(*mu); l <= i__2; ++l) {
/*<       if (l.eq.0) goto 99 >*/
	    if (l == 0) {
		goto L99;
	    }
/*<       a1=inm2(l) >*/
	    a1 = inm2[l + 25];
/*<       d1=inm1(l) >*/
	    d1 = inm1[l + 25];
/*<       g1=in(l) >*/
	    g1 = in[l + 25];
/*<       if(a1.eq.0.) go to 99 >*/
	    if (a1 == 0.) {
		goto L99;
	    }
/*<       if(d1.eq.0.) go to 99 >*/
	    if (d1 == 0.) {
		goto L99;
	    }
/*<       if(i3(l).eq.0.) go to 99 >*/
	    if (i3[l + 25] == 0.) {
		goto L99;
	    }
/*<       y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l))) >*/
/* Computing 2nd power */
	    d__1 = 1 - g1 / d1;
	    y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / i3[l + 25]);
/*<       y=abs(y) >*/
	    y = abs(y);
/*<       z=max(y,z) >*/
	    z__ = max(y,z__);
/*<   99  continue >*/
L99:
	    ;
	}
/*<       if(z.lt.0.0001) then >*/
	if (z__ < 1e-4) {

/*     successful test (geometrical serie) */

/*<       do 606 l=-mu,mu >*/
	    i__2 = *mu;
	    for (l = -(*mu); l <= i__2; ++l) {
/*<       if (l.eq.0) goto 606 >*/
		if (l == 0) {
		    goto L606;
		}
/*<       y1=1. >*/
		y1 = 1.;
/*<       d1=inm1(l) >*/
		d1 = inm1[l + 25];
/*<       g1=in(l) >*/
		g1 = in[l + 25];
/*<       if(d1.eq.0.0) go to 606 >*/
		if (d1 == 0.) {
		    goto L606;
		}
/*<       y1=1-g1/d1 >*/
		y1 = 1 - g1 / d1;
/*<       g1=g1/y1 >*/
		g1 /= y1;
/*<       i3(l)=i3(l)+g1 >*/
		i3[l + 25] += g1;
/*<   606 continue >*/
L606:
		;
	    }
/*<       d1=tavion1 >*/
	    d1 = tavion1;
/*<       g1=tavion0 >*/
	    g1 = tavion0;
/*<       y1=1. >*/
	    y1 = 1.;
/*<       if (d1.ge.acu) then >*/
	    if (d1 >= acu) {
/*<       if (abs(g1-d1).ge.acu) then >*/
		if ((d__1 = g1 - d1, abs(d__1)) >= acu) {
/*<          y1=1.-g1/d1 >*/
		    y1 = 1. - g1 / d1;
/*<          g1=g1/y1 >*/
		    g1 /= y1;
/*<       endif >*/
		}
/*<       tavion=tavion+g1 >*/
		tavion += g1;
/*<       endif >*/
	    }
/*<       go to 505 >*/
	    goto L505;
/*<       endif >*/
	}

/*     inm2 is the (n-2)ieme scattering order */

/*<       do 26 k=-mu,mu >*/
	i__2 = *mu;
	for (k = -(*mu); k <= i__2; ++k) {
/*<       inm2(k)=inm1(k) >*/
	    inm2[k + 25] = inm1[k + 25];
/*<    26 continue >*/
/* L26: */
	}
/*<       tavion2=tavion1 >*/
	tavion2 = tavion1;
/*<       endif >*/
    }

/*     inm1 is the (n-1)ieme scattering order */

/*<       do 27 k=-mu,mu >*/
    i__2 = *mu;
    for (k = -(*mu); k <= i__2; ++k) {
/*<       inm1(k)=in(k) >*/
	inm1[k + 25] = in[k + 25];
/*<    27 continue >*/
/* L27: */
    }
/*<       tavion1=tavion0 >*/
    tavion1 = tavion0;

/*     sum of the n-1 orders */

/*<       do 610 l=-mu,mu >*/
    i__2 = *mu;
    for (l = -(*mu); l <= i__2; ++l) {
/*<       i3(l)=i3(l)+in(l) >*/
	i3[l + 25] += in[l + 25];
/*<   610 continue >*/
/* L610: */
    }
/*<       tavion=tavion+tavion0 >*/
    tavion += tavion0;

/*     stop if order n is less than 1% of the sum */

/*<       z=0. >*/
    z__ = 0.;
/*<       do 611 l=-mu,mu >*/
    i__2 = *mu;
    for (l = -(*mu); l <= i__2; ++l) {
/*<       if(i3(l).ne.0)then >*/
	if (i3[l + 25] != 0.) {
/*<       y=abs(in(l)/i3(l)) >*/
	    y = (d__1 = in[l + 25] / i3[l + 25], abs(d__1));
/*<       z=max(z,y) >*/
	    z__ = max(z__,y);
/*<       endif >*/
	}
/*<   611 continue >*/
/* L611: */
    }
/*<       if(z.lt.0.00001) go to 505 >*/
    if (z__ < 1e-5) {
	goto L505;
    }

/*      stop if order n is greater than 20 in any case */

/*<       if(ig-igmax) 503,503,505 >*/
    if (ig - multorder_1.igmax <= 0) {
	goto L503;
    } else {
	goto L505;
    }
/*<   505 continue >*/
L505:


/*<       xf(1)=xf(1)+i3(mu) >*/
    xf[1] += i3[*mu + 25];
/*<       xf(-1)=tavion >*/
    xf[-1] = tavion;
/*<       do k=1,mu >*/
    i__2 = *mu;
    for (k = 1; k <= i__2; ++k) {
/*<       xf(0)=xf(0)+rm(k)*gb(k)*i3(-k) >*/
	xf[0] += rm[k] * gb[k] * i3[-k + 25];
/*<       enddo >*/
    }
/*<       nt=snt >*/
    *nt = snt;
/*<       return >*/
    return 0;
/*<       end >*/
} /* iso_ */

#ifdef __cplusplus
	}
#endif
