/* OS.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int os_(integer *iaer_prof__, doublereal *tamoy, doublereal *
	trmoy, doublereal *pizmoy, doublereal *tamoyp, doublereal *trmoyp, 
	doublereal *palt, doublereal *phirad, integer *nt, integer *mu, 
	integer *np, doublereal *rm, doublereal *gb, doublereal *rp, 
	doublereal *xl, doublereal *xlphim, integer *nfi, doublereal *rolut)
{
    /* System generated locals */
    integer rm_offset, gb_offset, xl_dim1, xl_offset, rolut_dim1, 
	    rolut_offset, i__1, i__2, i__3, i__4;
    doublereal d__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal), acos(doublereal), cos(doublereal)
	    , sqrt(doublereal);

    /* Local variables */
    doublereal roavion0, roavion1, roavion2, a, b, c__, d__, f, h__[31];
    integer i__, j, k, l, m;
    doublereal x, y, z__, a1, d1, g1, i1[1581]	/* was [31][51] */, i2[1581]	
	    /* was [31][51] */, i3[51], i4[51];
    extern /* Subroutine */ int aero_prof__(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *);
    doublereal y1, ca, dd, ha, ch[31];
    integer ig;
    doublereal bp[1326]	/* was [26][51] */;
    integer jj;
    doublereal ta, cr, in[51], pi, hr, th;
    integer it, is;
    doublereal tr, xx, zx, yy, ii1, ii2, sa1, sa2, xi1, xi2, zi1, xt1, xt2, 
	    cfi;
    integer ifi;
    doublereal xdb, phi, tap;
    integer isp, itp;
    doublereal psl[51102]	/* was [1002][51] */, its, ron, spl, piz, xpl[
	    51], xpk, trp;
    integer snt, ntp;
    doublereal xpj, ypk, inm1[51], inm2[51];
	doublereal acu2 = 0.;
    integer mum1;
    doublereal ppp1, ppp2, aaaa, scaa, accu, altc[31], bpjk, xdel[31], ydel[
	    31], nbmu, taup, beta0, xmus, accu2, beta2, cscaa, bpjmk;
    integer index, iborm;
    doublereal ratio, filut[1025]	/* was [25][41] */, luttv, iscama, 
	    nbisca, iscami;
    integer iplane;
    extern /* Subroutine */ int discre_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), kernel_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    doublereal phimul;
    integer nfilut[25];
    doublereal lutmuv, delta0s, roavion;

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/*<       real nbmu >*/
/* - to vary the number of quadratures */
/*  dimension for gauss integration */
/*<       integer mu,np,nfi >*/
/*<       real rm(-mu:mu),gb(-mu:mu),rp(np),xlphim(nfi) >*/
/*  dimension for os computation */
/*<       real  xl(-mu:mu,np) >*/
/* array for sos computation */
/* THOMAS */
/*      double precision xpl(-mu:mu),bp(0:mu,-mu:mu), */
/*     s xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt),altc(0:nt) */
/*      double precision i1(0:nt,-mu:mu),i2(0:nt,-mu:mu),i3(-mu:mu), */
/*     s   i4(-mu:mu),in(-mu:mu),inm1(-mu:mu),inm2(-mu:mu) */
/*<    >*/
/*<    >*/
/* CCC Begin Variable for Look up table generation */
/* azimuth or scattering angle variable for LUT computation (rolut) */
/*azimuth table for look up table computation (filut), nb of fi in each ca
se (nfilut)*/
/*<       real    luttv,lutmuv,iscama,iscami,nbisca,its,scaa >*/
/*<       real    rolut(mu,41) >*/
/* THOMAS */
/* 			real    filut(mu,41) */
/*< 			real    filut(25,41) >*/
/*      double precision psl(-1:nqmax_p,-mu:mu) */
/*<       double precision psl(-1:1000,-25:25)  >*/
/*      integer nfilut(mu) */
/*<       integer nfilut(25) >*/
/* CCC End Variable for Look up table generation */
/*<       Real tamoy,trmoy,pizmoy >*/
/*<       Real tamoyp,trmoyp,palt,phirad >*/
/*<       real delta,sigma >*/
/*<       double precision hr,ta,tr,trp >*/
/*<       double precision tap,piz,accu,accu2,ha,xmus,zx,yy,dd >*/
/*<       double precision ppp2,ppp1,ca,cr,ratio >*/
/*<       double precision taup,th,xt1,xt2,pi,phi,aaaa,ron >*/
/*<       double precision roavion1,roavion2,roavion,spl,sa1 >*/
/*<       double precision beta0,beta2,roavion0 >*/
/*<       double precision sa2,c,zi1,f,d,xpk,y >*/
/*<       double precision a1,d1,g1,y1,delta0s >*/
/*<       integer snt >*/
/*<       integer nt,iwr,iplane,mum1,ntp,j,it,itp,i,l,m,iborm >*/
/*<       integer is,isp,ig,k,jj,index >*/
/*<       logical ier >*/
/*<       double precision xx,xdb,bpjk,bpjmk,z,xi1,xi2,x,xpj,ypk,a,b,ii1,ii2 >*/
/*<       integer igmax,iaer_prof >*/
/*<       common/sixs_del/delta,sigma >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       common /multorder/ igmax >*/
/*<       nbmu=nquad >*/
    /* Parameter adjustments */
    rolut_dim1 = *mu;
    rolut_offset = rolut_dim1 + 1;
    rolut -= rolut_offset;
    gb_offset = -(*mu);
    gb -= gb_offset;
    rm_offset = -(*mu);
    rm -= rm_offset;
    xl_dim1 = *mu - (-(*mu)) + 1;
    xl_offset = -(*mu) + xl_dim1;
    xl -= xl_offset;
    --rp;
    --xlphim;

    /* Function Body */
    nbmu = (doublereal) num_quad__1.nquad;
