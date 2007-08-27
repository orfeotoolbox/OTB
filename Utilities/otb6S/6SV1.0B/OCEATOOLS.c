/* OCEATOOLS.f -- translated by f2c (version 19970805).
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

static doublereal c_b3 = .62;
static doublereal c_b10 = 2.;

/*<       subroutine morcasiwat(wl,C,R2) >*/
/* Subroutine */ int morcasiwat_(real *wl, real *c__, real *r2)
{
    /* Initialized data */

    static real tkw[61] = { (float).0209,(float).02,(float).0196,(float).0189,
	    (float).0183,(float).0182,(float).0171,(float).017,(float).0168,(
	    float).0166,(float).0168,(float).017,(float).0173,(float).0174,(
	    float).0175,(float).0184,(float).0194,(float).0203,(float).0217,(
	    float).024,(float).0271,(float).032,(float).0384,(float).0445,(
	    float).049,(float).0505,(float).0518,(float).0543,(float).0568,(
	    float).0615,(float).064,(float).064,(float).0717,(float).0762,(
	    float).0807,(float).094,(float).107,(float).128,(float).157,(
	    float).2,(float).253,(float).279,(float).296,(float).303,(float)
	    .31,(float).315,(float).32,(float).325,(float).33,(float).34,(
	    float).35,(float).37,(float).405,(float).418,(float).43,(float)
	    .44,(float).45,(float).47,(float).5,(float).55,(float).65 };
    static real txc[61] = { (float).11,(float).111,(float).1125,(float).1135,(
	    float).1126,(float).1104,(float).1078,(float).1065,(float).1041,(
	    float).0996,(float).0971,(float).0939,(float).0896,(float).0859,(
	    float).0823,(float).0788,(float).0746,(float).0726,(float).069,(
	    float).066,(float).0636,(float).06,(float).0578,(float).054,(
	    float).0498,(float).0475,(float).0467,(float).045,(float).044,(
	    float).0426,(float).041,(float).04,(float).039,(float).0375,(
	    float).036,(float).034,(float).033,(float).0328,(float).0325,(
	    float).033,(float).034,(float).035,(float).036,(float).0375,(
	    float).0385,(float).04,(float).042,(float).043,(float).044,(float)
	    .0445,(float).045,(float).046,(float).0475,(float).049,(float)
	    .0515,(float).052,(float).0505,(float).044,(float).039,(float)
	    .034,(float).03 };
    static real te[61] = { (float).668,(float).672,(float).68,(float).687,(
	    float).693,(float).701,(float).707,(float).708,(float).707,(float)
	    .704,(float).701,(float).699,(float).7,(float).703,(float).703,(
	    float).703,(float).703,(float).704,(float).702,(float).7,(float)
	    .7,(float).695,(float).69,(float).685,(float).68,(float).675,(
	    float).67,(float).665,(float).66,(float).655,(float).65,(float)
	    .645,(float).64,(float).63,(float).623,(float).615,(float).61,(
	    float).614,(float).618,(float).622,(float).626,(float).63,(float)
	    .634,(float).638,(float).642,(float).647,(float).653,(float).658,(
	    float).663,(float).667,(float).672,(float).677,(float).682,(float)
	    .687,(float).695,(float).697,(float).693,(float).665,(float).64,(
	    float).62,(float).6 };
    static real tbw[61] = { (float).0076,(float).0072,(float).0068,(float)
	    .0064,(float).0061,(float).0058,(float).0055,(float).0052,(float)
	    .0049,(float).0047,(float).0045,(float).0043,(float).0041,(float)
	    .0039,(float).0037,(float).0036,(float).0034,(float).0033,(float)
	    .0031,(float).003,(float).0029,(float).0027,(float).0026,(float)
	    .0025,(float).0024,(float).0023,(float).0022,(float).0022,(float)
	    .0021,(float).002,(float).0019,(float).0018,(float).0018,(float)
	    .0017,(float).0017,(float).0016,(float).0016,(float).0015,(float)
	    .0015,(float).0014,(float).0014,(float).0013,(float).0013,(float)
	    .0012,(float).0012,(float).0011,(float).0011,(float).001,(float)
	    .001,(float).001,(float).001,(float)9e-4,(float)8e-4,(float)8e-4,(
	    float)8e-4,(float)7e-4,(float)7e-4,(float)7e-4,(float)7e-4,(float)
	    7e-4,(float)7e-4 };

    /* System generated locals */
    real r__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer i_nint(real *);
    double pow_dd(doublereal *, doublereal *), r_lg10(real *);

    /* Local variables */
    real b, e, r1, u1, u2, bb, kd, bw, xc, kw, bbt, err;
    integer iwl;

/* Spectral diffuse attenuation coefficient of Case I Waters as Predicted 
*/
/*by MOREL within the spectral range 400-700nm (1988, Journal of Geophysic
al*/
/* Research, Vol.93, No C9, pp 10749-10768) */

/* input parameters:	wl wavelength (IN MICROMETERS) */
/* 			C  pigment concentration */
/* output parameter:	R2  reflectance of water */

/* According Morel,1988, we use: */

/* Kd	spectral value of the attenuation coefficient for */
/* 	 downwelling irradiance */
/* 	 with: Kd=Kw+Xc*C**e */
/* Kw	spectral value of the diffuse attenuation coefficient */
/* 	 for pure oceanic water */
/* Xc, e	spectral coefficients to compute the diffuse attenuation */
/* 	 coefficient for pigment */
/* bb	total backscattering coefficient */
/* 	 with: bb=0.5*bw+bbt*b */
/* bw	spectral value of the molecular scattering coefficient of water */
/* bbt,b	parameters to compute the scattering coefficients of pigments */

/* R2	reflectance of water below the surface */
/* 	 with: R2=(0.33/u)*(bb/Kd)	where u is depending of R2 */

/*<       real Kw,Kd >*/
/*<       real tKw(61),tXc(61),te(61),tbw(61) >*/
/*<       real wl,c,r2,xc,e,bw,bb,b,bbt,u1,r1,u2,err >*/
/*<       integer iwl >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       if (wl.lt.0.400.or.wl.gt.0.700)then >*/
    if (*wl < (float).4 || *wl > (float).7) {
/*< 	R2=0.000 >*/
	*r2 = (float)0.;
/*< 	goto 60 >*/
	goto L60;
/*<       endif >*/
    }
/*<       iwl=1+nint((wl-0.400)/0.005) >*/
    r__1 = (*wl - (float).4) / (float).005;
    iwl = i_nint(&r__1) + 1;
/*<       Kw=tKw(iwl) >*/
    kw = tkw[iwl - 1];
/*<       Xc=tXc(iwl) >*/
    xc = txc[iwl - 1];
/*<       e=te(iwl) >*/
    e = te[iwl - 1];
/*<       bw=tbw(iwl) >*/
    bw = tbw[iwl - 1];

/*<       if (abs(C).lt.0.0001)then >*/
    if (dabs(*c__) < (float)1e-4) {
/*<          bb=0.5*bw >*/
	bb = bw * (float).5;
/*<          Kd=Kw >*/
	kd = kw;
/*<       else >*/
    } else {
/*<          b=0.30*C**0.62 >*/
	d__1 = (doublereal) (*c__);
	b = pow_dd(&d__1, &c_b3) * (float).3;
/*<          bbt=0.002+0.02*(0.5-0.25*alog10(C))*0.550/wl >*/
	bbt = ((float).5 - r_lg10(c__) * (float).25) * (float).02 * (float)
		.55 / *wl + (float).002;
/*<          bb=0.5*bw+bbt*b >*/
	bb = bw * (float).5 + bbt * b;
/*<          Kd=Kw+Xc*C**e >*/
	d__1 = (doublereal) (*c__);
	d__2 = (doublereal) e;
	kd = kw + xc * pow_dd(&d__1, &d__2);
/*<       endif >*/
    }
/*<       u1=0.75 >*/
    u1 = (float).75;
/*<       R1=0.33*bb/u1/Kd >*/
    r1 = bb * (float).33 / u1 / kd;
/*<  50   u2=0.90*(1.-R1)/(1.+2.25*R1) >*/
L50:
    u2 = ((float)1. - r1) * (float).9 / (r1 * (float)2.25 + (float)1.);
/*<       R2=0.33*bb/u2/Kd >*/
    *r2 = bb * (float).33 / u2 / kd;
/*<       err=abs((R2-R1)/R2) >*/
    err = (r__1 = (*r2 - r1) / *r2, dabs(r__1));
/*<       if (err.lt.0.0001)goto 60 >*/
    if (err < (float)1e-4) {
	goto L60;
    }
/*<       R1=R2 >*/
    r1 = *r2;
/*<       goto 50 >*/
    goto L50;
/*<  60   return >*/
L60:
    return 0;
/*<       end >*/
} /* morcasiwat_ */


