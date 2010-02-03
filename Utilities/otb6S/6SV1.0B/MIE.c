/* MIE.f -- translated by f2c (version 19970805).
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
    doublereal ph[20000]	/* was [20][1000] */, qh[20000]	/* was [20][
	    1000] */, uh[20000]	/* was [20][1000] */;
} sixs_aerbas__;

#define sixs_aerbas__1 sixs_aerbas__

Extern struct {
    doublereal rmax, rmin;
    integer icp;
    doublereal rn[80]	/* was [20][4] */, ri[80]	/* was [20][4] */, x1[
	    4], x2[4], x3[4], cij[4];
    integer irsunph;
    doublereal rsunph[50], nrsunph[50];
} mie_in__;

#define mie_in__1 mie_in__

/* Table of constant values */

static doublereal c_b2 = -1.;
static doublereal c_b3 = 1.;
static doublereal c_b5 = 10.;
static doublereal c_b14 = 2.;
static integer c__9 = 9;
static integer c__1 = 1;
static integer c__3 = 3;

/*<       subroutine mie(iaer,wldis,ex,sc,asy,ipol) >*/
/* Subroutine */ int mie_(integer *iaer, doublereal *wldis, doublereal *ex, 
	doublereal *sc, doublereal *asy, integer *ipol)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double atan(doublereal), pow_dd(doublereal *, doublereal *), d_lg10(
	    doublereal *), exp(doublereal), sqrt(doublereal), log(doublereal),
	     acos(doublereal);

    /* Local variables */
    integer i__, j, k, l;
    doublereal r__, p1[80000]	/* was [20][4][1000] */, q1[80000]	/* 
	    was [20][4][1000] */, r0;
    extern /* Subroutine */ int exscphase_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *)
	    ;
    doublereal u1[80000]	/* was [20][4][1000] */, p11[1000], q11[1000],
	     u11[1000], pi, dr, np[4], nr, vi[4], sca[80]	/* was [20][4]
	     */, arg, ext[80]	/* was [20][4] */, qsca, sigm;
    integer nbmu;
    doublereal xmug, qext, alpha, asy_d__, rmind, asy_n__, rmaxd, ldexp;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    integer nbmu_2__;
    doublereal xndpr2, cosang[1000], pdgs_s__[1000], weight[1000], cgaus_s__[
	    1000], rlogpas;

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real ph,qh,uh >*/
/*<       common /sixs_aerbas/ ph(20,nqmax_p),qh(20,nqmax_p),uh(20,nqmax_p) >*/
/*<       real cgaus_S(nqmax_p), pdgs_S(nqmax_p) >*/
/*<       integer nbmu, nbmu_2 >*/
/*<       real cosang(nqmax_p),weight(nqmax_p) >*/
/* - to vary the number of quadratures */
/*<       double precision nr,ext(20,4),sca(20,4),np(4),p1(20,4,nqmax_p)   >*/
/*<       double precision q1(20,4,nqmax_p),u1(20,4,nqmax_p)    >*/
/*<       double precision p11(nqmax_p),q11(nqmax_p),u11(nqmax_p) >*/
/*<       double precision pi,r,rmind,rmaxd,r0,alpha,dr,xndpr2,Qext,Qsca >*/
/*<       double precision rlogpas >*/
/*<       real ex(4,20),sc(4,20),asy(4,20),wldis(20) >*/
/*<       real rmax,rmin,rn,ri,x1,x2,x3,rsunph,nrsunph >*/
/*<       real asy_n,asy_d,cij >*/
/*<       integer icp,i,j,l,k,iaer,irsunph,ipol >*/
/*<       double precision arg,ldexp >*/
/*<    >*/
/*<       real sigm, vi(4) >*/
/*<       ldexp=-300. >*/
    /* Parameter adjustments */
    asy -= 5;
    sc -= 5;
    ex -= 5;
    --wldis;

    /* Function Body */
    ldexp = -300.;
/*<       pi=4.D+00*datan(1.D+00) >*/
    pi = atan(1.) * 4.;
/*<       rlogpas=0.011 >*/
    rlogpas = .011;
/*<       nbmu=nquad  >*/
    nbmu = num_quad__1.nquad;
/*<       do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         np(i)=0.D+00 >*/
	np[i__ - 1] = 0.;
/*<         do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<           ex(i,l)=0.0 >*/
	    ex[i__ + (l << 2)] = 0.;
/*<           sc(i,l)=0.0 >*/
	    sc[i__ + (l << 2)] = 0.;
/*<           asy(i,l)=0.0 >*/
	    asy[i__ + (l << 2)] = 0.;
/*<           ext(l,i)=0.D+00 >*/
	    ext[l + i__ * 20 - 21] = 0.;
/*<           sca(l,i)=0.D+00 >*/
	    sca[l + i__ * 20 - 21] = 0.;
/*<           do k=1,nbmu >*/
	    i__2 = nbmu;
	    for (k = 1; k <= i__2; ++k) {
/*<             p1(l,i,k)=0.D+00                          >*/
		p1[l + (i__ + (k << 2)) * 20 - 101] = 0.;
/*<             q1(l,i,k)=0.D+00                          >*/
		q1[l + (i__ + (k << 2)) * 20 - 101] = 0.;
/*<             u1(l,i,k)=0.D+00                          >*/
		u1[l + (i__ + (k << 2)) * 20 - 101] = 0.;
/*<           enddo >*/
	    }
/*<         enddo >*/
	}
/*<       enddo >*/
    }
/*<       rmaxd=dble(rmax) >*/
    rmaxd = mie_in__1.rmax;
/*<       rmind=dble(rmin) >*/
    rmind = mie_in__1.rmin;
