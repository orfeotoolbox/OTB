/* TRUNCA.f -- translated by f2c (version 19970805).
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

/* Table of constant values */

static doublereal c_b2 = -1.;
static doublereal c_b3 = 1.;

/*<       subroutine trunca(coeff,ipol) >*/
/* Subroutine */ int trunca_(doublereal *coeff, integer *ipol)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    doublereal d__, e;
    integer i__, j, k;
    doublereal x, c2;
    integer mm;
    doublereal pl[1002];
    integer nn;
    doublereal rm, xx, co1, co2, co3, z1p, pol[1001], som1, som2, som3, som4;
    integer nbmu;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    doublereal deltal[1001], pdgs_s__[1000], cgaus_s__[1000];

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real cgaus_S(nqmax_p), pdgs_S(nqmax_p) >*/
/*<       real pl(-1:nqmax_p),pol(0:nqmax_p),deltal(0:nqmax_p) >*/
/*<       real pha,qha,uha,alphal,betal,gammal,zetal >*/
/*<    >*/
/* - to vary the number of quadratures */
/*<       real aa,x1,x2,a,x,rm,z1,z1p,e,d,co1,co2,co3,xx,c2,xp >*/
/*<       real som1,som2,som3,som4,coef >*/
/*<       integer nbmu,k,j,kk,i,nn,mm >*/
/*<       nbmu=nquad >*/
    nbmu = num_quad__1.nquad;
/* - calculation of gauss points */
/*<       cgaus_S(nbmu)=1.0 >*/
    cgaus_s__[nbmu - 1] = 1.;
/*<       pdgs_S(nbmu)=0.0 >*/
    pdgs_s__[nbmu - 1] = 0.;
/*<       call gauss(-1.,1.,cgaus_S,pdgs_S,nbmu-2) >*/
    i__1 = nbmu - 2;
    gauss_(&c_b2, &c_b3, cgaus_s__, pdgs_s__, &i__1);
/*<       do j=1,nbmu-2 >*/
    i__1 = nbmu - 2;
    for (j = 1; j <= i__1; ++j) {
/*<        cgaus_S(nbmu-j)=cgaus_S(nbmu-j-1) >*/
	cgaus_s__[nbmu - j - 1] = cgaus_s__[nbmu - j - 2];
/*<        pdgs_S(nbmu-j)=pdgs_S(nbmu-j-1) >*/
	pdgs_s__[nbmu - j - 1] = pdgs_s__[nbmu - j - 2];
/*<       enddo >*/
    }
/*<       cgaus_S(1)=-1.0 >*/
    cgaus_s__[0] = -1.;
/*<       pdgs_S(1)=0.0 >*/
    pdgs_s__[0] = 0.;
/*      write(6,*) 'TRUNCA - gauss' */
/*      do j=1,nbmu */
/*      write(6,*) j,cgaus_S(j),pdgs_S(j) */
/*      enddo */
/*      Stop */
/* - calculation of gauss points */
/* Computations of Legendre coefficients */
/*<       do 10 k=0,nbmu-3 >*/
    i__1 = nbmu - 3;
    for (k = 0; k <= i__1; ++k) {
/*< 	alphal(k)=0. >*/
	sixs_polar__1.alphal[k] = 0.;
/*<         betal(k)=0. >*/
	sixs_polar__1.betal[k] = 0.;
/*< 	gammal(k)=0. >*/
	sixs_polar__1.gammal[k] = 0.;
/*< 	deltal(k)=0. >*/
	deltal[k] = 0.;
/*< 	zetal(k)=0. >*/
	sixs_polar__1.zetal[k] = 0.;
/*<    10 continue >*/
/* L10: */
    }
/*<       do 11 j=1,nbmu >*/
    i__1 = nbmu;
    for (j = 1; j <= i__1; ++j) {
/*<         x=pha(j)*pdgs_S(j) >*/
	x = sixs_polar__1.pha[j - 1] * pdgs_s__[j - 1];
/*<         rm=cgaus_S(j) >*/
	rm = cgaus_s__[j - 1];
/*<         pl(-1)=0. >*/
	pl[0] = 0.;
/*<         pl(0)=1. >*/
	pl[1] = 1.;
/*<         do 12 k=0,nbmu-3 >*/
	i__2 = nbmu - 3;
	for (k = 0; k <= i__2; ++k) {
/*<           pl(k+1)=((2*k+1.)*rm*pl(k)-k*pl(k-1))/(k+1.) >*/
	    pl[k + 2] = (((k << 1) + 1.) * rm * pl[k + 1] - k * pl[k]) / (k + 
		    1.);
/*<           betal(k)=betal(k)+x*pl(k) >*/
	    sixs_polar__1.betal[k] += x * pl[k + 1];
/*<   12    continue >*/
/* L12: */
	}
/*<   11  continue >*/
/* L11: */
    }