/*<        subroutine indwat(wl,xsal,nr,ni) >*/
/* Subroutine */ int indwat_(real *wl, real *xsal, real *nr, real *ni)
{
    /* Initialized data */

    static real twl[62] = { (float).25,(float).275,(float).3,(float).325,(
	    float).345,(float).375,(float).4,(float).425,(float).445,(float)
	    .475,(float).5,(float).525,(float).55,(float).575,(float).6,(
	    float).625,(float).65,(float).675,(float).7,(float).725,(float)
	    .75,(float).775,(float).8,(float).825,(float).85,(float).875,(
	    float).9,(float).925,(float).95,(float).975,(float)1.,(float)1.2,(
	    float)1.4,(float)1.6,(float)1.8,(float)2.,(float)2.2,(float)2.4,(
	    float)2.6,(float)2.65,(float)2.7,(float)2.75,(float)2.8,(float)
	    2.85,(float)2.9,(float)2.95,(float)3.,(float)3.05,(float)3.1,(
	    float)3.15,(float)3.2,(float)3.25,(float)3.3,(float)3.35,(float)
	    3.4,(float)3.45,(float)3.5,(float)3.6,(float)3.7,(float)3.8,(
	    float)3.9,(float)4. };
    static real tnr[62] = { (float)1.362,(float)1.354,(float)1.349,(float)
	    1.346,(float)1.343,(float)1.341,(float)1.339,(float)1.338,(float)
	    1.337,(float)1.336,(float)1.335,(float)1.334,(float)1.333,(float)
	    1.333,(float)1.332,(float)1.332,(float)1.331,(float)1.331,(float)
	    1.331,(float)1.33,(float)1.33,(float)1.33,(float)1.329,(float)
	    1.329,(float)1.329,(float)1.328,(float)1.328,(float)1.328,(float)
	    1.327,(float)1.327,(float)1.327,(float)1.324,(float)1.321,(float)
	    1.317,(float)1.312,(float)1.306,(float)1.296,(float)1.279,(float)
	    1.242,(float)1.219,(float)1.188,(float)1.157,(float)1.142,(float)
	    1.149,(float)1.201,(float)1.292,(float)1.371,(float)1.426,(float)
	    1.467,(float)1.483,(float)1.478,(float)1.467,(float)1.45,(float)
	    1.432,(float)1.42,(float)1.41,(float)1.4,(float)1.385,(float)
	    1.374,(float)1.364,(float)1.357,(float)1.351 };
    static real tni[62] = { (float)3.35e-8,(float)2.35e-8,(float)1.6e-8,(
	    float)1.08e-8,(float)6.5e-9,(float)3.5e-9,(float)1.86e-9,(float)
	    1.3e-9,(float)1.02e-9,(float)9.35e-10,(float)1e-9,(float)1.32e-9,(
	    float)1.96e-9,(float)3.6e-9,(float)1.09e-8,(float)1.39e-8,(float)
	    1.64e-8,(float)2.23e-8,(float)3.35e-8,(float)9.15e-8,(float)
	    1.56e-7,(float)1.48e-7,(float)1.25e-7,(float)1.82e-7,(float)
	    2.93e-7,(float)3.91e-7,(float)4.86e-7,(float)1.06e-6,(float)
	    2.93e-6,(float)3.48e-6,(float)2.89e-6,(float)9.89e-6,(float)
	    1.38e-4,(float)8.55e-5,(float)1.15e-4,(float).0011,(float)2.89e-4,
	    (float)9.56e-4,(float).00317,(float).0067,(float).019,(float).059,
	    (float).115,(float).185,(float).268,(float).298,(float).272,(
	    float).24,(float).192,(float).135,(float).0924,(float).061,(float)
	    .0368,(float).0261,(float).0195,(float).0132,(float).0094,(float)
	    .00515,(float).0036,(float).0034,(float).0038,(float).0046 };

    integer i__;
    real yi, yr, nic, nrc, xwl;


/* input parameters:  wl=wavelength (in micrometers) */
/*                   xsal=salinity (in ppt), if xsal<0 then 34.3ppt by def
ault*/
/* output parameters: nr=index of refraction of sea water */
/*                    ni=extinction coefficient of sea water */

/*<        real twl(62),tnr(62),tni(62) >*/
/*<        real nr,ni,wl,xwl,yr,yi,nrc,nic,xsal >*/
/*<        integer i >*/
/* Indices of refraction for pure water from Hale and Querry, */
/* Applied Optique, March 1973, Vol. 12,  No. 3, pp. 555-563 */
/*<    >*/
/*<    >*/
/*<    >*/
/*<         i=2 >*/
    i__ = 2;
/*<  10     if (wl.lt.twl(i)) goto 20 >*/
L10:
    if (*wl < twl[i__ - 1]) {
	goto L20;
    }
/*<         if (i.lt.62) then >*/
    if (i__ < 62) {
/*<            i=i+1 >*/
	++i__;
/*<            goto 10 >*/
	goto L10;
/*<            endif >*/
    }
/*<  20     xwl=twl(i)-twl(i-1)         >*/
L20:
    xwl = twl[i__ - 1] - twl[i__ - 2];
/*<         yr=tnr(i)-tnr(i-1)         >*/
    yr = tnr[i__ - 1] - tnr[i__ - 2];
/*<         yi=tni(i)-tni(i-1)         >*/
    yi = tni[i__ - 1] - tni[i__ - 2];
/*<         nr=tnr(i-1)+(wl-twl(i-1))*yr/xwl >*/
    *nr = tnr[i__ - 2] + (*wl - twl[i__ - 2]) * yr / xwl;
/*<         ni=tni(i-1)+(wl-twl(i-1))*yi/xwl >*/
    *ni = tni[i__ - 2] + (*wl - twl[i__ - 2]) * yi / xwl;

/*Correction to be applied to the index of refraction and to the extinctio
n*/
/* coefficients of the pure water to obtain the ocean water one (see for 
*/
/* example Friedman). By default, a typical sea water is assumed */
/* (Salinity=34.3ppt, Chlorinity=19ppt) as reported by Sverdrup. */
/*In that case there is no correction for the extinction coefficient betwe
en*/
/* 0.25 and 4 microns. For the index of refraction, a correction of +0.006
 */
/*has to be applied (McLellan). For a chlorinity of 19.0ppt the correction
*/
/*is a linear function of the salt concentration. Then, in 6S users are ab
le*/
/* to enter the salt concentration (in ppt). */
/* REFERENCES: */
/* Friedman D., Applied Optics, 1969, Vol.8, No.10, pp.2073-2078. */
/* McLellan H.J., Elements of physical Oceanography, Pergamon Press, Inc.,
 */
/*        New-York, 1965, p 129. */
/*Sverdrup H.V. et al., The Oceans (Prentice-Hall, Inc., Englewood Cliffs,
*/
/*        N.J., 1942, p 173. */
/*<         nrc=0.006 >*/
    nrc = (float).006;
/*<         nic=0.000 >*/
    nic = (float)0.;
/*<         nr=nr+nrc*(xsal/34.3) >*/
    *nr += nrc * (*xsal / (float)34.3);
/*< 	ni=ni+nic*(xsal/34.3) >*/
    *ni += nic * (*xsal / (float)34.3);
/*<         return >*/
    return 0;
/*<         end >*/
} /* indwat_ */