/*<       do k=1,nbmu >*/
    i__1 = nbmu;
    for (k = 1; k <= i__1; ++k) {
/*<        q11(k)=0.D+00 >*/
	q11[k - 1] = 0.;
/*<        u11(k)=0.D+00 >*/
	u11[k - 1] = 0.;
/*<       enddo >*/
    }
/* - calculation of gauss points */
/*<       nbmu=nquad  >*/
    nbmu = num_quad__1.nquad;
/*<       nbmu_2=(nbmu-3)/2 >*/
    nbmu_2__ = (nbmu - 3) / 2;
/*<       call gauss(-1.,1.,cosang,weight,nbmu-3) >*/
    i__1 = nbmu - 3;
    gauss_(&c_b2, &c_b3, cosang, weight, &i__1);
/*<       cgaus_S(1)=-1.0 >*/
    cgaus_s__[0] = -1.;
/*<       pdgs_S(1)=0.0 >*/
    pdgs_s__[0] = 0.;
/*<       do j=1,nbmu_2 >*/
    i__1 = nbmu_2__;
    for (j = 1; j <= i__1; ++j) {
/*<         cgaus_S(j+1)=cosang(j) >*/
	cgaus_s__[j] = cosang[j - 1];
/*<         pdgs_S(j+1)=weight(j) >*/
	pdgs_s__[j] = weight[j - 1];
/*<       enddo  >*/
    }
/*<       cgaus_S(nbmu_2+2)=0. >*/
    cgaus_s__[nbmu_2__ + 1] = 0.;
/*<       pdgs_S(nbmu_2+2)=0. >*/
    pdgs_s__[nbmu_2__ + 1] = 0.;
/*<       do j=nbmu_2+1,nbmu-3 >*/
    i__1 = nbmu - 3;
    for (j = nbmu_2__ + 1; j <= i__1; ++j) {
/*<         cgaus_S(j+2)=cosang(j) >*/
	cgaus_s__[j + 1] = cosang[j - 1];
/*<         pdgs_S(j+2)=weight(j) >*/
	pdgs_s__[j + 1] = weight[j - 1];
/*<       enddo >*/
    }
/*<       cgaus_S(nbmu)=1.0 >*/
    cgaus_s__[nbmu - 1] = 1.;
/*<       pdgs_S(nbmu)=0. >*/
    pdgs_s__[nbmu - 1] = 0.;
/* - calculation of gauss points */
/* LOOPS ON THE NUMBER OF PARTICLE TYPE (4 max) */
/*<       do 600 i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        r=rmind >*/
	r__ = rmind;
/*<        dr=r*(10**rlogpas-1.D+00) >*/
	dr = r__ * (pow_dd(&c_b5, &rlogpas) - 1.);
/*<  123   continue >*/
L123:
/* LOOPS ON THE RADIUS OF THE PARTICLE */
/*call for the size distribution nr. For our computation, we need dn/d
r for*/
/*all functions except for sun-photometer inputs, for which we need dV
/dlog(r)*/
/*<        goto(300,301,302,303)iaer-7 >*/
	switch (*iaer - 7) {
	    case 1:  goto L300;
	    case 2:  goto L301;
	    case 3:  goto L302;
	    case 4:  goto L303;
	}
/* --- Mixture of particles (Log-Normal distribution functions, up to 
5) */
/*<  300    nr=DLOG10(1.D+00*r/x1(i))*DLOG10(1.D+00*r/x1(i)) >*/
L300:
	d__1 = r__ * 1. / mie_in__1.x1[i__ - 1];
	d__2 = r__ * 1. / mie_in__1.x1[i__ - 1];
	nr = d_lg10(&d__1) * d_lg10(&d__2);
/*<         nr=nr/(DLOG10(1.D+00*x2(i))*DLOG10(1.D+00*x2(i))) >*/
	d__1 = mie_in__1.x2[i__ - 1] * 1.;
	d__2 = mie_in__1.x2[i__ - 1] * 1.;
	nr /= d_lg10(&d__1) * d_lg10(&d__2);
/*<         nr=DEXP(-nr/2.D+00) >*/
	nr = exp(-nr / 2.);
/*<         nr=nr/dsqrt(2.D+00*pi)/DLOG(10.D+00)/r/DLOG10(1.D+00*x2(i)) >*/
	d__1 = mie_in__1.x2[i__ - 1] * 1.;
	nr = nr / sqrt(pi * 2.) / log(10.) / r__ / d_lg10(&d__1);
/*<         goto 399 >*/
	goto L399;
/* --- Modified Gamma distribution function */
/*<  301   r0=1.00D+00    >*/
L301:
	r0 = 1.;
/*<        arg=-x2(i)*((r/r0)**x3(i)) >*/
	d__1 = r__ / r0;
	arg = -mie_in__1.x2[i__ - 1] * pow_dd(&d__1, &mie_in__1.x3[i__ - 1]);
/*<        if (arg.gt.ldexp) then >*/
	if (arg > ldexp) {
/*<           nr=((r/r0)**x1(i))*DEXP(arg) >*/
	    d__1 = r__ / r0;
	    nr = pow_dd(&d__1, &mie_in__1.x1[i__ - 1]) * exp(arg);
/*<           else >*/
	} else {
/*<           nr=0. >*/
	    nr = 0.;
/*<           endif >*/
	}
/*<        goto 399 >*/
	goto L399;
/* --- Junge power-law function */
/*<  302   r0=0.1000D+00 >*/
L302:
	r0 = .1;
/*<        nr= r0**(-x1(i)) >*/
	d__1 = -mie_in__1.x1[i__ - 1];
	nr = pow_dd(&r0, &d__1);