/* the optical thickness above plane are recomputed to give o.t above pla 
*/
/*     write(6,*) 'tamoy,trmoy,tamoyp,trmoyp,palt,pizmoy' */
/*      write(6,*) tamoy,trmoy,tamoyp,trmoyp,palt,pizmoy */
/*      write(6,*) 'betal 0:80' */
/*      do i=0,80 */
/*        write(6,*) i,betal(i) */
/*      enddo */
/*      write(6,*) 'phase function 83 terms' */
/*      do i=1,83 */
/*        write(6,*) pha(i) */
/*      enddo */
/*<       snt=nt >*/
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
/*     print *, 'ta,tr,piz,tap,trp,palt,nt' */
/*     print *,ta,tr,piz,tap,trp,palt,nt */
/*<       iplane=0 >*/
    iplane = 0;
/*<       accu=1.e-20 >*/
    accu = 1e-20;
/*<       accu2=1.e-3 >*/
    accu2 = .001;
/*<       mum1=mu-1 >*/
    mum1 = *mu - 1;
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
/*     it's a mixing rayleigh+aerosol */
/*<       if(palt.le.900..and.palt.gt.0.0) then >*/
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

/*<       xmus=-rm(0) >*/
    xmus = -rm[0];

/* compute mixing rayleigh, aerosol */
/* case 1: pure rayleigh */
/* case 2: pure aerosol */
/* case 3: mixing rayleigh-aerosol */

/*<       if((ta.le.accu2).and.(tr.gt.ta)) then >*/
    if (ta <= accu2 && tr > ta) {
/*<       do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<       h(j)=j*tr/ntp >*/
	    h__[j] = j * tr / ntp;
/*<       ch(j)=exp(-h(j)/xmus)/2. >*/
	    ch[j] = exp(-h__[j] / xmus) / 2.;
/*<       ydel(j)=1.0 >*/
	    ydel[j] = 1.;
/*<       xdel(j)=0.0 >*/
	    xdel[j] = 0.;
/*<       if (j.eq.0) then >*/
	    if (j == 0) {
/*<          altc(j)=300. >*/
		altc[j] = 300.;
/*<          else >*/
	    } else {
/*<          altc(j)=-log(h(j)/tr)*hr >*/
		altc[j] = -log(h__[j] / tr) * hr;
/*<          endif >*/
	    }
/*<       enddo >*/
	}
/*<       endif >*/
    }
/*<       if((tr.le.accu2).and.(ta.gt.tr)) then >*/
    if (tr <= accu2 && ta > tr) {
/*<       do j=0,ntp >*/
	i__1 = ntp;
	for (j = 0; j <= i__1; ++j) {
/*<       h(j)=j*ta/ntp >*/
	    h__[j] = j * ta / ntp;
/*<       ch(j)=exp(-h(j)/xmus)/2. >*/
	    ch[j] = exp(-h__[j] / xmus) / 2.;
/*<       ydel(j)=0.0 >*/
	    ydel[j] = 0.;
/*<       xdel(j)=piz >*/
	    xdel[j] = piz;
/*<       if (j.eq.0) then >*/
	    if (j == 0) {
/*<          altc(j)=300. >*/
		altc[j] = 300.;
/*<          else >*/
	    } else {
/*<          altc(j)=-log(h(j)/ta)*ha >*/
		altc[j] = -log(h__[j] / ta) * ha;
/*<          endif >*/
	    }
/*<       enddo >*/
	}
/*<       endif >*/
    }

