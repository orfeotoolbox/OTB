/* POLGLIT.f -- translated by f2c (version 19970805).
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

static doublereal c_b3 = 2.;

/*<        Subroutine POLGLIT(xts,xtv,phi,wspd,azw,ropq,ropu) >*/
/* Subroutine */ int polglit_(doublereal *xts, doublereal *xtv, doublereal *
	phi, doublereal *wspd, doublereal *azw, doublereal *ropq, doublereal *
	ropu)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double acos(doublereal), cos(doublereal), sin(doublereal), sqrt(
	    doublereal), atan(doublereal), exp(doublereal);

    /* Local variables */
    doublereal m, r1, r2, r3, c21, c03, c40, c04, c22, cs, ni, pi, cv, xe, rl,
	     nr, rr, ss, xn, sv, zx, zy, xe2, xn2, sca, ksi, dtr, phw, mus, 
	    muv, csca, coef, cksi, tilt, sinv, alpha, proba, sigmac, factor, 
	    sigmau, tantilt;

/*<        real xts,xtv,phi,azw,wspd,ropq,ropu >*/
/*      compute the polarized components of the surface */
/*      to an agitated surface model */
/*   xts is the sun zenith angle in degrees */
/*   xtv is the view zenith angle in degrees */
/*   phi is the relative azimuth between sun and view in degrees */
/*   wspd is the wind speed for use in ponderating */
/*       azw=azim. of the sun - azim. of the wind (in deg.) */
/*   the polarization by the actual glint reflectance */
/*   ropq and ropu and the stokes parameter (Q,U) of the polarized */
/*   surface reflectance */
/*<         real pi,dtr,csca,sca,alpha,m,rl,rr,factor >*/
/*   csca is the cosine of the scattering angle (sca) */
/*   alpha is the incidence angle used for specular reflection */
/*   alphap is the refraction angle after specular reflection */
/*   m is the refractive index of water (fixed to 1.33) */
/*   rr and rl and parameters used in the reflection mattrix */
/*   computation. */
/*   factor is a multiplicative factor to account for agitated surface */
/* following is a set of intermediate variable used in the computation */
/*  of factor */
/*<       real phw >*/
/*<       real cs,cv,ss,sv,zx,zy,tantilt,tilt,proba,xe,xn,xe2,xn2 >*/
/*<       real coef,cos2chi,coschi,sinchi >*/
/*<       real sigmaC,sigmaU,C21,C03,C40,C04,C22 >*/
/*<       real mus,muv,sinv,cksi,ksi,r1f >*/
/*<       real nr,ni >*/
/*<         m=1.33	 >*/
    m = 1.33;
/*< 	nr=1.33 >*/
    nr = 1.33;
/*< 	ni=0.0 >*/
    ni = 0.;
/*<         pi=acos(0.0)*2.0 >*/
    pi = acos(0.) * 2.;
/*<         dtr=pi/180.0 >*/
    dtr = pi / 180.;
/*< 	c >*/
    csca = -cos(*xts * dtr) * cos(*xtv * dtr) - sin(*xts * dtr) * sin(*xtv * 
	    dtr) * cos(*phi * dtr);
/*<         sca=acos(csca) >*/
    sca = acos(csca);
/*< 	alpha=(pi-sca)/2.0 >*/
    alpha = (pi - sca) / 2.;
/* Originally from Deuze et al cannot mak it work to be investigated */
/*<  	 >*/
    rl = (sqrt(m * m - sin(alpha) * sin(alpha)) - m * m * cos(alpha)) / (sqrt(
	    m * m - sin(alpha) * sin(alpha)) + m * m * cos(alpha));

/*<  	 >*/
    rr = (cos(alpha) - sqrt(m * m - sin(alpha) * sin(alpha))) / (cos(alpha) + 
	    sqrt(m * m - sin(alpha) * sin(alpha)));

/*<         r1=(rl*rl+rr*rr)/2. >*/
    r1 = (rl * rl + rr * rr) / 2.;
/*<        	r2=(rl*rl-rr*rr)/2. >*/
    r2 = (rl * rl - rr * rr) / 2.;
/*<  	r3=rl*rr >*/
    r3 = rl * rr;
/*<  	r3=0.0 >*/
    r3 = 0.;
/*       rpsur=sqrt(r2*r2+r3*r3) */
/* 	chi=atan2(r3,r2) */
/* adjust with agitated surface */
/*<       phw=azw*dtr >*/
    phw = *azw * dtr;
/*<       cs=cos(xts*dtr) >*/
    cs = cos(*xts * dtr);
/*<       cv=cos(xtv*dtr) >*/
    cv = cos(*xtv * dtr);
/*<       ss=sin(xts*dtr) >*/
    ss = sin(*xts * dtr);