/*<        IF(r.GT.r0 ) nr= r**(-x1(i)) >*/
	if (r__ > r0) {
	    d__1 = -mie_in__1.x1[i__ - 1];
	    nr = pow_dd(&r__, &d__1);
	}
/*<        goto 399 >*/
	goto L399;

/* --- from sun photometer */
/*<  303    nr=0.D+00 >*/
L303:
	nr = 0.;
/*< 	do 299 j=2,irsunph >*/
	i__2 = mie_in__1.irsunph;
	for (j = 2; j <= i__2; ++j) {
/*< 	if ((r-rsunph(j)).lt.0.000001)then >*/
	    if (r__ - mie_in__1.rsunph[j - 1] < 1e-6) {
/*<          nr=(r-rsunph(j-1))/(rsunph(j)-rsunph(j-1)) >*/
		nr = (r__ - mie_in__1.rsunph[j - 2]) / (mie_in__1.rsunph[j - 
			1] - mie_in__1.rsunph[j - 2]);
/*<          nr=nrsunph(j-1)+nr*(nrsunph(j)-nrsunph(j-1)) >*/
		nr = mie_in__1.nrsunph[j - 2] + nr * (mie_in__1.nrsunph[j - 1]
			 - mie_in__1.nrsunph[j - 2]);
/*< 	 goto 398 >*/
		goto L398;
/*< 	endif >*/
	    }
/*<  299   continue >*/
/* L299: */
	}
/*The Mie's calculations have to be called several times (min=2, max=1
0 for*/
/*each type of particle): at wavelengths bounding the range of the sel
ected*/
/*wavelengths,and at 0.550 microns to normalized the extinction coeffi
cient*/
/* (if it's not in the selected range of wavelengths). */
/*<  398   nr=nr*3./(pi*r*r*r*r*4.) >*/
L398:
	nr = nr * 3. / (pi * r__ * r__ * r__ * r__ * 4.);
/*<  399   continue >*/
L399:
/*<        xndpr2=nr*dr*pi*(r**2.D+00) >*/
	xndpr2 = nr * dr * pi * pow_dd(&r__, &c_b14);
/*relatif number of particle for each type of particle (has to be equa
l to 1)*/
/*<        np(i)=np(i)+nr*dr >*/
	np[i__ - 1] += nr * dr;
/*<         do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*< 	 alpha=2.D+00*pi*r/wldis(l) >*/
	    alpha = pi * 2. * r__ / wldis[l];
/*<    >*/
	    exscphase_(&alpha, &mie_in__1.rn[l + i__ * 20 - 21], &
		    mie_in__1.ri[l + i__ * 20 - 21], &qext, &qsca, p11, q11, 
		    u11, ipol, cgaus_s__, pdgs_s__);
/*<          ext(l,i)=ext(l,i)+xndpr2*Qext >*/
	    ext[l + i__ * 20 - 21] += xndpr2 * qext;
/*<          sca(l,i)=sca(l,i)+xndpr2*Qsca >*/
	    sca[l + i__ * 20 - 21] += xndpr2 * qsca;
/* phase function for each type of particle */
/*<          do k=1,nbmu >*/
	    i__2 = nbmu;
	    for (k = 1; k <= i__2; ++k) {
/*<           p1(l,i,k)=p1(l,i,k)+4.D+00*p11(k)*xndpr2 >*/
		p1[l + (i__ + (k << 2)) * 20 - 101] += p11[k - 1] * 4. * 
			xndpr2;
/*< 	  if (ipol.ne.0)then >*/
		if (*ipol != 0) {
/*<             q1(l,i,k)=q1(l,i,k)+4.D+00*q11(k)*xndpr2 >*/
		    q1[l + (i__ + (k << 2)) * 20 - 101] += q11[k - 1] * 4. * 
			    xndpr2;
/*<             u1(l,i,k)=u1(l,i,k)+4.D+00*u11(k)*xndpr2 >*/
		    u1[l + (i__ + (k << 2)) * 20 - 101] += u11[k - 1] * 4. * 
			    xndpr2;
/*< 	  endif >*/
		}
/*<          enddo >*/
	    }
/*<        enddo >*/
	}
/*<   599  continue >*/
/* L599: */
/*<        vi(i)=vi(i)+r*r*r*nr*dr >*/
	vi[i__ - 1] += r__ * r__ * r__ * nr * dr;
/*<        r=r+dr >*/
	r__ += dr;
/*<        dr=r*(10**rlogpas-1.D+00) >*/
	dr = r__ * (pow_dd(&c_b5, &rlogpas) - 1.);
/*<        if(r.ge.rmaxd) goto 600 >*/
	if (r__ >= rmaxd) {
	    goto L600;
	}
/*<        goto 123 >*/
	goto L123;
/*<   600 continue >*/
L600:
	;
    }
/* NOW WE MIXT THE DIFFERENT TYPES OF PARTICLE */
/* Computation of the scattering and extinction coefficients. */
/* The coefficients will be normalized at 0.550 micron. */
/*<        sigm=0. >*/
    sigm = 0.;
/*<        do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        vi(i)=4*pi*vi(i)/3 >*/
	vi[i__ - 1] = pi * 4 * vi[i__ - 1] / 3;
/*<        sigm=sigm+cij(i)/vi(i) >*/
	sigm += mie_in__1.cij[i__ - 1] / vi[i__ - 1];
/*<        enddo >*/
    }