/*<       do 13 k=0,nbmu-3 >*/
    i__1 = nbmu - 3;
    for (k = 0; k <= i__1; ++k) {
/*<         betal(k)=(2*k+1.)*0.5*betal(k) >*/
	sixs_polar__1.betal[k] = ((k << 1) + 1.) * .5 * sixs_polar__1.betal[k]
		;
/* - to put negative coefficients to 0 */
/*<       if (betal(k).lt.0) then >*/
	if (sixs_polar__1.betal[k] < 0.) {
/*<         do j=k,nbmu-3 >*/
	    i__2 = nbmu - 3;
	    for (j = k; j <= i__2; ++j) {
/*< 	  betal(j)=0.0 >*/
		sixs_polar__1.betal[j] = 0.;
/*< 	enddo >*/
	    }
/*< 	goto 133 >*/
	    goto L133;
/*<       endif >*/
	}
/* - to put negative coefficients to 0 */
/*<   13  continue >*/
/* L13: */
    }
/*<   133 continue >*/
L133:
/*    cases of polarization */
/*<       if (ipol.ne.0)then >*/
    if (*ipol != 0) {
/*<         do 14 j=1,nbmu >*/
	i__1 = nbmu;
	for (j = 1; j <= i__1; ++j) {
/*< 	  x=qha(j)*pdgs_S(j) >*/
	    x = sixs_polar__1.qha[j - 1] * pdgs_s__[j - 1];
/*< 	  xx=uha(j)*pdgs_S(j) >*/
	    xx = sixs_polar__1.uha[j - 1] * pdgs_s__[j - 1];
/*< 	  rm=cgaus_S(j) >*/
	    rm = cgaus_s__[j - 1];
/*< 	  pol(0)=0. >*/
	    pol[0] = 0.;
/*< 	  pol(1)=0. >*/
	    pol[1] = 0.;
/*< 	  pol(2)=3.*(1.-rm**2)/2./sqrt(6.0) >*/
/* Computing 2nd power */
	    d__1 = rm;
	    pol[2] = (1. - d__1 * d__1) * 3. / 2. / sqrt(6.);
/*< 	  pl(-1)=0. >*/
	    pl[0] = 0.;
/*< 	  pl(0)=1. >*/
	    pl[1] = 1.;
/*< 	  do 15 k=2,nbmu-3 >*/
	    i__2 = nbmu - 3;
	    for (k = 2; k <= i__2; ++k) {
/*< 	    d=(2.*k+1.)/sqrt((k+3)*(k-1.)) >*/
		d__ = (k * 2. + 1.) / sqrt((k + 3) * (k - 1.));
/*< 	    e=sqrt((k+2.)*(k-2.))/(2.*k+1.) >*/
		e = sqrt((k + 2.) * (k - 2.)) / (k * 2. + 1.);
/*< 	    pol(k+1)=d*(rm*pol(k)-e*pol(k-1)) >*/
		pol[k + 1] = d__ * (rm * pol[k] - e * pol[k - 1]);
/*< 	    gammal(k)=gammal(k)+x*pol(k) >*/
		sixs_polar__1.gammal[k] += x * pol[k];
/*<  15       continue >*/
/* L15: */
	    }
/*< 	  do 16 k=0,nbmu-3 >*/
	    i__2 = nbmu - 3;
	    for (k = 0; k <= i__2; ++k) {
/*< 	    pl(k+1)=((2.*k+1.)*rm*pl(k)-k*pl(k-1))/(k+1.) >*/
		pl[k + 2] = ((k * 2. + 1.) * rm * pl[k + 1] - k * pl[k]) / (k 
			+ 1.);
/*< 	    deltal(k)=deltal(k)+xx*pl(k) >*/
		deltal[k] += xx * pl[k + 1];
/*<  16       continue >*/
/* L16: */
	    }
/*<  14     continue >*/
/* L14: */
	}
/*< 	do 17 k=0,nbmu-3 >*/
	i__1 = nbmu - 3;
	for (k = 0; k <= i__1; ++k) {
/*< 	  deltal(k)=deltal(k)*(2.*k+1.)/2. >*/
	    deltal[k] = deltal[k] * (k * 2. + 1.) / 2.;
/*< 	  gammal(k)=gammal(k)*(2.*k+1.)/2. >*/
	    sixs_polar__1.gammal[k] = sixs_polar__1.gammal[k] * (k * 2. + 1.) 
		    / 2.;
/*<  17     continue >*/
/* L17: */
	}
/*< 	do 18 i=2,nbmu-3 >*/
	i__1 = nbmu - 3;
	for (i__ = 2; i__ <= i__1; ++i__) {
/*< 	  co1=4.*(2.*i+1.)/(i*(i-1.)*(i+1.)*(i+2.)) >*/
	    co1 = (i__ * 2. + 1.) * 4. / (i__ * (i__ - 1.) * (i__ + 1.) * (
		    i__ + 2.));
/*< 	  co2=i*(i-1.)/((i+1.)*(i+2.)) >*/
	    co2 = i__ * (i__ - 1.) / ((i__ + 1.) * (i__ + 2.));
/*< 	  co3=co2*deltal(i) >*/
	    co3 = co2 * deltal[i__];
/*< 	  co2=co2*betal(i) >*/
	    co2 *= sixs_polar__1.betal[i__];
/*< 	  nn=i/2. >*/
	    nn = (integer) (i__ / 2.);
/*< 	  mm=(i-1)/2. >*/
	    mm = (integer) ((i__ - 1) / 2.);
/*< 	  som1=0. >*/
	    som1 = 0.;
/*< 	  som2=0. >*/
	    som2 = 0.;
/*< 	  som3=0. >*/
	    som3 = 0.;
/*< 	  som4=0. >*/
	    som4 = 0.;
/*< 	  do 19 j=1,nn >*/
	    i__2 = nn;
	    for (j = 1; j <= i__2; ++j) {
/*< 	    c2=(i-1.)*(i-1.)-3.*(2*j-1.)*(i-j) >*/
		c2 = (i__ - 1.) * (i__ - 1.) - ((j << 1) - 1.) * 3. * (i__ - 
			j);
/*< 	    som1=som1+c2*betal(i-2*j) >*/
		som1 += c2 * sixs_polar__1.betal[i__ - (j << 1)];
/*< 	    som2=som2+c2*deltal(i-2*j) >*/
		som2 += c2 * deltal[i__ - (j << 1)];
/*<  19       continue >*/
/* L19: */
	    }
/*< 	  do 20 j=0,mm >*/
	    i__2 = mm;
	    for (j = 0; j <= i__2; ++j) {
/*< 	    c2=(i-1.)*(i-1.)-3.*j*(2*i-2*j-1.) >*/
		c2 = (i__ - 1.) * (i__ - 1.) - j * 3. * ((i__ << 1) - (j << 1)
			 - 1.);
/*< 	    som3=som3+c2*betal(i-2*j-1) >*/
		som3 += c2 * sixs_polar__1.betal[i__ - (j << 1) - 1];
/*< 	    som4=som4+c2*deltal(i-2*j-1) >*/
		som4 += c2 * deltal[i__ - (j << 1) - 1];
/*<  20       continue >*/
/* L20: */
	    }
/*< 	  zetal(i)=co3-co1*(som2-som3) >*/
	    sixs_polar__1.zetal[i__] = co3 - co1 * (som2 - som3);
/*< 	  alphal(i)=co2-co1*(som1-som4) >*/
	    sixs_polar__1.alphal[i__] = co2 - co1 * (som1 - som4);
/*<  18     continue >*/
/* L18: */
	}
/*<         z1p=betal(0) >*/
	z1p = sixs_polar__1.betal[0];
/*< 	do k=0,nbmu-3 >*/
	i__1 = nbmu - 3;
	for (k = 0; k <= i__1; ++k) {
/*< 	 alphal(k)=alphal(k)/z1p >*/
	    sixs_polar__1.alphal[k] /= z1p;
/*< 	 betal(k)=betal(k)/z1p >*/
	    sixs_polar__1.betal[k] /= z1p;
/*< 	 gammal(k)=gammal(k)/z1p >*/
	    sixs_polar__1.gammal[k] /= z1p;
/*< 	 zetal(k)=zetal(k)/z1p >*/
	    sixs_polar__1.zetal[k] /= z1p;
/*< 	enddo >*/
	}
/*<       endif >*/
    }
/*<       coeff=0.0 >*/
    *coeff = 0.;
/*<       return >*/
    return 0;
/*<       end >*/
} /* trunca_ */

#ifdef __cplusplus
	}
#endif