/*<       subroutine sunglint(wspd,nr,ni,azw,ts,tv,fi,rog) >*/
/* Subroutine */ int sunglint_(real *wspd, real *nr, real *ni, real *azw, 
	real *ts, real *tv, real *fi, real *rog)
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal), sqrt(
	    doublereal), exp(doublereal);

    /* Local variables */
    real r1, c21, c03, c40, c04, c22, cs, pi, cv, xe, ss, xn, sv, zx, zy, xe2,
	     xn2, fac, phi, phw, coef, tilt, proba, sigmac, coschi, sinchi, 
	    sigmau, cos2chi;
    extern /* Subroutine */ int fresnel_(real *, real *, real *, real *, real 
	    *);
    real tantilt;

/* input parameters:   wspd=speed of the wind (in m/s) */
/*                     nr=index of refraction of the sea water */
/*                     ni=extinction coefficient of the sea water */
/*                     azw=azim. of the sun - azim. of the wind (in deg.) 
*/
/*                     ts=solar zenith angle (in deg.) */
/*                     tv=view zenith angle (in deg.) */
/*                     fi=relative azimuth (sun-satellite) */
/* output parameters:  rog=reflectance of the sun glint */

/*<       real pi,fac >*/
/*<       real wspd,nr,ni,ts,tv,fi,rog,azw,phw >*/
/*<       real cs,cv,ss,sv,phi,zx,zy,tantilt,tilt,proba,xe,xn,xe2,xn2 >*/
/*<       real coef,cos2chi,coschi,sinchi >*/
/*<       real r1,sigmaC,sigmaU,C21,C03,C40,C04,C22 >*/
/*<       pi=atan(1.)*4. >*/
    pi = atan((float)1.) * (float)4.;