/*     cij coefficients calculation */
/*<          do j=1,icp >*/
    i__1 = mie_in__1.icp;
    for (j = 1; j <= i__1; ++j) {
/*<          cij(j)=(cij(j)/vi(j)/sigm) >*/
	mie_in__1.cij[j - 1] = mie_in__1.cij[j - 1] / vi[j - 1] / sigm;
/*<          enddo  >*/
    }
/*<       do l=1,20 >*/
    for (l = 1; l <= 20; ++l) {
/*<        do i=1,icp >*/
	i__1 = mie_in__1.icp;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<          ex(1,l)=ex(1,l)+cij(i)*real(ext(l,i)) >*/
	    ex[(l << 2) + 1] += mie_in__1.cij[i__ - 1] * ext[l + i__ * 20 - 
		    21];
/*<          sc(1,l)=sc(1,l)+cij(i)*real(sca(l,i)) >*/
	    sc[(l << 2) + 1] += mie_in__1.cij[i__ - 1] * sca[l + i__ * 20 - 
		    21];
/*<        enddo >*/
	}
/*<       enddo >*/
    }
/* computation of the phase functions and the asymetry coefficient */
/* of the mixture of particles */
/*<       do l=1,20 >*/
    for (l = 1; l <= 20; ++l) {
/*<         asy_n=0. >*/
	asy_n__ = 0.;
/*<         asy_d=0. >*/
	asy_d__ = 0.;
/*<         do k=1,nbmu >*/
	i__1 = nbmu;
	for (k = 1; k <= i__1; ++k) {
/*<           ph(l,k)=0. >*/
	    sixs_aerbas__1.ph[l + k * 20 - 21] = 0.;
/*<           do i=1,icp >*/
	    i__2 = mie_in__1.icp;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/*<            ph(l,k)=ph(l,k)+real(cij(i)*p1(l,i,k)) >*/
		sixs_aerbas__1.ph[l + k * 20 - 21] += mie_in__1.cij[i__ - 1] *
			 p1[l + (i__ + (k << 2)) * 20 - 101];
/*<           enddo >*/
	    }
/*<           ph(l,k)=ph(l,k)/sc(1,l) >*/
	    sixs_aerbas__1.ph[l + k * 20 - 21] /= sc[(l << 2) + 1];
/*< 	  if (ipol.ne.0)then >*/
	    if (*ipol != 0) {
/*< 	    qh(l,k)=0. >*/
		sixs_aerbas__1.qh[l + k * 20 - 21] = 0.;
/*< 	    uh(l,k)=0. >*/
		sixs_aerbas__1.uh[l + k * 20 - 21] = 0.;
/*< 	    do i=1,icp >*/
		i__2 = mie_in__1.icp;
		for (i__ = 1; i__ <= i__2; ++i__) {
/*<               qh(l,k)=qh(l,k)+real(cij(i)*q1(l,i,k)) >*/
		    sixs_aerbas__1.qh[l + k * 20 - 21] += mie_in__1.cij[i__ - 
			    1] * q1[l + (i__ + (k << 2)) * 20 - 101];
/*<               uh(l,k)=uh(l,k)+real(cij(i)*u1(l,i,k)) >*/
		    sixs_aerbas__1.uh[l + k * 20 - 21] += mie_in__1.cij[i__ - 
			    1] * u1[l + (i__ + (k << 2)) * 20 - 101];
/*< 	    enddo >*/
		}
/*<             qh(l,k)=qh(l,k)/sc(1,l) >*/
		sixs_aerbas__1.qh[l + k * 20 - 21] /= sc[(l << 2) + 1];
/*<             uh(l,k)=uh(l,k)/sc(1,l) >*/
		sixs_aerbas__1.uh[l + k * 20 - 21] /= sc[(l << 2) + 1];
/*< 	    xmug=180/pi*acos(cgaus_S(k)) >*/
		xmug = 180 / pi * acos(cgaus_s__[k - 1]);
/*< 	  endif >*/
	    }
/*< 	  asy_n=asy_n+cgaus_S(k)*ph(l,k)*pdgs_S(k) >*/
	    asy_n__ += cgaus_s__[k - 1] * sixs_aerbas__1.ph[l + k * 20 - 21] *
		     pdgs_s__[k - 1];
/*< 	  asy_d=asy_d+ph(l,k)*pdgs_S(k) >*/
	    asy_d__ += sixs_aerbas__1.ph[l + k * 20 - 21] * pdgs_s__[k - 1];
/*<         enddo >*/
	}
/*< 	  asy(1,l)=asy_n/asy_d >*/
	asy[(l << 2) + 1] = asy_n__ / asy_d__;
/*<       enddo >*/
    }
/*<       do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<         ex(i,l)=ex(i,l)/np(i) >*/
	    ex[i__ + (l << 2)] /= np[i__ - 1];
/*<         sc(i,l)=sc(i,l)/np(i)  >*/
	    sc[i__ + (l << 2)] /= np[i__ - 1];
/*<        enddo >*/
	}
/*<       enddo >*/
    }
/*<       return >*/
    return 0;
/*<       END                                                                >*/
} /* mie_ */