/*<       if(tr.gt.accu2.and.ta.gt.accu2.and.iaer_prof.eq.0)then >*/
    if (tr > accu2 && ta > accu2 && *iaer_prof__ == 0) {
/*<       ydel(0)=1.0 >*/
	ydel[0] = 1.;
/*<       xdel(0)=0.0 >*/
	xdel[0] = 0.;
/*<       h(0)=0. >*/
	h__[0] = 0.;
/*<       ch(0)=0.5 >*/
	ch[0] = .5;
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
/*<       xx=-zx/ha >*/
	    xx = -zx / ha;
/*<       if (xx.le.-20) then >*/
	    if (xx <= -20.) {
/*<          ca=0. >*/
		ca = 0.;
/*<          else >*/
	    } else {
/*<          ca=ta*dexp(xx) >*/
		ca = ta * exp(xx);
/*<          endif >*/
	    }
/*<       xx=-zx/hr >*/
	    xx = -zx / hr;
/*<       cr=tr*dexp(xx) >*/
	    cr = tr * exp(xx);
/*<       h(it)=cr+ca >*/
	    h__[it] = cr + ca;
/*<       altc(it)=zx >*/
	    altc[it] = zx;
/*<       ch(it)=exp(-h(it)/xmus)/2. >*/
	    ch[it] = exp(-h__[it] / xmus) / 2.;
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
/*     print *,'discre ',it,cr,ca,xdel(it),ydel(it),zx */
/*<   14  continue >*/
/* L14: */
	}
/*<       endif >*/
    }
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
/*<          th=0.0005 >*/
	th = 5e-4;
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
/*<             ch(i)=ch(i-1) >*/
		ch[i__] = ch[i__ - 1];
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
/*<          if ( tr.gt.accu2.and.ta.gt.accu2) then >*/
	if (tr > accu2 && ta > accu2) {
/*<          ca=ta*exp(-palt/ha) >*/
	    ca = ta * exp(-(*palt) / ha);
/*<          cr=tr*exp(-palt/hr) >*/
	    cr = tr * exp(-(*palt) / hr);
/*<          h(iplane)=ca+cr >*/
	    h__[iplane] = ca + cr;
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
/*<          ch(iplane)=exp(-h(iplane)/xmus)/2. >*/
	    ch[iplane] = exp(-h__[iplane] / xmus) / 2.;
/*<          endif >*/
	}
/*<          if ( tr.gt.accu2.and.ta.le.accu2) then >*/
	if (tr > accu2 && ta <= accu2) {
/*<          ydel(iplane)=1. >*/
	    ydel[iplane] = 1.;
/*<          xdel(iplane)=0. >*/
	    xdel[iplane] = 0.;
/*<          altc(iplane)=palt >*/
	    altc[iplane] = *palt;
/*<          endif >*/
	}