/*<       fac=pi/180. >*/
    fac = pi / (float)180.;
/*<       phw=azw*fac >*/
    phw = *azw * fac;
/*<       cs=cos(ts*fac) >*/
    cs = cos(*ts * fac);
/*<       cv=cos(tv*fac) >*/
    cv = cos(*tv * fac);
/*<       ss=sin(ts*fac) >*/
    ss = sin(*ts * fac);
/*<       sv=sin(tv*fac) >*/
    sv = sin(*tv * fac);
/*<       phi=fi*fac >*/
    phi = *fi * fac;
/*<       Zx=-sv*sin(phi)/(cs+cv) >*/
    zx = -sv * sin(phi) / (cs + cv);
/*<       Zy=(ss+sv*cos(phi))/(cs+cv) >*/
    zy = (ss + sv * cos(phi)) / (cs + cv);
/*<       tantilt=sqrt(zx*zx+zy*zy) >*/
    tantilt = sqrt(zx * zx + zy * zy);
/*<       tilt=atan(tantilt) >*/
    tilt = atan(tantilt);
/*      write(6,*) "tantilt ",tantilt */
/*      write(6,*) "tilt " ,tilt */
/*  Anisotropic Gaussian distribution */
/*    phw=phi_sun-phi_wind */
/*<       sigmaC=0.003+0.00192*wspd >*/
    sigmac = *wspd * (float).00192 + (float).003;