/****************************************************************************
*/
/* Using the Mie's theory, this subroutine computes the scattering and */
/*extinction efficiency factors (usually written Qsca and Qext) and it also*/
/* computes the scattering intensity efficiency */
/*<    >*/
/* Subroutine */ int exscphase_(doublereal *x, doublereal *nr, doublereal *ni,
	 doublereal *qext, doublereal *qsca, doublereal *p11, doublereal *q11,
	 doublereal *u11, integer *ipol, doublereal *cgaus_s__, doublereal *
	pdgs_s__)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    /* Subroutine */ int s_stop(char *, ftnlen);
    double cos(doublereal), sin(doublereal), pow_dd(doublereal *, doublereal *
	    );

    /* Local variables */
    doublereal xnumidny, xnumrdny;
    integer j, k, n;
    doublereal y;
    integer np;
    doublereal rn[10000001], xj[10000001];
    integer mu;
    doublereal up, xy[10000002], is1, is2;
    integer mu1, mu2;
    doublereal rs1, rs2, ian[10000001], ibn[10000001], ran[10000001], rbn[
	    10000001], imn, ren;
    integer mub;
    doublereal pin[10000001], ianb, ibnb, co_n__, ranb, rbnb;
    integer nbmu;
    doublereal coxj, idny[10000001], ignx[10000001], taun[10000001], rdnx[
	    10000001], rdny[10000001], xmud, rgnx[10000001], test, xjonh, 
	    xpond, xdenan, xdenbn, xden1an, xden2an, xden1bn, xden2bn, 
	    xnum1an, xnum2an, xnum1bn, xnum2bn, xdendny, xdengnx;

    /* Fortran I/O blocks */
    static cilist io___49 = { 0, 6, 0, 0, 0 };
    static cilist io___50 = { 0, 6, 0, 0, 0 };
    static cilist io___86 = { 0, 6, 0, 0, 0 };


/*<       parameter (nser=10000000)       >*/
/*<       double precision Ren,Imn,X,Y,Up,XnumRDnY,XnumIDnY >*/
/*<       double precision XdenDnY,coxj,Qsca,Qext,xJonH,XdenGNX >*/
/*<       double precision Xnum1An,Xnum2An,XdenAn,Xden1An,Xden2An,RAnb,IAnb >*/
/*<       double precision Xnum1Bn,Xnum2Bn,XdenBn,Xden1Bn,Xden2Bn,RBnb,IBnb >*/
/*<       double precision xmud,xpond,RS1,RS2,IS1,IS2,co_n,test >*/
/*<       double precision xj(0:nser),xy(-1:nser),Rn(0:nser) >*/
/*<       double precision IDnY(0:nser),RDnX(0:nser),RDnY(0:nser) >*/
/*<       double precision IGnX(0:nser),RGnX(0:nser) >*/
/*<       double precision RAn(0:nser),IAn(0:nser),RBn(0:nser),IBn(0:nser) >*/
/*<       double precision TAUn(0:nser),PIn(0:nser) >*/
/*<       real nr,ni >*/
/*<       integer N,Np,mu,mub,mu1,mu2,k,nbmu,j >*/
/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       double precision p11(nqmax_p),q11(nqmax_p),u11(nqmax_p) >*/
/*<       real cgaus_S(nqmax_p), pdgs_S(nqmax_p) >*/
/* - to vary the number of quadratures */
/*<       nbmu=nquad       >*/
    /* Parameter adjustments */
    --pdgs_s__;
    --cgaus_s__;
    --u11;
    --q11;
    --p11;

    /* Function Body */
    nbmu = num_quad__1.nquad;
/*<       Ren=nr/(nr*nr+ni*ni) >*/
    ren = *nr / (*nr * *nr + *ni * *ni);
/*<       Imn=ni/(nr*nr+ni*ni) >*/
    imn = *ni / (*nr * *nr + *ni * *ni);
/* jcc */
/*<       Y=X*sqrt(nr*nr+ni*ni) >*/
    y = *x * sqrt(*nr * *nr + *ni * *ni);
/* jcc */
/* ---Identification of the greater order of computation (=mu) */
/*    as defined by F.J. Corbato, J. Assoc. Computing Machinery, 1959, */
/*    6, 366-375 */
/* jcc  N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*X*X)))+1 */
/*<       N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*Y*Y)))+1 >*/
    n = (integer) ((sqrt(y * 4. * y + 1.) - 1.) * .5) + 1;
/*In certain this value of N above leads to a value of Up~1 and big proble
ms*/
/*     N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*Y*Y)))+3 */
/* jcc */
/*<       if (N.eq.1)N=2 >*/
    if (n == 1) {
	n = 2;
    }
/*<       mu2=1000000 >*/
    mu2 = 1000000;
/*<       Np=N >*/
    np = n;
/* jcc */
/*     Up=2.D+00*X/(2.D+00*Np+1.D+00) */
/*<       Up=2.D+00*Y/(2.D+00*Np+1.D+00) >*/
    up = y * 2. / (np * 2. + 1.);
/* jcc */
/*<       mu1=int(Np+30.*(0.10+0.35*Up*(2-Up*Up)/2./(1-Up))) >*/
    mu1 = (integer) (np + (up * .35 * (2 - up * up) / 2. / (1 - up) + .1) * 
	    30.);
/* jcc */
/*     Np=int(X-0.5D+00+dsqrt(30.*0.35*X)) */
/*<       Np=int(Y-0.5D+00+dsqrt(30.*0.35*Y)) >*/
    np = (integer) (y - .5 + sqrt(y * 10.5));
/* jcc */
/*<       if (Np.gt.N)then >*/
    if (np > n) {
/* jcc */
/*      Up=2.D+00*X/(2.D+00*Np+1.D+00) */
/*<        Up=2.D+00*Y/(2.D+00*Np+1.D+00) >*/
	up = y * 2. / (np * 2. + 1.);
/* jcc */
/*<        mu2=int(Np+30.*(0.10+0.35*Up*(2-Up*Up)/2./(1-Up))) >*/
	mu2 = (integer) (np + (up * .35 * (2 - up * up) / 2. / (1 - up) + .1) 
		* 30.);
/*<       endif >*/
    }
/*<       mu=min0(mu1,mu2) >*/
    mu = min(mu1,mu2);