/*<       sv=sin(xtv*dtr) >*/
    sv = sin(*xtv * dtr);
/*<       Zx=-sv*sin(phi*dtr)/(cs+cv) >*/
    zx = -sv * sin(*phi * dtr) / (cs + cv);
/*<       Zy=(ss+sv*cos(phi*dtr))/(cs+cv) >*/
    zy = (ss + sv * cos(*phi * dtr)) / (cs + cv);
/*<       tantilt=sqrt(zx*zx+zy*zy) >*/
    tantilt = sqrt(zx * zx + zy * zy);
/*<       tilt=atan(tantilt) >*/
    tilt = atan(tantilt);
/*  Anisotropic Gaussian distribution */
/*    phw=phi_sun-phi_wind */
/*<       sigmaC=0.003+0.00192*wspd >*/
    sigmac = *wspd * .00192 + .003;
/*<       sigmaU=0.00316*wspd >*/
    sigmau = *wspd * .00316;
/*<       C21=0.01-0.0086*wspd >*/
    c21 = .01 - *wspd * .0086;
/*<       C03=0.04-0.033*wspd >*/
    c03 = .04 - *wspd * .033;
/*<       C40=0.40 >*/
    c40 = .4;
/*<       C22=0.12 >*/
    c22 = .12;
/*<       C04=0.23 >*/
    c04 = .23;
/*<       xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC) >*/
    xe = (cos(phw) * zx + sin(phw) * zy) / sqrt(sigmac);
/*<       xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU) >*/
    xn = (-sin(phw) * zx + cos(phw) * zy) / sqrt(sigmau);
/*<       xe2=xe*xe >*/
    xe2 = xe * xe;
/*<       xn2=xn*xn >*/
    xn2 = xn * xn;
/*<       coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn >*/
    coef = 1 - c21 / 2. * (xe2 - 1) * xn - c03 / 6. * (xn2 - 3) * xn;
/*<       coef=coef+c40/24.*(xe2*xe2-6*xe2+3) >*/
    coef += c40 / 24. * (xe2 * xe2 - xe2 * 6 + 3);
/*<       coef=coef+C04/24.*(xn2*xn2-6*xn2+3) >*/
    coef += c04 / 24. * (xn2 * xn2 - xn2 * 6 + 3);
/*<       coef=coef+C22/4.*(xe2-1)*(xn2-1) >*/
    coef += c22 / 4. * (xe2 - 1) * (xn2 - 1);
/*<       proba=coef/2./pi/sqrt(sigmaU)/sqrt(sigmaC)*exp(-(xe2+xn2)/2.) >*/
    proba = coef / 2. / pi / sqrt(sigmau) / sqrt(sigmac) * exp(-(xe2 + xn2) / 
	    2.);
/*      write(6,*) "probapol:",proba */
/*      write(6,*) "coefpol:",coef */
/*      write(6,*) "tiltpol:",tilt */
/*      write(6,*) "phiw pol:",phw */
/* Compute Fresnel's coefficient R1 */
/*      cos2chi=cv*cs+sv*ss*cos(phi*dtr) */
/*      if (cos2chi.gt.1.0)cos2chi=0.99999999999 */
/*      if (cos2chi.lt.-1.0)cos2chi=-0.99999999999 */
/*      coschi=sqrt(0.5*(1+cos2chi)) */
/*      sinchi=sqrt(0.5*(1-cos2chi)) */
/*      Call Fresnel(nr,ni,coschi,sinchi,R1f) */
/*      Call pfresnel(nr,ni,coschi,sinchi,r1f,r2f) */
/*      write(6,*) "R1 fresnel:",R1f," r1 actual:",r1 */
/*      write(6,*) "R2 fresnel:",R2f," r2 actual:",r2 */
/* Compute Reflectance of the sun glint */
/*      Rog=pi*R1*proba/4./cs/cv/(cos(tilt)**4) */
/*<         factor=pi*proba/4./cs/cv/(cos(tilt)**4) >*/
/* Computing 4th power */
    d__1 = cos(tilt), d__1 *= d__1;
    factor = pi * proba / 4. / cs / cv / (d__1 * d__1);
/* compute rotation angle for Q and U */
/*<         muv=cos(xtv*dtr) >*/
    muv = cos(*xtv * dtr);
/*< 	mus=cos(xts*dtr) >*/
    mus = cos(*xts * dtr);
/*< 	sinv=sin(xtv*dtr) >*/
    sinv = sin(*xtv * dtr);