/*<       sigmaU=0.00316*wspd >*/
    sigmau = *wspd * (float).00316;
/*<       C21=0.01-0.0086*wspd >*/
    c21 = (float).01 - *wspd * (float).0086;
/*<       C03=0.04-0.033*wspd >*/
    c03 = (float).04 - *wspd * (float).033;
/*<       C40=0.40 >*/
    c40 = (float).4;
/*<       C22=0.12 >*/
    c22 = (float).12;
/*<       C04=0.23 >*/
    c04 = (float).23;
/*<       xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC) >*/
    xe = (cos(phw) * zx + sin(phw) * zy) / sqrt(sigmac);
/*<       xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU) >*/
    xn = (-sin(phw) * zx + cos(phw) * zy) / sqrt(sigmau);
/*<       xe2=xe*xe >*/
    xe2 = xe * xe;
/*<       xn2=xn*xn >*/
    xn2 = xn * xn;
/*<       coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn >*/
    coef = 1 - c21 / (float)2. * (xe2 - 1) * xn - c03 / (float)6. * (xn2 - 3) 
	    * xn;
/*<       coef=coef+c40/24.*(xe2*xe2-6*xe2+3) >*/
    coef += c40 / (float)24. * (xe2 * xe2 - xe2 * 6 + 3);
/*<       coef=coef+C04/24.*(xn2*xn2-6*xn2+3) >*/
    coef += c04 / (float)24. * (xn2 * xn2 - xn2 * 6 + 3);
/*<       coef=coef+C22/4.*(xe2-1)*(xn2-1) >*/
    coef += c22 / (float)4. * (xe2 - 1) * (xn2 - 1);
/*<       proba=coef/2./pi/sqrt(sigmaU)/sqrt(sigmaC)*exp(-(xe2+xn2)/2.) >*/
    proba = coef / (float)2. / pi / sqrt(sigmau) / sqrt(sigmac) * exp(-(xe2 + 
	    xn2) / (float)2.);
/*      write(6,*) "probaglit:",proba */
/*      write(6,*) "coef glit:",coef */
/*      write(6,*) "tilt glit:",tilt */
/*      write(6,*) "phw glit:",phw */
/* Compute Fresnel's coefficient R1 */
/*<       cos2chi=cv*cs+sv*ss*cos(phi) >*/
    cos2chi = cv * cs + sv * ss * cos(phi);
/*<       if (cos2chi.gt.1.0)cos2chi=0.99999999999 >*/
    if (cos2chi > (float)1.) {
	cos2chi = (float).99999999999;
    }
/*<       if (cos2chi.lt.-1.0)cos2chi=-0.99999999999 >*/
    if (cos2chi < (float)-1.) {
	cos2chi = (float)-.99999999999;
    }
/*<       coschi=sqrt(0.5*(1+cos2chi)) >*/
    coschi = sqrt((cos2chi + 1) * (float).5);
/*<       sinchi=sqrt(0.5*(1-cos2chi)) >*/
    sinchi = sqrt((1 - cos2chi) * (float).5);
/*<       if (coschi.ge.1.0)coschi=0.99999999 >*/
    if (coschi >= (float)1.) {
	coschi = (float).99999999;
    }
/*<       if (coschi.le.-1.0)coschi=-0.999999 >*/
    if (coschi <= (float)-1.) {
	coschi = (float)-.999999;
    }
/*<        if (sinchi.gt.1.0)sinchi=0.9999999 >*/
    if (sinchi > (float)1.) {
	sinchi = (float).9999999;
    }
/*<       if (sinchi.lt.-1.0)sinchi=-0.999999 >*/
    if (sinchi < (float)-1.) {
	sinchi = (float)-.999999;
    }
/*<       Call Fresnel(nr,ni,coschi,sinchi,R1) >*/
    fresnel_(nr, ni, &coschi, &sinchi, &r1);
/* Compute Reflectance of the sun glint */
/*<       Rog=pi*R1*proba/4./cs/cv/(cos(tilt)**4) >*/
/* Computing 4th power */
    r__1 = cos(tilt), r__1 *= r__1;
    *rog = pi * r1 * proba / (float)4. / cs / cv / (r__1 * r__1);
/*      write(6,*) "ROg ",Rog,R1,proba */
/*<       return >*/
    return 0;
/*<       end >*/
} /* sunglint_ */