/*<       if (mu.ge.nser) then >*/
    if (mu >= 10000000) {
/*<          write(6,*) " Error, nser is too small, mu is equal to : ",mu >*/
	s_wsle(&io___49);
	do_lio(&c__9, &c__1, " Error, nser is too small, mu is equal to : ", 
		44L);
	do_lio(&c__3, &c__1, (char *)&mu, (ftnlen)sizeof(integer));
	e_wsle();
/*<          Stop >*/
	s_stop("", 0L);
/*<          endif >*/
    }
/*<       if (mu.le.0) then >*/
    if (mu <= 0) {
/*<          write(6,*) " Error, mu is too small, mu is equal to : ",mu >*/
	s_wsle(&io___50);
	do_lio(&c__9, &c__1, " Error, mu is too small, mu is equal to : ", 
		42L);
	do_lio(&c__3, &c__1, (char *)&mu, (ftnlen)sizeof(integer));
	e_wsle();
/*<          Stop >*/
	s_stop("", 0L);
/*<          endif >*/
    }
/* --- Identification of the transition line. Below this line the Bessel 
*/
/*     function j behaves as oscillating functions. Above the behavior */
/*     becomes monotonic. We start at a order greater than this transition
 */
/*     line (order max=mu) because a downward recursion is called for. */
/*<       Rn(mu)=0.D+00 >*/
    rn[mu] = 0.;
/*<       k=mu+1 >*/
    k = mu + 1;
/*<  149  continue >*/
L149:
/*<       k=k-1 >*/
    --k;
/*<       xj(k)=0.D+00 >*/
    xj[k] = 0.;
/*<       Rn(k-1)=X/(2.D+00*k+1.D+00-X*Rn(k)) >*/
    rn[k - 1] = *x / (k * 2. + 1. - *x * rn[k]);
/*<       if (k.eq.2)then >*/
    if (k == 2) {
/*< 	  mub=mu >*/
	mub = mu;
/*< 	  xj(mub+1)=0.D+00 >*/
	xj[mub + 1] = 0.;
/*< 	  xj(mub)=1.D+00 >*/
	xj[mub] = 1.;
/*< 	  goto 150 >*/
	goto L150;
/*<       endif >*/
    }
/*<       if (Rn(k-1).gt.1.D+00)then >*/
    if (rn[k - 1] > 1.) {
/*< 	  mub=k-1 >*/
	mub = k - 1;
/*< 	  xj(mub+1)=Rn(mub) >*/
	xj[mub + 1] = rn[mub];
/*< 	  xj(mub)=1.D+00 >*/
	xj[mub] = 1.;
/*< 	  goto 150 >*/
	goto L150;
/*<       endif >*/
    }
/*<       goto 149 >*/
    goto L149;
/*<  150  continue >*/
L150:
/*<       do k=mub,1,-1 >*/
    for (k = mub; k >= 1; --k) {
/*< 	xj(k-1)=(2.D+00*k+1.D+00)*xj(k)/X-xj(k+1) >*/
	xj[k - 1] = (k * 2. + 1.) * xj[k] / *x - xj[k + 1];
/*<       enddo >*/
    }
/*<       coxj=(xj(0)-X*xj(1))*dcos(X)+X*xj(0)*sin(X) >*/
    coxj = (xj[0] - *x * xj[1]) * cos(*x) + *x * xj[0] * sin(*x);
/* --- Computation Dn(alpha) and Dn(alpha*m) (cf MIE's theory) */
/*     downward recursion    - real and imaginary parts */
/*<       RDnY(mu)=0.D+00 >*/
    rdny[mu] = 0.;
/*<       IDnY(mu)=0.D+00 >*/
    idny[mu] = 0.;
/*<       RDnX(mu)=0.D+00 >*/
    rdnx[mu] = 0.;
/*<       do k=mu,1,-1 >*/
    for (k = mu; k >= 1; --k) {
/*< 	 RDnX(k-1)=k/X-1.D+00/(RDnX(k)+k/X) >*/
	rdnx[k - 1] = k / *x - 1. / (rdnx[k] + k / *x);
/*< 	 XnumRDnY=RDnY(k)+Ren*k/X >*/
	xnumrdny = rdny[k] + ren * k / *x;
/*< 	 XnumIDnY=IDnY(k)+Imn*k/X >*/
	xnumidny = idny[k] + imn * k / *x;
/*< 	 XdenDnY=XnumRDnY*XnumRDnY+XnumIDnY*XnumIDnY >*/
	xdendny = xnumrdny * xnumrdny + xnumidny * xnumidny;
/*< 	 RDnY(k-1)=k*Ren/X-XnumRDnY/XdenDnY >*/
	rdny[k - 1] = k * ren / *x - xnumrdny / xdendny;
/*< 	 IDnY(k-1)=k*Imn/X+XnumIDnY/XdenDnY >*/
	idny[k - 1] = k * imn / *x + xnumidny / xdendny;
/*<       enddo  >*/
    }
/* --- Initialization of the upward recursions */
/*<       xy(-1)=dsin(x)/x >*/
    xy[0] = sin(*x) / *x;
/*<       xy(0)=-dcos(x)/x >*/
    xy[1] = -cos(*x) / *x;
/*<       RGnX(0)=0.D+00 >*/
    rgnx[0] = 0.;
/*<       IGnX(0)=-1.D+00 >*/
    ignx[0] = -1.;
/*<       Qsca=0.D+00 >*/
    *qsca = 0.;
/*<       Qext=0.D+00 >*/
    *qext = 0.;
