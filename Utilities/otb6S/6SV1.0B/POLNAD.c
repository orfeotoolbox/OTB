/* POLNAD.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<        Subroutine POLNAD(xts,xtv,phi,pveg,ropq,ropu) >*/
/* Subroutine */ int polnad_(doublereal *xts, doublereal *xtv, doublereal *
	phi, doublereal *pveg, doublereal *ropq, doublereal *ropu)
{
    /* Builtin functions */
    double acos(doublereal), cos(doublereal), sin(doublereal), asin(
	    doublereal), sqrt(doublereal);

    /* Local variables */
    doublereal n, pi, xf1, xf2, sca, ksi, dtr, mui, mus, mut, muv, csca, cksi,
	     sinv, alpha, rpveg, rpsur, alphap, rpsoil, fpalpha;

/*<        real xts,xtv,phi,pveg,ropq,ropu >*/
/*      compute the polarized components of the surface */
/*      according to the Nadal and Breon model */
/*   xts is the sun zenith angle in degrees */
/*   xtv is the view zenith angle in degrees */
/*   phi is the relative azimuth between sun and view in degrees */
/*   pveg is the "percentage of vegetation" for use in ponderating */
/*   the polarization of bare soil and vegetation */
/*   ropq and ropu and the stokes parameter (Q,U) of the polarized */
/*   surface reflectance */
/*<         real pi,dtr,csca,sca,alpha,alphap,N,mui,mut >*/
/*   csca is the cosine of the scattering angle (sca) */
/*   alpha is the incidence angle used for specular reflection */
/*   alphap is the refraction angle after specular reflection */
/*   N is the refractive index of vegetation (assumed 1.5) */
/*   intermediate variable to compute direction of polarization */
/*   according to fresnel law */
/*<         real mus,muv,sinv,cksi,ksi,xf1,xf2 >*/
/*<         N=1.5	 >*/
    n = 1.5;
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
/*< 	alphap=asin(sin(alpha)/N) >*/
    alphap = asin(sin(alpha) / n);
/*< 	mui=cos(alpha) >*/
    mui = cos(alpha);
/*< 	mut=cos(alphap) >*/
    mut = cos(alphap);
/*< 	xf1=(N*mut-mui)/(N*mut+mui) >*/
    xf1 = (n * mut - mui) / (n * mut + mui);
/*< 	xf2=(N*mui-mut)/(N*mui+mut) >*/
    xf2 = (n * mui - mut) / (n * mui + mut);
/*< 	fpalpha=0.5*(xf1*xf1-xf2*xf2) >*/
    fpalpha = (xf1 * xf1 - xf2 * xf2) * .5;
/*<         rpveg=fpalpha/4./(cos(xts*dtr)+cos(xtv*dtr)) >*/
    rpveg = fpalpha / 4. / (cos(*xts * dtr) + cos(*xtv * dtr));
/*< 	rpsoil=fpalpha/4./cos(xts*dtr)/cos(xtv*dtr) >*/
    rpsoil = fpalpha / 4. / cos(*xts * dtr) / cos(*xtv * dtr);
/*< 	rpsur=rpveg*pveg+rpsoil*(1.-pveg) >*/
    rpsur = rpveg * *pveg + rpsoil * (1. - *pveg);
/* compute rotation angle for Q and U */
/*<         muv=cos(xtv*dtr) >*/
    muv = cos(*xtv * dtr);
/*< 	mus=cos(xts*dtr) >*/
    mus = cos(*xts * dtr);
/*< 	sinv=sin(xtv*dtr) >*/
    sinv = sin(*xtv * dtr);
/*< 	if (xtv.gt.0.5) then >*/
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
/*<         cksi=0.0 >*/
	cksi = 0.;
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
/*< 	ropq=rpsur*(2.*cksi*cksi-1.) >*/
    *ropq = rpsur * (cksi * 2. * cksi - 1.);
/*< 	ropu=-rpsur*2.*cksi*sqrt(1.-cksi*cksi) >*/
    *ropu = -rpsur * 2. * cksi * sqrt(1. - cksi * cksi);
/*< 	return >*/
    return 0;
/*< 	end >*/
} /* polnad_ */

#ifdef __cplusplus
	}
#endif