/*<       Subroutine Fresnel(nr,ni,coschi,sinchi,R1) >*/
/* Subroutine */ int fresnel_(real *nr, real *ni, real *coschi, real *sinchi, 
	real *r1)
{
    /* System generated locals */
    real r__1, r__2, r__3, r__4;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    real u, v, a1, a2, b1, b2, rl2, rr2;


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
    a1 = (r__1 = *nr * *nr - *ni * *ni - *sinchi * *sinchi, dabs(r__1));
/*<       a2=sqrt((nr*nr-ni*ni-sinchi*sinchi)**2.+4*nr*nr*ni*ni) >*/
    d__1 = (doublereal) (*nr * *nr - *ni * *ni - *sinchi * *sinchi);
    a2 = sqrt(pow_dd(&d__1, &c_b10) + *nr * 4 * *nr * *ni * *ni);
/*<       u=sqrt(0.5*abs(a1+a2)) >*/
    u = sqrt((r__1 = a1 + a2, dabs(r__1)) * (float).5);
/*<       v=sqrt(0.5*abs(-a1+a2)) >*/
    v = sqrt((r__1 = -a1 + a2, dabs(r__1)) * (float).5);
/*<       Rr2=((coschi-u)**2+v*v)/((coschi+u)**2+v*v) >*/
/* Computing 2nd power */
    r__1 = *coschi - u;
/* Computing 2nd power */
    r__2 = *coschi + u;
    rr2 = (r__1 * r__1 + v * v) / (r__2 * r__2 + v * v);
/*<       b1=(nr*nr-ni*ni)*coschi >*/
    b1 = (*nr * *nr - *ni * *ni) * *coschi;
/*<       b2=2*nr*ni*coschi >*/
    b2 = *nr * 2 * *ni * *coschi;
/*<       Rl2=((b1-u)**2+(b2+v)**2)/((b1+u)**2+(b2-v)**2) >*/
/* Computing 2nd power */
    r__1 = b1 - u;
/* Computing 2nd power */
    r__2 = b2 + v;
/* Computing 2nd power */
    r__3 = b1 + u;
/* Computing 2nd power */
    r__4 = b2 - v;
    rl2 = (r__1 * r__1 + r__2 * r__2) / (r__3 * r__3 + r__4 * r__4);
/*<       R1=(Rr2+Rl2)/2. >*/
    *r1 = (rr2 + rl2) / (float)2.;
/*      write(6,*) "fresnel ", R1,u,v,a1,a2 */
/*<       return >*/
    return 0;
/*<       end >*/
} /* fresnel_ */



/*<       subroutine glitalbe(wspd,nr,ni,azw,rge) >*/
/* Subroutine */ int glitalbe_(real *wspd, real *nr, real *ni, real *azw, 
	real *rge)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    real r__1;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal), acos(doublereal), sin(
	    doublereal), cos(doublereal), exp(doublereal);

    /* Local variables */
    integer i__, j;
    real q, r1, c21, c03, c40, c04, c22;
    integer km;
    real pi, xe, pp, pr, xn, zx, zy, xe2, xn2, fac, hfa;
    integer nfa;
    real hta, htb;
    integer nta, ntb;
    real tet, phw, cofa, diff, coef, cota, cotb, phin, pond, fonc0, sigma, 
	    proba, prefl, costt, sigmac, coschi, sinchi, sigmau, costet, 
	    sintet;
    extern /* Subroutine */ int fresnel_(real *, real *, real *, real *, real 
	    *);
    real cosphin, sinphin, tantetn, costetn, sintetn;


/* To compute the spherical albedo of the sea water. See for example */
/* Masuda et al., Remote Sens. Environ., 24, 313-329, 1988. */

/* input parameters: wsp=wind of speed */
/*                   nr=index of refraction of the sea water */
/*                   ni=extinction coefficient of the sea water */
/*                   azw=azim. of sun - azim. of wind (in deg.) */
/* output parameter: rge=spherical albedo of the sun glint */

/*<       real nr,ni,azw,phw,rge,q,wspd,prefl,proba,pr,pp,pi,fac >*/
/*<       real sigma,sigmaC,sigmaU,C21,C03,C40,C04,C22 >*/
/*<       real costt,hta,htb,hfa,cotb,cota,cofa,diff,coef >*/
/*<       real phin,cosphin,sinphin,costet,tet,sintet >*/
/*<       real costetn,sintetn,tantetn,coschi,sinchi >*/
/*<       real zx,zy,xe,xn,xe2,xn2,fonc0,pond,r1 >*/
/*<       integer nta,nfa,ntb,km,i,j >*/
/*<       pi=atan(1.)*4. >*/
    pi = atan((float)1.) * (float)4.;
/*<       fac=pi/180. >*/
    fac = pi / (float)180.;
/*<       sigma=0.003+0.00512*wspd >*/
    sigma = *wspd * (float).00512 + (float).003;
/*<       sigmaC=0.003+0.00192*wspd >*/
    sigmac = *wspd * (float).00192 + (float).003;
/*<       sigmaU=0.00316*wspd >*/
    sigmau = *wspd * (float).00316;
/*<       C21=0.01-0.0086*wspd >*/
    c21 = (float).01 - *wspd * (float).0086;