/*<       do k=1,mu >*/
    i__1 = mu;
    for (k = 1; k <= i__1; ++k) {
/*< 	 if (k.le.mub)then >*/
	if (k <= mub) {
/*< 	   xj(k)=xj(k)/coxj >*/
	    xj[k] /= coxj;
/*< 	 else >*/
	} else {
/*< 	   xj(k)=Rn(k-1)*xj(k-1) >*/
	    xj[k] = rn[k - 1] * xj[k - 1];
/*< 	 endif >*/
	}
/* --- Computation of bessel's function y(alpha) */
/*< 	 xy(k)=(2.D+00*k-1.D+00)*xy(k-1)/X-xy(k-2) >*/
	xy[k + 1] = (k * 2. - 1.) * xy[k] / *x - xy[k - 1];
/*< 	 xJonH=xj(k)/(xj(k)*xj(k)+xy(k)*xy(k)) >*/
	xjonh = xj[k] / (xj[k] * xj[k] + xy[k + 1] * xy[k + 1]);
/* --- Computation of Gn(alpha), Real and Imaginary part */
/*<          XdenGNX=(RGnX(k-1)-k/X)**2.D+00+IGnX(k-1)*IGnX(k-1) >*/
	d__1 = rgnx[k - 1] - k / *x;
	xdengnx = pow_dd(&d__1, &c_b14) + ignx[k - 1] * ignx[k - 1];
/*< 	 RGnX(k)=(k/X-RGnX(k-1))/XdenGNX-k/X >*/
	rgnx[k] = (k / *x - rgnx[k - 1]) / xdengnx - k / *x;
/*< 	 IGnX(k)=IGnX(k-1)/XdenGNX >*/
	ignx[k] = ignx[k - 1] / xdengnx;
/* --- Computation of An(alpha) and Bn(alpha), Real and Imaginary part
 */
/*< 	 Xnum1An=RDnY(k)-nr*RDnX(k) >*/
	xnum1an = rdny[k] - *nr * rdnx[k];
/*< 	 Xnum2An=IDnY(k)+ni*RDnX(k) >*/
	xnum2an = idny[k] + *ni * rdnx[k];
/*< 	 Xden1An=RDnY(k)-nr*RGnX(k)-ni*IGnX(k) >*/
	xden1an = rdny[k] - *nr * rgnx[k] - *ni * ignx[k];
/*< 	 Xden2An=IDnY(k)+ni*RGnX(k)-nr*IGnX(k) >*/
	xden2an = idny[k] + *ni * rgnx[k] - *nr * ignx[k];
/*< 	 XdenAn=Xden1An*Xden1An+Xden2An*Xden2An >*/
	xdenan = xden1an * xden1an + xden2an * xden2an;
/*< 	 RAnb=(Xnum1An*Xden1An+Xnum2An*Xden2An)/XdenAn >*/
	ranb = (xnum1an * xden1an + xnum2an * xden2an) / xdenan;
/*< 	 IAnb=(-Xnum1An*Xden2An+Xnum2An*Xden1An)/XdenAn >*/
	ianb = (-xnum1an * xden2an + xnum2an * xden1an) / xdenan;
/*< 	 RAn(k)=xJonH*(xj(k)*RAnb-xy(k)*IAnb) >*/
	ran[k] = xjonh * (xj[k] * ranb - xy[k + 1] * ianb);
/*< 	 IAn(k)=xJonH*(xy(k)*RAnb+xj(k)*IAnb) >*/
	ian[k] = xjonh * (xy[k + 1] * ranb + xj[k] * ianb);
/*< 	 Xnum1Bn=nr*RDnY(k)+ni*IDnY(k)-RDnX(k) >*/
	xnum1bn = *nr * rdny[k] + *ni * idny[k] - rdnx[k];
/*< 	 Xnum2Bn=nr*IDnY(k)-ni*RDnY(k) >*/
	xnum2bn = *nr * idny[k] - *ni * rdny[k];
/*< 	 Xden1Bn=nr*RDnY(k)+ni*IDnY(k)-RGnX(k) >*/
	xden1bn = *nr * rdny[k] + *ni * idny[k] - rgnx[k];
/*< 	 Xden2Bn=nr*IDnY(k)-ni*RDnY(k)-IGnX(k) >*/
	xden2bn = *nr * idny[k] - *ni * rdny[k] - ignx[k];
/*< 	 XdenBn=Xden1Bn*Xden1Bn+Xden2Bn*Xden2Bn >*/
	xdenbn = xden1bn * xden1bn + xden2bn * xden2bn;
/*< 	 RBnb=(Xnum1Bn*Xden1Bn+Xnum2Bn*Xden2Bn)/XdenBn >*/
	rbnb = (xnum1bn * xden1bn + xnum2bn * xden2bn) / xdenbn;
/*< 	 IBnb=(-Xnum1Bn*Xden2Bn+Xnum2Bn*Xden1Bn)/XdenBn >*/
	ibnb = (-xnum1bn * xden2bn + xnum2bn * xden1bn) / xdenbn;
/*< 	 RBn(k)=xJonH*(xj(k)*RBnb-xy(k)*IBnb) >*/
	rbn[k] = xjonh * (xj[k] * rbnb - xy[k + 1] * ibnb);
/*< 	 IBn(k)=xJonH*(xy(k)*RBnb+xj(k)*IBnb) >*/
	ibn[k] = xjonh * (xy[k + 1] * rbnb + xj[k] * ibnb);
/* ---Criterion on the recursion formulas as defined by D. Deirmendjia
n */
/*    et al., J. Opt. Soc. Am., 1961, 51, 6, 620-633 */
/*<  	 test=(RAn(k)**2.+IAn(k)**2.+RBn(k)**2.+IBn(k)**2.)/k >*/
	test = (pow_dd(&ran[k], &c_b14) + pow_dd(&ian[k], &c_b14) + pow_dd(&
		rbn[k], &c_b14) + pow_dd(&ibn[k], &c_b14)) / k;
/*<  	 if (test.lt.1.0D-14)then >*/
	if (test < 1e-14) {
/*<            mu=k >*/
	    mu = k;
/*<            if (mu.le.0) then >*/
	    if (mu <= 0) {
/*<             write(6,*) "attention mu <0 ",mu >*/
		s_wsle(&io___86);
		do_lio(&c__9, &c__1, "attention mu <0 ", 16L);
		do_lio(&c__3, &c__1, (char *)&mu, (ftnlen)sizeof(integer));
		e_wsle();
/*<             endif >*/
	    }
/*<            goto 400 >*/
	    goto L400;
/*<          endif >*/
	}
/* --- Computation of the scattering and extinction efficiency factor 
*/
/*<          xpond=2.D+00/X/X*(2.D+00*k+1) >*/
	xpond = 2. / *x / *x * (k * 2. + 1);
/*<          Qsca=Qsca+xpond*(RAn(k)**2.+IAn(k)**2.+RBn(k)**2.+IBn(k)**2.) >*/
	*qsca += xpond * (pow_dd(&ran[k], &c_b14) + pow_dd(&ian[k], &c_b14) + 
		pow_dd(&rbn[k], &c_b14) + pow_dd(&ibn[k], &c_b14));
/*<          Qext=Qext+xpond*(RAn(k)+RBn(k)) >*/
	*qext += xpond * (ran[k] + rbn[k]);
/*<       enddo >*/
    }