/*<          if ( tr.le.accu2.and.ta.gt.accu2) then >*/
	if (tr <= accu2 && ta > accu2) {
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


/*     print *,ha,hr,palt,ta,tr,tap,trp */
/*     do i=0,nt */
/*     print *,i,h(i),ch(i),xdel(i),ydel(i),altc(i) */
/*     enddo */

/*<       pi=acos(-1.) >*/
    pi = acos(-1.);
/*<       phi=phirad >*/
    phi = *phirad;
/*<       do 615 l=1,np >*/
    i__1 = *np;
    for (l = 1; l <= i__1; ++l) {
/*<       do 615 m=-mu,mu >*/
	i__2 = *mu;
	for (m = -(*mu); m <= i__2; ++m) {
/*<  615  xl(m,l)=0. >*/
/* L615: */
	    xl[m + l * xl_dim1] = 0.;
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
/*<       nfilut(i)=0 >*/
	nfilut[i__ - 1] = 0;
/*<       do j=1,(nbmu-1)/2 >*/
	i__1 = (integer) ((nbmu - 1) / 2);
	for (j = 1; j <= i__1; ++j) {
/*<       rolut(i,j)=0. >*/
	    rolut[i__ + j * rolut_dim1] = 0.;
/*<       filut(i,j)=0. >*/
	    filut[i__ + j * 25 - 26] = 0.;
/*<       enddo >*/
	}
/*<       enddo >*/
    }
/*<       its=acos(xmus)*180.0/pi >*/
    its = acos(xmus) * 180. / pi;
/*<       do i=1,mu >*/
    i__2 = *mu;
    for (i__ = 1; i__ <= i__2; ++i__) {
/*<          lutmuv=rm(i) >*/
	lutmuv = rm[i__];
/*<          luttv=acos(lutmuv)*180./pi >*/
	luttv = acos(lutmuv) * 180. / pi;
/*<          iscama=(180-abs(luttv-its)) >*/
	iscama = 180 - (d__1 = luttv - its, abs(d__1));
/*<          iscami=(180-(luttv+its)) >*/
	iscami = 180 - (luttv + its);
/*<          nbisca=int((iscama-iscami)/4)+1 >*/
	nbisca = (doublereal) ((integer) ((iscama - iscami) / 4) + 1);
/*<          nfilut(i)=nbisca >*/
	nfilut[i__ - 1] = (integer) nbisca;
/*<          filut(i,1)=0.0 >*/
	filut[i__ - 1] = 0.;
/*<          filut(i,nbisca)=180.0 >*/
	filut[i__ + (integer) nbisca * 25 - 26] = 180.;
/*< 	 scaa=iscama >*/
	scaa = iscama;
/*<          do j=2,nfilut(i)-1 >*/
	i__1 = nfilut[i__ - 1] - 1;
	for (j = 2; j <= i__1; ++j) {
/*<           scaa=scaa-4.0 >*/
	    scaa += -4.;
/*<           cscaa=cos(scaa*pi/180.) >*/
	    cscaa = cos(scaa * pi / 180.);
/*<    >*/
	    cfi = -(cscaa + xmus * lutmuv) / (sqrt(1 - xmus * xmus) * sqrt(1. 
		    - lutmuv * lutmuv));
/*<           filut(i,j)=acos(cfi)*180.0/pi >*/
	    filut[i__ + j * 25 - 26] = acos(cfi) * 180. / pi;
/*<          enddo >*/
	}
/*<       enddo >*/
    }
/* CCC Check initialization  (debug) */
/*      do i=1,mu */
/*        lutmuv=rm(i) */
/*        luttv=acos(lutmuv)*180./pi */
/*       do j=1,nfilut(i) */
/*      cscaa=-xmus*lutmuv-cos(filut(i,j)*pi/180.)*sqrt(1.-xmus*xmus) */
/*    S  *sqrt(1.-lutmuv*lutmuv) */
/*      scaa=acos(cscaa)*180./pi */
/*      write(6,*) its,luttv,filut(i,j),scaa */
/*      enddo */
/*      enddo */
/* CCC Check initialization  (debug) */
/* CC end initialization of look up table variable */

/*     ************ incident angle mus ******* */


/*<       aaaa=delta/(2-delta) >*/
    aaaa = sixs_del__1.delta / (2 - sixs_del__1.delta);
/*<       ron=(1-aaaa)/(1+2*aaaa) >*/
    ron = (1 - aaaa) / (aaaa * 2 + 1);
/*     write(6,*) 'ron ',ron */

/*     rayleigh phase function */

/*<       beta0=1. >*/
    beta0 = 1.;
/*<       beta2=0.5*ron >*/
    beta2 = ron * .5;

/*     fourier decomposition */

/*<       do 17 j=-mu,mu >*/
    i__2 = *mu;
    for (j = -(*mu); j <= i__2; ++j) {
/*<       i4(j)=0. >*/
	i4[j + 25] = 0.;
/*<    17 continue >*/
/* L17: */
    }
/*<       iborm=nbmu-3 >*/
    iborm = (integer) (nbmu - 3);
/*<       if( abs (xmus-1.000000) .lt.1.e-06)iborm=0 >*/
    if ((d__1 = xmus - 1., abs(d__1)) < 1e-6) {
	iborm = 0;
    }
/*<       do 24 is=0,iborm >*/
    i__2 = iborm;
    for (is = 0; is <= i__2; ++is) {

/*    primary scattering */

/*<       ig=1 >*/
	ig = 1;
/*<       roavion0=0. >*/
	roavion0 = 0.;
/*<       roavion1=0. >*/
	roavion1 = 0.;
/*<       roavion2=0. >*/
	roavion2 = 0.;
/*<       roavion=0. >*/
	roavion = 0.;
/*<       do 16 j=-mu,mu >*/
	i__1 = *mu;
	for (j = -(*mu); j <= i__1; ++j) {
/*<       i3(j)=0. >*/
	    i3[j + 25] = 0.;
/*<    16 continue >*/
/* L16: */
	}

/*     kernel computations */

/*<       isp=is >*/
	isp = is;
/*<       call kernel(isp,mu,rm,xpl,psl,bp) >*/
	kernel_(&isp, mu, &rm[rm_offset], xpl, psl, bp);
/*<       if(is.gt.0)beta0=0. >*/
	if (is > 0) {
	    beta0 = 0.;
	}
/*<       do 100 j=-mu,mu >*/
	i__1 = *mu;
	for (j = -(*mu); j <= i__1; ++j) {
/*<       if(is-2)200,200,201 >*/
	    if (is - 2 <= 0) {
		goto L200;
	    } else {
		goto L201;
	    }
/*<  200  spl=xpl(0) >*/
L200:
	    spl = xpl[25];
/*<       sa1=beta0+beta2*xpl(j)*spl >*/
	    sa1 = beta0 + beta2 * xpl[j + 25] * spl;
/*<       sa2=bp(0,j) >*/
	    sa2 = bp[j * 26 + 650];
/*<       goto 202 >*/
	    goto L202;
/*<  201  sa2=bp(0,j) >*/
L201:
	    sa2 = bp[j * 26 + 650];
/*<       sa1=0. >*/
	    sa1 = 0.;

/*     primary scattering source function at every level within th
e layer */

/*<  202  do 101 k=0,nt >*/
L202:
	    i__3 = *nt;
	    for (k = 0; k <= i__3; ++k) {
/*<       c=ch(k) >*/
		c__ = ch[k];
/*<       a=ydel(k) >*/
		a = ydel[k];
/*<       b=xdel(k) >*/
		b = xdel[k];
/*<       i2(k,j)=c*(sa2*b+sa1*a) >*/
		i2[k + j * 31 + 775] = c__ * (sa2 * b + sa1 * a);
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
/*<       i1(nt,k)=0. >*/
	    i1[*nt + k * 31 + 775] = 0.;
/*<       zi1=i1(nt,k) >*/
	    zi1 = i1[*nt + k * 31 + 775];
/*<       yy=rm(k) >*/
	    yy = rm[k];
/*<       do 108 i=nt-1,0,-1 >*/
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
/*<       d=1.0e+00-c >*/
		d__ = 1. - c__;
/*<       xx=h(i)-h(jj)*c >*/
		xx = h__[i__] - h__[jj] * c__;
/*<       zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00 >*/
		zi1 = c__ * zi1 + (d__ * (b + a * yy) + a * xx) * .5;
/*<       i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<   108 continue >*/
/* L108: */
	    }
	}

/*     vertical integration, primary downward radiation */

/*<       do 109 k=-mu,-1 >*/
	for (k = -(*mu); k <= -1; ++k) {
/*<       i1(0,k)=0. >*/
	    i1[k * 31 + 775] = 0.;
/*<       zi1=i1(0,k) >*/
	    zi1 = i1[k * 31 + 775];
/*<       yy=rm(k) >*/
	    yy = rm[k];
/*<       do 109 i=1,nt >*/
	    i__1 = *nt;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/*<       jj=i-1 >*/
		jj = i__ - 1;
/*<       f=h(i)-h(jj) >*/
		f = h__[i__] - h__[jj];
/*<       c=exp(f/yy) >*/
		c__ = exp(f / yy);
/*<       d=1.0e+00-c >*/
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
/*<       roavion2=i1(iplane,mu) >*/
	roavion2 = i1[iplane + *mu * 31 + 775];
/*<       roavion=i1(iplane,mu) >*/
	roavion = i1[iplane + *mu * 31 + 775];

/*     loop on successive order */

/*<   503 ig=ig+1 >*/
L503:
	++ig;
/*     write(6,*) 'ig ',ig */

/*     successive orders */

/*     multiple scattering source function at every level within the l
aye */

/*     if is < ou = 2 kernels are a mixing of aerosols and molecules k
ern */
/*     if is >2 aerosols kernels only */

/*<       if(is-2)210,210,211 >*/
	if (is - 2 <= 0) {
	    goto L210;
	} else {
	    goto L211;
	}
/*<   210 do455 k=1,mu >*/
L210:
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<       xpk=xpl(k) >*/
	    xpk = xpl[k + 25];
/*<       ypk=xpl(-k) >*/
	    ypk = xpl[-k + 25];
/*<       do 455 i=0,nt >*/
	    i__3 = *nt;
	    for (i__ = 0; i__ <= i__3; ++i__) {
/*<       ii1=0. >*/
		ii1 = 0.;
/*<       ii2=0. >*/
		ii2 = 0.;
/*<       x=xdel(i) >*/
		x = xdel[i__];
/*<       y=ydel(i) >*/
		y = ydel[i__];
/*<       do477 j=1,mu >*/
		i__4 = *mu;
		for (j = 1; j <= i__4; ++j) {
/*<       xpj=xpl(j) >*/
		    xpj = xpl[j + 25];
/*<       z=gb(j) >*/
		    z__ = gb[j];
/*<       xi1=i1(i,j) >*/
		    xi1 = i1[i__ + j * 31 + 775];
/*<       xi2=i1(i,-j) >*/
		    xi2 = i1[i__ + -j * 31 + 775];
/*<       bpjk=bp(j,k)*x+y*(beta0+beta2*xpj*xpk) >*/
		    bpjk = bp[j + k * 26 + 650] * x + y * (beta0 + beta2 * 
			    xpj * xpk);
/*<       bpjmk=bp(j,-k)*x+y*(beta0+beta2*xpj*ypk) >*/
		    bpjmk = bp[j + -k * 26 + 650] * x + y * (beta0 + beta2 * 
			    xpj * ypk);
/*<       xdb=z*(xi1*bpjk+xi2*bpjmk) >*/
		    xdb = z__ * (xi1 * bpjk + xi2 * bpjmk);
/*<       ii2=ii2+xdb >*/
		    ii2 += xdb;
/*<       xdb=z*(xi1*bpjmk+xi2*bpjk) >*/
		    xdb = z__ * (xi1 * bpjmk + xi2 * bpjk);
/*<       ii1=ii1+xdb >*/
		    ii1 += xdb;
/*<  477  continue >*/
/* L477: */
		}
/*<       if (abs(ii2).lt.1.E-30) ii2=0. >*/
		if (abs(ii2) < 1e-30) {
		    ii2 = 0.;
		}
/*<       if (abs(ii1).lt.1.E-30) ii1=0. >*/
		if (abs(ii1) < 1e-30) {
		    ii1 = 0.;
		}
/*<       i2(i,k)=ii2 >*/
		i2[i__ + k * 31 + 775] = ii2;
/*<       i2(i,-k)=ii1 >*/
		i2[i__ + -k * 31 + 775] = ii1;
/*<  455  continue >*/
/* L455: */
	    }
	}
/*<       goto 213 >*/
	goto L213;
/*<  211  do45 k=1,mu >*/
L211:
	i__3 = *mu;
	for (k = 1; k <= i__3; ++k) {
/*<       do 45 i=0,nt >*/
	    i__1 = *nt;
	    for (i__ = 0; i__ <= i__1; ++i__) {
/*<       ii1=0. >*/
		ii1 = 0.;
/*<       ii2=0. >*/
		ii2 = 0.;
/*<       x=xdel(i) >*/
		x = xdel[i__];
/*<       do47 j=1,mu >*/
		i__4 = *mu;
		for (j = 1; j <= i__4; ++j) {
/*<       z=gb(j) >*/
		    z__ = gb[j];
/*<       xi1=i1(i,j) >*/
		    xi1 = i1[i__ + j * 31 + 775];
/*<       xi2=i1(i,-j) >*/
		    xi2 = i1[i__ + -j * 31 + 775];
/*<       bpjk=bp(j,k)*x >*/
		    bpjk = bp[j + k * 26 + 650] * x;
/*<       bpjmk=bp(j,-k)*x >*/
		    bpjmk = bp[j + -k * 26 + 650] * x;
/*<       xdb=z*(xi1*bpjk+xi2*bpjmk) >*/
		    xdb = z__ * (xi1 * bpjk + xi2 * bpjmk);
/*<       ii2=ii2+xdb >*/
		    ii2 += xdb;
/*<       xdb=z*(xi1*bpjmk+xi2*bpjk) >*/
		    xdb = z__ * (xi1 * bpjmk + xi2 * bpjk);
/*<       ii1=ii1+xdb >*/
		    ii1 += xdb;
/*<    47 continue >*/
/* L47: */
		}
/*<       if (abs(ii2).lt.1.E-30) ii2=0. >*/
		if (abs(ii2) < 1e-30) {
		    ii2 = 0.;
		}
/*<       if (abs(ii1).lt.1.E-30) ii1=0. >*/
		if (abs(ii1) < 1e-30) {
		    ii1 = 0.;
		}
/*<       i2(i,k)=ii2 >*/
		i2[i__ + k * 31 + 775] = ii2;
/*<       i2(i,-k)=ii1 >*/
		i2[i__ + -k * 31 + 775] = ii1;
/*<    45 continue >*/
/* L45: */
	    }
	}

/*     vertical integration, upward radiation */

/*<  213  do 48 k=1,mu >*/
L213:
	i__1 = *mu;
	for (k = 1; k <= i__1; ++k) {
/*<       i1(nt,k)=0. >*/
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
/*<       if (abs(zi1).le.1.E-20) zi1=0. >*/
		if (abs(zi1) <= 1e-20) {
		    zi1 = 0.;
		}
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
	    i__1 = *nt;
	    for (i__ = 1; i__ <= i__1; ++i__) {
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
/*<       if (abs(zi1).le.1.E-20) zi1=0. >*/
		if (abs(zi1) <= 1e-20) {
		    zi1 = 0.;
		}
/*<       i1(i,k)=zi1 >*/
		i1[i__ + k * 31 + 775] = zi1;
/*<    50 continue >*/
/* L50: */
	    }
	}

/*     in is the nieme scattering order */

/*<       do 30 k=-mu,mu >*/
	i__1 = *mu;
	for (k = -(*mu); k <= i__1; ++k) {
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
/*<       roavion0=i1(iplane,mu) >*/
	roavion0 = i1[iplane + *mu * 31 + 775];

/*   convergence test (geometrical serie) */

/*<       if(ig.gt.2) then >*/
	if (ig > 2) {
/*<       z=0. >*/
	    z__ = 0.;
/*<       a1=roavion2 >*/
	    a1 = roavion2;
/*<       d1=roavion1 >*/
	    d1 = roavion1;
/*<       g1=roavion0 >*/
	    g1 = roavion0;
/*<       if(a1.ge.accu.and.d1.ge.accu.and.roavion.ge.accu) then >*/
	    if (a1 >= accu && d1 >= accu && roavion >= accu) {
/*<       y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/roavion)) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / roavion);
/*<       y=abs(y) >*/
		y = abs(y);
/*<       z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<       endif >*/
	    }
/*<       do 99 l=-mu,mu >*/
	    i__1 = *mu;
	    for (l = -(*mu); l <= i__1; ++l) {
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
/*<       if(a1.le.accu) go to 99 >*/
		if (a1 <= accu) {
		    goto L99;
		}
/*<       if(d1.le.accu) go to 99 >*/
		if (d1 <= accu) {
		    goto L99;
		}
/*<       if(i3(l).le.accu) go to 99 >*/
		if (i3[l + 25] <= accu) {
		    goto L99;
		}
/*<       y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l))) >*/
/* Computing 2nd power */
		d__1 = 1 - g1 / d1;
		y = (g1 / d1 - d1 / a1) / (d__1 * d__1) * (g1 / i3[l + 25]);
/*<       y=abs(y) >*/
		y = abs(y);
/*<       z=dmax1(dble(y),z) >*/
		z__ = max(y,z__);
/*<   99  continue >*/
L99:
		;
	    }
/*<       if(z.lt.0.0001) then >*/
	    if (z__ < 1e-4) {

/*     successful test (geometrical serie) */

/*<       do 606 l=-mu,mu >*/
		i__1 = *mu;
		for (l = -(*mu); l <= i__1; ++l) {
/*<       y1=1. >*/
		    y1 = 1.;
/*<       d1=inm1(l) >*/
		    d1 = inm1[l + 25];
/*<       g1=in(l) >*/
		    g1 = in[l + 25];
/*<       if(d1.le.accu) go to 606 >*/
		    if (d1 <= accu) {
			goto L606;
		    }
/*<       y1=1-g1/d1 >*/
		    y1 = 1 - g1 / d1;
/*<       if(abs(g1-d1).le.accu) then >*/
		    if ((d__1 = g1 - d1, abs(d__1)) <= accu) {
/*<       go to 606 >*/
			goto L606;
/*<       endif >*/
		    }
/*<       g1=g1/y1 >*/
		    g1 /= y1;
/*<       i3(l)=i3(l)+g1 >*/
		    i3[l + 25] += g1;
/*<   606 continue >*/
L606:
		    ;
		}
/*<       d1=roavion1 >*/
		d1 = roavion1;
/*<       g1=roavion0 >*/
		g1 = roavion0;
/*<       y1=1. >*/
		y1 = 1.;
/*<       if(d1.ge.accu) then >*/
		if (d1 >= accu) {
/*<       if(abs(g1-d1).ge.accu) then >*/
		    if ((d__1 = g1 - d1, abs(d__1)) >= accu) {
/*<       y1=1-g1/d1 >*/
			y1 = 1 - g1 / d1;
/*<       g1=g1/y1 >*/
			g1 /= y1;
/*<       endif >*/
		    }
/*<       roavion=roavion+g1 >*/
		    roavion += g1;
/*<       endif >*/
		}
/*<       go to 505 >*/
		goto L505;
/*<       endif >*/
	    }

/*     inm2 is the (n-2)ieme scattering order */

/*<       do 26 k=-mu,mu >*/
	    i__1 = *mu;
	    for (k = -(*mu); k <= i__1; ++k) {
/*<       inm2(k)=inm1(k) >*/
		inm2[k + 25] = inm1[k + 25];
/*<    26 continue >*/
/* L26: */
	    }
/*<       roavion2=roavion1 >*/
	    roavion2 = roavion1;
/*<       endif >*/
	}

/*     inm1 is the (n-1)ieme scattering order */

/*<       do 27 k=-mu,mu >*/
	i__1 = *mu;
	for (k = -(*mu); k <= i__1; ++k) {
/*<       inm1(k)=in(k) >*/
	    inm1[k + 25] = in[k + 25];
/*<    27 continue >*/
/* L27: */
	}
/*<       roavion1=roavion0 >*/
	roavion1 = roavion0;

/*     sum of the n-1 orders */

/*<       do 610 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<       i3(l)=i3(l)+in(l) >*/
	    i3[l + 25] += in[l + 25];
/*<   610 continue >*/
/* L610: */
	}
/*<       roavion=roavion+roavion0 >*/
	roavion += roavion0;

/*     stop if order n is less than 1% of the sum */

/*<       z=0. >*/
	z__ = 0.;
/*<       do 611 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<       if (abs(i3(l)).ge.accu) then >*/
	    if ((d__1 = i3[l + 25], abs(d__1)) >= accu) {
/*<       y=abs(in(l)/i3(l)) >*/
		y = (d__1 = in[l + 25] / i3[l + 25], abs(d__1));
/*<       z=dmax1(z,dble(y)) >*/
		z__ = max(z__,y);
/*<       endif >*/
	    }
/*<   611 continue >*/
/* L611: */
	}
/*     if(z.lt.0.00001) go to 505    # 6SV4.0 choice */
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

/*     sum of the fourier component s */

/*<       delta0s=1 >*/
	delta0s = 1.;
/*<       if(is.ne.0) delta0s=2 >*/
	if (is != 0) {
	    delta0s = 2.;
	}
/*<       do 612 l=-mu,mu >*/
	i__1 = *mu;
	for (l = -(*mu); l <= i__1; ++l) {
/*<       i4(l)=i4(l)+delta0s*i3(l) >*/
	    i4[l + 25] += delta0s * i3[l + 25];
/*<   612 continue >*/
/* L612: */
	}

/*     stop of the fourier decomposition */

/*<       do 614 l=1,np >*/
	i__1 = *np;
	for (l = 1; l <= i__1; ++l) {
/*<       phi=rp(l) >*/
	    phi = rp[l];
/*<       do 614 m=-mum1,mum1 >*/
	    i__3 = mum1;
	    for (m = -mum1; m <= i__3; ++m) {
/*<       if(m.gt.0) then >*/
		if (m > 0) {
/*<       xl(m,l)=xl(m,l)+delta0s*i3(m)*cos(is*(phi+pi)) >*/
		    xl[m + l * xl_dim1] += delta0s * i3[m + 25] * cos(is * (
			    phi + pi));
/*<       else >*/
		} else {
/*<       xl(m,l)=xl(m,l)+delta0s*i3(m)*cos(is*phi) >*/
		    xl[m + l * xl_dim1] += delta0s * i3[m + 25] * cos(is * 
			    phi);
/*<       endif >*/
		}
/*<  614  continue >*/
/* L614: */
	    }
	}
/* Look up table generation */
/*<       do m=1,mu >*/
	i__3 = *mu;
	for (m = 1; m <= i__3; ++m) {
/*<       do l=1,nfilut(m) >*/
	    i__1 = nfilut[m - 1];
	    for (l = 1; l <= i__1; ++l) {
/*<       phimul=filut(m,l)*pi/180. >*/
		phimul = filut[m + l * 25 - 26] * pi / 180.;
/*<       rolut(m,l)=rolut(m,l)+delta0s*i3(m)*cos(is*(phimul+pi)) >*/
		rolut[m + l * rolut_dim1] += delta0s * i3[m + 25] * cos(is * (
			phimul + pi));
/*<       enddo >*/
	    }
/*<       enddo >*/
	}
/* end of look up table generation */
/*<       if(is.eq.0) then >*/
	if (is == 0) {
/*<       do k=1,mum1 >*/
	    i__3 = mum1;
	    for (k = 1; k <= i__3; ++k) {
/*<       xl(0,1)=xl(0,1)+rm(k)*gb(k)*i3(-k) >*/
		xl[xl_dim1] += rm[k] * gb[k] * i3[-k + 25];
/*<       enddo >*/
	    }
/*<       endif >*/
	}
/*<       xl(mu,1)=xl(mu,1)+delta0s*i3(mu)*cos(is*(phirad+pi)) >*/
	xl[*mu + xl_dim1] += delta0s * i3[*mu + 25] * cos(is * (*phirad + pi))
		;
/*<       do ifi=1,nfi >*/
	i__3 = *nfi;
	for (ifi = 1; ifi <= i__3; ++ifi) {
/*<       phimul=(ifi-1)*pi/(nfi-1) >*/
	    phimul = (ifi - 1) * pi / (*nfi - 1);
/*<       xlphim(ifi)=xlphim(ifi)+delta0s*roavion*cos(is*(phimul+pi)) >*/
	    xlphim[ifi] += delta0s * roavion * cos(is * (phimul + pi));
/*<       enddo >*/
	}
/*<       xl(-mu,1)=xl(-mu,1)+delta0s*roavion*cos(is*(phirad+pi)) >*/
	xl[-(*mu) + xl_dim1] += delta0s * roavion * cos(is * (*phirad + pi));
/*<       z=0. >*/
	z__ = 0.;
/*<       do 613 l=-mu,mu >*/
	i__3 = *mu;
	for (l = -(*mu); l <= i__3; ++l) {
/*<        if (abs(i4(l)).lt.accu) goto 613 >*/
	    if ((d__1 = i4[l + 25], abs(d__1)) < accu) {
		goto L613;
	    }
/*<       x=abs(i3(l)/i4(l)) >*/
	    x = (d__1 = i3[l + 25] / i4[l + 25], abs(d__1));
/*<       z=dmax1(z,x) >*/
	    z__ = max(z__,x);
/*<   613 continue >*/
L613:
	    ;
	}
/*     if(z.gt.0.001) go to 24     #6Sv4.0 choice */
/*<       if(z.gt.0.001) go to 24 >*/
	if (z__ > .001) {
	    goto L24;
	}
/*<       goto 243 >*/
	goto L243;
/*<    24 continue >*/
L24:
	;
    }
/*<   243 continue >*/
L243:
/*<       nt=snt >*/
    *nt = snt;
/*     write(6,*) 'reflectance ', xl(mu,1)/xmus */
/*<       return >*/
    return 0;
/*<       end >*/
} /* os_ */

#ifdef __cplusplus
	}
#endif