/*<       C03=0.04-0.033*wspd >*/
    c03 = (float).04 - *wspd * (float).033;
/*<       C40=0.40 >*/
    c40 = (float).4;
/*<       C22=0.12 >*/
    c22 = (float).12;
/*<       C04=0.23 >*/
    c04 = (float).23;
/* costt to minimize the time of the computation */
/*     integration between 1 and costt instead of 1 and 0 */
/*<       q=50 >*/
    q = (float)50.;
/*<       costt=1./sqrt(1+q*sigma/4.) >*/
    costt = (float)1. / sqrt(q * sigma / (float)4. + 1);
/*<       phw=azw*fac >*/
    phw = *azw * fac;
/*<       prefl=0. >*/
    prefl = (float)0.;
/*<       proba=0. >*/
    proba = (float)0.;
/*<       ntb=31 >*/
    ntb = 31;
/*<       htb=1./float(ntb-1) >*/
    htb = (float)1. / (real) (ntb - 1);
/* loops on the zenith angle of the emitted radiation */
/*<       do km=1,ntb >*/
    i__1 = ntb;
    for (km = 1; km <= i__1; ++km) {
/*<         costet=(km-1)*htb >*/
	costet = (km - 1) * htb;
/*< 	if (costet.lt.0.99999999) then >*/
	if (costet < (float).99999999) {
/*<            tet=acos(costet) >*/
	    tet = acos(costet);
/*< 	   else >*/
	} else {
/*< 	   tet=0.0 >*/
	    tet = (float)0.;
/*< 	   endif >*/
	}
/*<         sintet=sin(tet) >*/
	sintet = sin(tet);
/*< 	tet=tet/fac >*/
	tet /= fac;
/* 	write(6,*) "sintet ",sintet,tet,costet */
/* Simpson's rules for the angle of the emitted radiation teta */
/*<         cotb=2. >*/
	cotb = (float)2.;
/*<         diff=abs(km/2-km/2.) >*/
	diff = (r__1 = km / 2 - km / (float)2., dabs(r__1));
/*<         if (diff.lt.0.00001)cotb=4. >*/
	if (diff < (float)1e-5) {
	    cotb = (float)4.;
	}
/*<         if (km.eq.1.or.km.eq.ntb)cotb=1.0 >*/
	if (km == 1 || km == ntb) {
	    cotb = (float)1.;
	}
/*  loops step for phiN and tetaN (N is the facet unit normal vector) 
*/
/*<         if (tet.lt.91)nta=801 >*/
	if (tet < (float)91.) {
	    nta = 801;
	}
/*<         if (tet.lt.81)nta=301 >*/
	if (tet < (float)81.) {
	    nta = 301;
	}
/*<         if (tet.lt.75)nta=101 >*/
	if (tet < (float)75.) {
	    nta = 101;
	}
/*<         if (tet.lt.65)nta=31 >*/
	if (tet < (float)65.) {
	    nta = 31;
	}
/*<         nfa=nta >*/
	nfa = nta;
/*<         hta=(1.-costt)/float(nta-1) >*/
	hta = ((float)1. - costt) / (real) (nta - 1);
/*<         hfa=pi/float(nfa-1) >*/
	hfa = pi / (real) (nfa - 1);
/* loops on phiN (azimuth angle of the facet normal vector) */
/*<         pr=0. >*/
	pr = (float)0.;
/*<         pp=0. >*/
	pp = (float)0.;
/*<         do i=1,nfa >*/
	i__2 = nfa;
	for (i__ = 1; i__ <= i__2; ++i__) {
/*<          phin=(i-1)*hfa >*/
	    phin = (i__ - 1) * hfa;
/*<          cosphin=cos(phin) >*/
	    cosphin = cos(phin);
/*<          sinphin=sin(phin) >*/
	    sinphin = sin(phin);
/*  Simpson's rules for phin */
/*<          cofa=2. >*/
	    cofa = (float)2.;
/*<          diff=abs(i/2-i/2.) >*/
	    diff = (r__1 = i__ / 2 - i__ / (float)2., dabs(r__1));
/*<          if (diff.lt.0.00001)cofa=4. >*/
	    if (diff < (float)1e-5) {
		cofa = (float)4.;
	    }
/*<          if (i.eq.1.or.i.eq.nfa)cofa=1.0 >*/
	    if (i__ == 1 || i__ == nfa) {
		cofa = (float)1.;
	    }
/* loops on tetaN (zenith angle of the facet normal vector) */
/*<          do j=1,nta >*/
	    i__3 = nta;
	    for (j = 1; j <= i__3; ++j) {
/*<           costetn=costt+(j-1)*hta >*/
		costetn = costt + (j - 1) * hta;
/*<           sintetn=sqrt(abs(1.-costetn*costetn)) >*/
		sintetn = sqrt((r__1 = (float)1. - costetn * costetn, dabs(
			r__1)));
/*<           tantetn=sintetn/costetn >*/
		tantetn = sintetn / costetn;
/*  Simpson's rules for tetaN */
/*<           cota=2. >*/
		cota = (float)2.;
/*<           diff=abs(j/2-j/2.) >*/
		diff = (r__1 = j / 2 - j / (float)2., dabs(r__1));
/*<           if (diff.lt.0.00001)cota=4. >*/
		if (diff < (float)1e-5) {
		    cota = (float)4.;
		}
/*<           if (j.eq.1.or.j.eq.nta)cota=1.0 >*/
		if (j == 1 || j == nta) {
		    cota = (float)1.;
		}
/* Fresnel's reflection coefficient R1 */
/*<           coschi=costet*costetn+sintet*sintetn*cosphin >*/
		coschi = costet * costetn + sintet * sintetn * cosphin;
/*       write(6,*)" coschi ",coschi,sintet,sintetn,cosphin */
/*<           if (coschi*coschi.gt.1.0)coschi=0.99999999999 >*/
		if (coschi * coschi > (float)1.) {
		    coschi = (float).99999999999;
		}
/*<           sinchi=sqrt(1-coschi*coschi) >*/
		sinchi = sqrt(1 - coschi * coschi);
/*<           if (coschi.lt.0.0)then >*/
		if (coschi < (float)0.) {
/*<             r1=0. >*/
		    r1 = (float)0.;
/*<             cota=0. >*/
		    cota = (float)0.;
/*<           else >*/
		} else {
/*<             Call Fresnel(nr,ni,coschi,sinchi,r1) >*/
		    fresnel_(nr, ni, &coschi, &sinchi, &r1);
/*<           endif >*/
		}
/*  Anisotropic Gaussian distribution for wave facets slopes 
*/
/*<           Zx=-tantetn*cosphin >*/
		zx = -tantetn * cosphin;
/*<           Zy=-tantetn*sinphin >*/
		zy = -tantetn * sinphin;
/*<           xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC) >*/
		xe = (cos(phw) * zx + sin(phw) * zy) / sqrt(sigmac);
/*<           xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU) >*/
		xn = (-sin(phw) * zx + cos(phw) * zy) / sqrt(sigmau);
/*<           xe2=xe*xe >*/
		xe2 = xe * xe;
/*<           xn2=xn*xn >*/
		xn2 = xn * xn;
/*<           coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn >*/
		coef = 1 - c21 / (float)2. * (xe2 - 1) * xn - c03 / (float)6. 
			* (xn2 - 3) * xn;
/*<           coef=coef+c40/24.*(xe2*xe2-6*xe2+3) >*/
		coef += c40 / (float)24. * (xe2 * xe2 - xe2 * 6 + 3);
/*<           coef=coef+C04/24.*(xn2*xn2-6*xn2+3) >*/
		coef += c04 / (float)24. * (xn2 * xn2 - xn2 * 6 + 3);
/*<           coef=coef+C22/4.*(xe2-1)*(xn2-1) >*/
		coef += c22 / (float)4. * (xe2 - 1) * (xn2 - 1);
/*<           fonc0=0.5*coschi*coef*exp(-(xe2+xn2)/2.)/(costetn**4) >*/
/* Computing 4th power */
		r__1 = costetn, r__1 *= r__1;
		fonc0 = coschi * (float).5 * coef * exp(-(xe2 + xn2) / (float)
			2.) / (r__1 * r__1);
/*<           pr=pr+r1*fonc0*cofa*cota*cotb >*/
		pr += r1 * fonc0 * cofa * cota * cotb;
/*<           pp=pp+fonc0*cofa*cota*cotb >*/
		pp += fonc0 * cofa * cota * cotb;
/*       write(6,*) coef,coschi,xe2 ,xn2," pr ",pr," pp ",pp 
*/
/*<          enddo >*/
	    }
/*<         enddo >*/
	}

/*<         pond=2.*hta*hfa*htb/pi/sqrt(sigmaC)/sqrt(sigmaU)/3./3./3. >*/
	pond = hta * (float)2. * hfa * htb / pi / sqrt(sigmac) / sqrt(sigmau) 
		/ (float)3. / (float)3. / (float)3.;
/*       write(6,*) "pond ",pond," pr ",pr," pp ",pp */
/*<         prefl=prefl+pr*pond >*/
	prefl += pr * pond;
/*<         proba=proba+pp*pond >*/
	proba += pp * pond;
/*<       enddo >*/
    }
/*      write(6,*) "proba ",proba," prefl ",prefl */
/*<       rge=prefl/proba >*/
    *rge = prefl / proba;
/*<       return >*/
    return 0;
/*<       end >*/
} /* glitalbe_ */

#ifdef __cplusplus
	}
#endif