/*<  400  continue >*/
L400:
/* --- Computation of the amplitude functions S1 and S2 (cf MIE's theory) 
*/
/*     defined by PIn, TAUn, An and Bn with PIn and TAUn related to the */
/*     Legendre polynomials. */
/*<       do j=1,nbmu >*/
    i__1 = nbmu;
    for (j = 1; j <= i__1; ++j) {
/*< 	 xmud=cgaus_S(j) >*/
	xmud = cgaus_s__[j];
/*< 	 RS1=0.D+00 >*/
	rs1 = 0.;
/*< 	 RS2=0.D+00 >*/
	rs2 = 0.;
/*< 	 IS1=0.D+00 >*/
	is1 = 0.;
/*< 	 IS2=0.D+00 >*/
	is2 = 0.;
/*< 	 PIn(0)=0.D+00 >*/
	pin[0] = 0.;
/*< 	 PIn(1)=1.D+00 >*/
	pin[1] = 1.;
/*< 	 TAUn(1)=xmud >*/
	taun[1] = xmud;
/*< 	 do k=1,mu >*/
	i__2 = mu;
	for (k = 1; k <= i__2; ++k) {
/*<           co_n=(2.D+00*k+1.D+00)/k/(k+1.D+00) >*/
	    co_n__ = (k * 2. + 1.) / k / (k + 1.);
/*< 	  RS1=RS1+co_n*(RAn(k)*PIn(k)+RBn(k)*TAUn(k)) >*/
	    rs1 += co_n__ * (ran[k] * pin[k] + rbn[k] * taun[k]);
/*< 	  RS2=RS2+co_n*(RAn(k)*TAUn(k)+RBn(k)*PIn(k)) >*/
	    rs2 += co_n__ * (ran[k] * taun[k] + rbn[k] * pin[k]);
/*< 	  IS1=IS1+co_n*(IAn(k)*PIn(k)+IBn(k)*TAUn(k)) >*/
	    is1 += co_n__ * (ian[k] * pin[k] + ibn[k] * taun[k]);
/*< 	  IS2=IS2+co_n*(IAn(k)*TAUn(k)+IBn(k)*PIn(k)) >*/
	    is2 += co_n__ * (ian[k] * taun[k] + ibn[k] * pin[k]);
/*<           PIn(k+1)=((2.D+00*k+1)*xmud*PIn(k)-(k+1.D+00)*PIn(k-1))/k >*/
	    pin[k + 1] = ((k * 2. + 1) * xmud * pin[k] - (k + 1.) * pin[k - 1]
		    ) / k;
/*<           TAUn(k+1)=(k+1.D+00)*xmud*PIn(k+1)-(k+2.D+00)*PIn(k) >*/
	    taun[k + 1] = (k + 1.) * xmud * pin[k + 1] - (k + 2.) * pin[k];
/*<          enddo >*/
	}
/* --- Computation of the scattering intensity efficiency */
/*<          p11(j)=(RS1*RS1+IS1*IS1+RS2*RS2+IS2*IS2)/X/X/2.D+00 >*/
	p11[j] = (rs1 * rs1 + is1 * is1 + rs2 * rs2 + is2 * is2) / *x / *x / 
		2.;
/*< 	 if (ipol.ne.0)then >*/
	if (*ipol != 0) {
/*<            q11(j)=(RS2*RS2+IS2*IS2-RS1*RS1-IS1*IS1)/X/X/2.D+00 >*/
	    q11[j] = (rs2 * rs2 + is2 * is2 - rs1 * rs1 - is1 * is1) / *x / *
		    x / 2.;
/*<            u11(j)=(2.D+00*RS2*RS1+2.D+00*IS2*IS1)/X/X/2.D+00 >*/
	    u11[j] = (rs2 * 2. * rs1 + is2 * 2. * is1) / *x / *x / 2.;
/*< 	 endif >*/
	}
/*<       enddo >*/
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* exscphase_ */

#ifdef __cplusplus
	}
#endif