/*<         if (xtv.gt.0.5) then >*/
    if (*xtv > .5) {
/*<         if (sin(phi*dtr).lt.0) then >*/
	if (sin(*phi * dtr) < 0.) {
/*<         cksi=(muv*csca+mus)/sqrt(1.-csca*csca)/sinv >*/
	    cksi = (muv * csca + mus) / sqrt(1. - csca * csca) / sinv;
/*<         else >*/
	} else {
/*<         cksi=-(muv*csca+mus)/sqrt(1.-csca*csca)/sinv >*/
	    cksi = -(muv * csca + mus) / sqrt(1. - csca * csca) / sinv;
/*<         endif >*/
	}
/*<         else >*/
    } else {
/*<         cksi=1.0 >*/
	cksi = 1.;
/*<         endif >*/
    }
/*< 	if (cksi.gt.1.) cksi=1. >*/
    if (cksi > 1.) {
	cksi = 1.;
    }
/*< 	if (cksi.lt.-1.) cksi=-1. >*/
    if (cksi < -1.) {
	cksi = -1.;
    }
/*<         ksi=acos(cksi)/dtr >*/
    ksi = acos(cksi) / dtr;
/* 	write(6,*) "KSI=",ksi */
/* apply rotation mattrix */
/*< 	ropq=r2*(2.*cksi*cksi-1.)*factor >*/
    *ropq = r2 * (cksi * 2. * cksi - 1.) * factor;
/*< 	ropu=-r2*2.*cksi*sqrt(1.-cksi*cksi)*factor >*/
    *ropu = -r2 * 2. * cksi * sqrt(1. - cksi * cksi) * factor;
/*< 	return >*/
    return 0;
/*< 	end >*/
} /* polglit_ */

/*< 	Subroutine pfresnel(nr,ni,coschi,sinchi,r1,r2) >*/
/* Subroutine */ int pfresnel_(doublereal *nr, doublereal *ni, doublereal *
	coschi, doublereal *sinchi, doublereal *r1, doublereal *r2)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    doublereal u, v, a1, a2, b1, b2, rl2, rr2;


/* to compute the Fresnel's coefficient of reflection (see for */
/*example M. Born and E. Wolf, Principles of Optics, Pergamon Press, fifth
*/
/* edition, 1975, pp 628 */
/* input parameters: nr=index of refraction of the sea water */
/*                   ni=extinction coefficient of the sea water */
/*                  coschi & sinchi=cosine and sine of the incident radiat
ion*/
/*                                  with respect of the wave facet normal.
*/
/* output parameter: R1=Fresnel's coefficient for reflection */

/*<       real nr,ni,a1,a2,u,v,Rr2,Rl2,b1,b2,R1,coschi,sinchi >*/
/* absolute value for a1 to get v=0 when ni=0 */
/*<       a1=abs(nr*nr-ni*ni-sinchi*sinchi) >*/
    a1 = (d__1 = *nr * *nr - *ni * *ni - *sinchi * *sinchi, abs(d__1));
/*<       a2=sqrt((nr*nr-ni*ni-sinchi*sinchi)**2.+4*nr*nr*ni*ni) >*/
    d__1 = *nr * *nr - *ni * *ni - *sinchi * *sinchi;
    a2 = sqrt(pow_dd(&d__1, &c_b3) + *nr * 4 * *nr * *ni * *ni);
/*<       u=sqrt(0.5*(a1+a2)) >*/
    u = sqrt((a1 + a2) * .5);
/*<       v=sqrt(0.5*(-a1+a2)) >*/
    v = sqrt((-a1 + a2) * .5);
/*<       Rr2=((coschi-u)**2+v*v)/((coschi+u)**2+v*v) >*/
/* Computing 2nd power */
    d__1 = *coschi - u;
/* Computing 2nd power */
    d__2 = *coschi + u;
    rr2 = (d__1 * d__1 + v * v) / (d__2 * d__2 + v * v);
/*<       b1=(nr*nr-ni*ni)*coschi >*/
    b1 = (*nr * *nr - *ni * *ni) * *coschi;
/*<       b2=2*nr*ni*coschi >*/
    b2 = *nr * 2 * *ni * *coschi;
/*<       Rl2=((b1-u)**2+(b2+v)**2)/((b1+u)**2+(b2-v)**2) >*/
/* Computing 2nd power */
    d__1 = b1 - u;
/* Computing 2nd power */
    d__2 = b2 + v;
/* Computing 2nd power */
    d__3 = b1 + u;
/* Computing 2nd power */
    d__4 = b2 - v;
    rl2 = (d__1 * d__1 + d__2 * d__2) / (d__3 * d__3 + d__4 * d__4);
/*<       R1=(Rr2+Rl2)/2. >*/
    *r1 = (rr2 + rl2) / 2.;
/*<       R2=(Rl2-Rr2)/2. >*/
    *r2 = (rl2 - rr2) / 2.;
/*<       return >*/
    return 0;
/*<       end >*/
} /* pfresnel_ */

#ifdef __cplusplus
	}
#endif
