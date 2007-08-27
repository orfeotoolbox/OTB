/* POSNOA.f -- translated by f2c (version 19970805).
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
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/*<    >*/
/* Subroutine */ int posnoa_(integer *month, integer *jday, real *tu, integer 
	*nc, real *xlonan, real *hna, real *campm, real *asol, real *phi0, 
	real *avis, real *phiv, real *xlon, real *xlat)
{
    /* Builtin functions */
    double sin(doublereal), asin(doublereal), cos(doublereal), atan2(
	    doublereal, doublereal);

    /* Local variables */
    real d__, r__, t, u, y, z__, ai, an, pi, ylo1, hnam, delt, xden, ylat, 
	    zlat, cosy, ylon, siny, zlon, xnum, ylonan;
    extern /* Subroutine */ int possol_(integer *, integer *, real *, real *, 
	    real *, real *, real *);

/*<       logical ier >*/
/*<       real tu,xlonan,hna,asol,phi0,avis,phiv,xlon,xlat,campm >*/
/*<       real pi,r,ai,an,ylonan,t,hnam,u,delt,d,y,z >*/
/*<       real ylat,cosy,siny,ylon,ylo1,zlat,zlon,xnum,xden >*/
/*<       integer month,jday,nc,iwr >*/
/*<       common/sixs_ier/iwr,ier >*/
/*     noaa 6 definition */
/*     orbite inclination ai in radians */
/*     hor mouv in rad/s  an */
/*     h/r=860/6378 */
/*     campm allows the user to switch to pm platforms */
/*<       pi=3.1415926 >*/
    pi = (float)3.1415926;
/*<       r=860./6378.155 >*/
    r__ = (float).13483523056432464;
/*<       ai=98.96*pi/180. >*/
    ai = pi * (float)98.96 / (float)180.;
/*<       an=360.*pi/(6119.*180.) >*/
    an = pi * (float)360. / (float)1101420.;
/*<       ylonan=xlonan*pi/180. >*/
    ylonan = *xlonan * pi / (float)180.;
/*<       t=tu*3600 >*/
    t = *tu * 3600;
/*<       hnam=hna >*/
    hnam = *hna;
/*<       hnam=hnam*3600 >*/
    hnam *= 3600;
/*<       u=t-hnam >*/
    u = t - hnam;
/*<       u=campm*u*an >*/
    u = *campm * u * an;
/*<       delt=((nc-(2048+1)/2.)*55.385/((2048.-1)/2.)) >*/
    delt = (*nc - (float)1024.5) * (float)55.385 / (float)1023.5;
/*<       delt=campm*delt*pi/180. >*/
    delt = *campm * delt * pi / (float)180.;
/*<       avis=asin((1+r)*sin(delt)) >*/
    *avis = asin((r__ + 1) * sin(delt));
/*<       d=avis-delt >*/
    d__ = *avis - delt;
/*<       y=cos(d)*cos(ai)*sin(u)-sin(ai)*sin(d) >*/
    y = cos(d__) * cos(ai) * sin(u) - sin(ai) * sin(d__);
/*<       z=cos(d)*sin(ai)*sin(u)+cos(ai)*sin(d) >*/
    z__ = cos(d__) * sin(ai) * sin(u) + cos(ai) * sin(d__);
/*<       ylat=asin(z) >*/
    ylat = asin(z__);
/*<       cosy=cos(d)*cos(u)/cos(ylat) >*/
    cosy = cos(d__) * cos(u) / cos(ylat);
/*<       siny=y/cos(ylat) >*/
    siny = y / cos(ylat);
/*<       ylon=asin(siny) >*/
    ylon = asin(siny);
/*<       if(cosy.gt.0.)goto 10 >*/
    if (cosy > (float)0.) {
	goto L10;
    }
/*<       if(siny.gt.0) ylon=pi-ylon >*/
    if (siny > (float)0.) {
	ylon = pi - ylon;
    }
/*<       if(siny.le.0) ylon=-(pi+ylon) >*/
    if (siny <= (float)0.) {
	ylon = -(pi + ylon);
    }
/*<  10   ylo1=ylon+ylonan-(t-hnam)*2.*pi/86400. >*/
L10:
    ylo1 = ylon + ylonan - (t - hnam) * (float)2. * pi / (float)86400.;
/*<       xlat=ylat*180./pi >*/
    *xlat = ylat * (float)180. / pi;
/*<       xlon=ylo1*180./pi >*/
    *xlon = ylo1 * (float)180. / pi;
/*<    >*/
    possol_(month, jday, tu, xlon, xlat, asol, phi0);
/*<       if(ier)return >*/
    if (sixs_ier__1.ier) {
	return 0;
    }
/*<       zlat=asin(sin(ai)*sin(u)) >*/
    zlat = asin(sin(ai) * sin(u));
/*<       zlon=atan2(cos(ai)*sin(u),cos(u)) >*/
    zlon = atan2(cos(ai) * sin(u), cos(u));
/*<       if(nc.ne.1024)then >*/
    if (*nc != 1024) {
/*<       xnum=sin(zlon-ylon)*cos(zlat)/sin(abs(d)) >*/
	xnum = sin(zlon - ylon) * cos(zlat) / sin((dabs(d__)));
/*<       xden=(sin(zlat)-sin(ylat)*cos(d))/cos(ylat)/sin(abs(d)) >*/
	xden = (sin(zlat) - sin(ylat) * cos(d__)) / cos(ylat) / sin((dabs(d__)
		));
/*<       phiv=atan2(xnum,xden) >*/
	*phiv = atan2(xnum, xden);
/*<       else >*/
    } else {
/*<       phiv=0. >*/
	*phiv = (float)0.;
/*<       endif >*/
    }
/*<       phiv=phiv*180./pi >*/
    *phiv = *phiv * (float)180. / pi;
/*<       avis=abs(avis) >*/
    *avis = dabs(*avis);
/*<       avis=avis*180./pi >*/
    *avis = *avis * (float)180. / pi;
/*<       return >*/
    return 0;
/*<       end >*/
} /* posnoa_ */

#ifdef __cplusplus
	}
#endif
