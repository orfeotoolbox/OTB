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
/* Subroutine */ int morcasiwat_(doublereal *wl, doublereal *c__, doublereal *
	r2)
{
    /* Initialized data */

    static doublereal tkw[61] = { .0209,.02,.0196,.0189,.0183,.0182,.0171,
	    .017,.0168,.0166,.0168,.017,.0173,.0174,.0175,.0184,.0194,.0203,
	    .0217,.024,.0271,.032,.0384,.0445,.049,.0505,.0518,.0543,.0568,
	    .0615,.064,.064,.0717,.0762,.0807,.094,.107,.128,.157,.2,.253,
	    .279,.296,.303,.31,.315,.32,.325,.33,.34,.35,.37,.405,.418,.43,
	    .44,.45,.47,.5,.55,.65 };
    static doublereal txc[61] = { .11,.111,.1125,.1135,.1126,.1104,.1078,
	    .1065,.1041,.0996,.0971,.0939,.0896,.0859,.0823,.0788,.0746,.0726,
	    .069,.066,.0636,.06,.0578,.054,.0498,.0475,.0467,.045,.044,.0426,
	    .041,.04,.039,.0375,.036,.034,.033,.0328,.0325,.033,.034,.035,
	    .036,.0375,.0385,.04,.042,.043,.044,.0445,.045,.046,.0475,.049,
	    .0515,.052,.0505,.044,.039,.034,.03 };
    static doublereal te[61] = { .668,.672,.68,.687,.693,.701,.707,.708,.707,
	    .704,.701,.699,.7,.703,.703,.703,.703,.704,.702,.7,.7,.695,.69,
	    .685,.68,.675,.67,.665,.66,.655,.65,.645,.64,.63,.623,.615,.61,
	    .614,.618,.622,.626,.63,.634,.638,.642,.647,.653,.658,.663,.667,
	    .672,.677,.682,.687,.695,.697,.693,.665,.64,.62,.6 };
    static doublereal tbw[61] = { .0076,.0072,.0068,.0064,.0061,.0058,.0055,
	    .0052,.0049,.0047,.0045,.0043,.0041,.0039,.0037,.0036,.0034,.0033,
	    .0031,.003,.0029,.0027,.0026,.0025,.0024,.0023,.0022,.0022,.0021,
	    .002,.0019,.0018,.0018,.0017,.0017,.0016,.0016,.0015,.0015,.0014,
	    .0014,.0013,.0013,.0012,.0012,.0011,.0011,.001,.001,.001,.001,
	    9e-4,8e-4,8e-4,8e-4,7e-4,7e-4,7e-4,7e-4,7e-4,7e-4 };

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    integer i_dnnt(doublereal *);
    double pow_dd(doublereal *, doublereal *), d_lg10(doublereal *);

    /* Local variables */
    doublereal b, e, r1, u1, u2, bb, kd, bw, xc, kw, bbt, err;
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
    if (*wl < .4 || *wl > .7) {
/*< 	R2=0.000 >*/
	*r2 = 0.;
/*< 	goto 60 >*/
	goto L60;
/*<       endif >*/
    }
/*<       iwl=1+nint((wl-0.400)/0.005) >*/
    d__1 = (*wl - .4) / .005;
    iwl = i_dnnt(&d__1) + 1;
/*<       Kw=tKw(iwl) >*/
    kw = tkw[iwl - 1];
/*<       Xc=tXc(iwl) >*/
    xc = txc[iwl - 1];
/*<       e=te(iwl) >*/
    e = te[iwl - 1];
/*<       bw=tbw(iwl) >*/
    bw = tbw[iwl - 1];

/*<       if (abs(C).lt.0.0001)then >*/
    if (abs(*c__) < 1e-4) {
/*<          bb=0.5*bw >*/
	bb = bw * .5;
/*<          Kd=Kw >*/
	kd = kw;
/*<       else >*/
    } else {
/*<          b=0.30*C**0.62 >*/
	b = pow_dd(c__, &c_b3) * .3;
/*<          bbt=0.002+0.02*(0.5-0.25*alog10(C))*0.550/wl >*/
	bbt = (.5 - d_lg10(c__) * .25) * .02 * .55 / *wl + .002;
/*<          bb=0.5*bw+bbt*b >*/
	bb = bw * .5 + bbt * b;
/*<          Kd=Kw+Xc*C**e >*/
	kd = kw + xc * pow_dd(c__, &e);
/*<       endif >*/
    }
/*<       u1=0.75 >*/
    u1 = .75;
/*<       R1=0.33*bb/u1/Kd >*/
    r1 = bb * .33 / u1 / kd;
/*<  50   u2=0.90*(1.-R1)/(1.+2.25*R1) >*/
L50:
    u2 = (1. - r1) * .9 / (r1 * 2.25 + 1.);
/*<       R2=0.33*bb/u2/Kd >*/
    *r2 = bb * .33 / u2 / kd;
/*<       err=abs((R2-R1)/R2) >*/
    err = (d__1 = (*r2 - r1) / *r2, abs(d__1));
/*<       if (err.lt.0.0001)goto 60 >*/
    if (err < 1e-4) {
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
/* Subroutine */ int indwat_(doublereal *wl, doublereal *xsal, doublereal *nr,
	 doublereal *ni)
{
    /* Initialized data */

    static doublereal twl[62] = { .25,.275,.3,.325,.345,.375,.4,.425,.445,
	    .475,.5,.525,.55,.575,.6,.625,.65,.675,.7,.725,.75,.775,.8,.825,
	    .85,.875,.9,.925,.95,.975,1.,1.2,1.4,1.6,1.8,2.,2.2,2.4,2.6,2.65,
	    2.7,2.75,2.8,2.85,2.9,2.95,3.,3.05,3.1,3.15,3.2,3.25,3.3,3.35,3.4,
	    3.45,3.5,3.6,3.7,3.8,3.9,4. };
    static doublereal tnr[62] = { 1.362,1.354,1.349,1.346,1.343,1.341,1.339,
	    1.338,1.337,1.336,1.335,1.334,1.333,1.333,1.332,1.332,1.331,1.331,
	    1.331,1.33,1.33,1.33,1.329,1.329,1.329,1.328,1.328,1.328,1.327,
	    1.327,1.327,1.324,1.321,1.317,1.312,1.306,1.296,1.279,1.242,1.219,
	    1.188,1.157,1.142,1.149,1.201,1.292,1.371,1.426,1.467,1.483,1.478,
	    1.467,1.45,1.432,1.42,1.41,1.4,1.385,1.374,1.364,1.357,1.351 };
    static doublereal tni[62] = { 3.35e-8,2.35e-8,1.6e-8,1.08e-8,6.5e-9,
	    3.5e-9,1.86e-9,1.3e-9,1.02e-9,9.35e-10,1e-9,1.32e-9,1.96e-9,
	    3.6e-9,1.09e-8,1.39e-8,1.64e-8,2.23e-8,3.35e-8,9.15e-8,1.56e-7,
	    1.48e-7,1.25e-7,1.82e-7,2.93e-7,3.91e-7,4.86e-7,1.06e-6,2.93e-6,
	    3.48e-6,2.89e-6,9.89e-6,1.38e-4,8.55e-5,1.15e-4,.0011,2.89e-4,
	    9.56e-4,.00317,.0067,.019,.059,.115,.185,.268,.298,.272,.24,.192,
	    .135,.0924,.061,.0368,.0261,.0195,.0132,.0094,.00515,.0036,.0034,
	    .0038,.0046 };

    integer i__;
    doublereal yi, yr, nic, nrc, xwl;


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
    nrc = .006;
/*<         nic=0.000 >*/
    nic = 0.;
/*<         nr=nr+nrc*(xsal/34.3) >*/
    *nr += nrc * (*xsal / 34.3);
/*< 	ni=ni+nic*(xsal/34.3) >*/
    *ni += nic * (*xsal / 34.3);
/*<         return >*/
    return 0;
/*<         end >*/
} /* indwat_ */


/*<       subroutine sunglint(wspd,nr,ni,azw,ts,tv,fi,rog) >*/
/* Subroutine */ int sunglint_(doublereal *wspd, doublereal *nr, doublereal *
	ni, doublereal *azw, doublereal *ts, doublereal *tv, doublereal *fi, 
	doublereal *rog)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal), sqrt(
	    doublereal), exp(doublereal);

    /* Local variables */
    doublereal r1, c21, c03, c40, c04, c22, cs, pi, cv, xe, ss, xn, sv, zx, 
	    zy, xe2, xn2, fac, phi, phw, coef, tilt, proba, sigmac, coschi, 
	    sinchi, sigmau, cos2chi;
    extern /* Subroutine */ int fresnel_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    doublereal tantilt;

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
    pi = atan(1.) * 4.;
/*<       fac=pi/180. >*/
    fac = pi / 180.;
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
/*      write(6,*) "probaglit:",proba */
/*      write(6,*) "coef glit:",coef */
/*      write(6,*) "tilt glit:",tilt */
/*      write(6,*) "phw glit:",phw */
/* Compute Fresnel's coefficient R1 */
/*<       cos2chi=cv*cs+sv*ss*cos(phi) >*/
    cos2chi = cv * cs + sv * ss * cos(phi);
/*<       if (cos2chi.gt.1.0)cos2chi=0.99999999999 >*/
    if (cos2chi > 1.) {
	cos2chi = .99999999999;
    }
/*<       if (cos2chi.lt.-1.0)cos2chi=-0.99999999999 >*/
    if (cos2chi < -1.) {
	cos2chi = -.99999999999;
    }
/*<       coschi=sqrt(0.5*(1+cos2chi)) >*/
    coschi = sqrt((cos2chi + 1) * .5);
/*<       sinchi=sqrt(0.5*(1-cos2chi)) >*/
    sinchi = sqrt((1 - cos2chi) * .5);
/*<       if (coschi.ge.1.0)coschi=0.99999999 >*/
    if (coschi >= 1.) {
	coschi = .99999999;
    }
/*<       if (coschi.le.-1.0)coschi=-0.999999 >*/
    if (coschi <= -1.) {
	coschi = -.999999;
    }
/*<        if (sinchi.gt.1.0)sinchi=0.9999999 >*/
    if (sinchi > 1.) {
	sinchi = .9999999;
    }
/*<       if (sinchi.lt.-1.0)sinchi=-0.999999 >*/
    if (sinchi < -1.) {
	sinchi = -.999999;
    }
/*<       Call Fresnel(nr,ni,coschi,sinchi,R1) >*/
    fresnel_(nr, ni, &coschi, &sinchi, &r1);
/* Compute Reflectance of the sun glint */
/*<       Rog=pi*R1*proba/4./cs/cv/(cos(tilt)**4) >*/
/* Computing 4th power */
    d__1 = cos(tilt), d__1 *= d__1;
    *rog = pi * r1 * proba / 4. / cs / cv / (d__1 * d__1);
/*      write(6,*) "ROg ",Rog,R1,proba */
/*<       return >*/
    return 0;
/*<       end >*/
} /* sunglint_ */



/*<       Subroutine Fresnel(nr,ni,coschi,sinchi,R1) >*/
/* Subroutine */ int fresnel_(doublereal *nr, doublereal *ni, doublereal *
	coschi, doublereal *sinchi, doublereal *r1)
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
    a2 = sqrt(pow_dd(&d__1, &c_b10) + *nr * 4 * *nr * *ni * *ni);
/*<       u=sqrt(0.5*abs(a1+a2)) >*/
    u = sqrt((d__1 = a1 + a2, abs(d__1)) * .5);
/*<       v=sqrt(0.5*abs(-a1+a2)) >*/
    v = sqrt((d__1 = -a1 + a2, abs(d__1)) * .5);
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
/*      write(6,*) "fresnel ", R1,u,v,a1,a2 */
/*<       return >*/
    return 0;
/*<       end >*/
} /* fresnel_ */



/*<       subroutine glitalbe(wspd,nr,ni,azw,rge) >*/
/* Subroutine */ int glitalbe_(doublereal *wspd, doublereal *nr, doublereal *
	ni, doublereal *azw, doublereal *rge)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal), acos(doublereal), sin(
	    doublereal), cos(doublereal), exp(doublereal);

    /* Local variables */
    integer i__, j;
    doublereal q, r1, c21, c03, c40, c04, c22;
    integer km;
    doublereal pi, xe, pp, pr, xn, zx, zy, xe2, xn2, fac, hfa;
    integer nfa;
    doublereal hta, htb;
    integer nta, ntb;
    doublereal tet, phw, cofa, diff, coef, cota, cotb, phin, pond, fonc0, 
	    sigma, proba, prefl, costt, sigmac, coschi, sinchi, sigmau, 
	    costet, sintet;
    extern /* Subroutine */ int fresnel_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    doublereal cosphin, sinphin, tantetn, costetn, sintetn;


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
    pi = atan(1.) * 4.;
/*<       fac=pi/180. >*/
    fac = pi / 180.;
/*<       sigma=0.003+0.00512*wspd >*/
    sigma = *wspd * .00512 + .003;
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
/* costt to minimize the time of the computation */
/*     integration between 1 and costt instead of 1 and 0 */
/*<       q=50 >*/
    q = 50.;
/*<       costt=1./sqrt(1+q*sigma/4.) >*/
    costt = 1. / sqrt(q * sigma / 4. + 1);
/*<       phw=azw*fac >*/
    phw = *azw * fac;
/*<       prefl=0. >*/
    prefl = 0.;
/*<       proba=0. >*/
    proba = 0.;
/*<       ntb=31 >*/
    ntb = 31;
/*<       htb=1./float(ntb-1) >*/
    htb = 1. / (doublereal) (ntb - 1);
/* loops on the zenith angle of the emitted radiation */
/*<       do km=1,ntb >*/
    i__1 = ntb;
    for (km = 1; km <= i__1; ++km) {
/*<         costet=(km-1)*htb >*/
	costet = (km - 1) * htb;
/*< 	if (costet.lt.0.99999999) then >*/
	if (costet < .99999999) {
/*<            tet=acos(costet) >*/
	    tet = acos(costet);
/*< 	   else >*/
	} else {
/*< 	   tet=0.0 >*/
	    tet = 0.;
/*< 	   endif >*/
	}
/*<         sintet=sin(tet) >*/
	sintet = sin(tet);
/*< 	tet=tet/fac >*/
	tet /= fac;
/* 	write(6,*) "sintet ",sintet,tet,costet */
/* Simpson's rules for the angle of the emitted radiation teta */
/*<         cotb=2. >*/
	cotb = 2.;
/*<         diff=abs(km/2-km/2.) >*/
	diff = (d__1 = km / 2 - km / 2., abs(d__1));
/*<         if (diff.lt.0.00001)cotb=4. >*/
	if (diff < 1e-5) {
	    cotb = 4.;
	}
/*<         if (km.eq.1.or.km.eq.ntb)cotb=1.0 >*/
	if (km == 1 || km == ntb) {
	    cotb = 1.;
	}
/*  loops step for phiN and tetaN (N is the facet unit normal vector) 
*/
/*<         if (tet.lt.91)nta=801 >*/
	if (tet < 91.) {
	    nta = 801;
	}
/*<         if (tet.lt.81)nta=301 >*/
	if (tet < 81.) {
	    nta = 301;
	}
/*<         if (tet.lt.75)nta=101 >*/
	if (tet < 75.) {
	    nta = 101;
	}
/*<         if (tet.lt.65)nta=31 >*/
	if (tet < 65.) {
	    nta = 31;
	}
/*<         nfa=nta >*/
	nfa = nta;
/*<         hta=(1.-costt)/float(nta-1) >*/
	hta = (1. - costt) / (doublereal) (nta - 1);
/*<         hfa=pi/float(nfa-1) >*/
	hfa = pi / (doublereal) (nfa - 1);
/* loops on phiN (azimuth angle of the facet normal vector) */
/*<         pr=0. >*/
	pr = 0.;
/*<         pp=0. >*/
	pp = 0.;
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
	    cofa = 2.;
/*<          diff=abs(i/2-i/2.) >*/
	    diff = (d__1 = i__ / 2 - i__ / 2., abs(d__1));
/*<          if (diff.lt.0.00001)cofa=4. >*/
	    if (diff < 1e-5) {
		cofa = 4.;
	    }
/*<          if (i.eq.1.or.i.eq.nfa)cofa=1.0 >*/
	    if (i__ == 1 || i__ == nfa) {
		cofa = 1.;
	    }
/* loops on tetaN (zenith angle of the facet normal vector) */
/*<          do j=1,nta >*/
	    i__3 = nta;
	    for (j = 1; j <= i__3; ++j) {
/*<           costetn=costt+(j-1)*hta >*/
		costetn = costt + (j - 1) * hta;
/*<           sintetn=sqrt(abs(1.-costetn*costetn)) >*/
		sintetn = sqrt((d__1 = 1. - costetn * costetn, abs(d__1)));
/*<           tantetn=sintetn/costetn >*/
		tantetn = sintetn / costetn;
/*  Simpson's rules for tetaN */
/*<           cota=2. >*/
		cota = 2.;
/*<           diff=abs(j/2-j/2.) >*/
		diff = (d__1 = j / 2 - j / 2., abs(d__1));
/*<           if (diff.lt.0.00001)cota=4. >*/
		if (diff < 1e-5) {
		    cota = 4.;
		}
/*<           if (j.eq.1.or.j.eq.nta)cota=1.0 >*/
		if (j == 1 || j == nta) {
		    cota = 1.;
		}
/* Fresnel's reflection coefficient R1 */
/*<           coschi=costet*costetn+sintet*sintetn*cosphin >*/
		coschi = costet * costetn + sintet * sintetn * cosphin;
/*       write(6,*)" coschi ",coschi,sintet,sintetn,cosphin */
/*<           if (coschi*coschi.gt.1.0)coschi=0.99999999999 >*/
		if (coschi * coschi > 1.) {
		    coschi = .99999999999;
		}
/*<           sinchi=sqrt(1-coschi*coschi) >*/
		sinchi = sqrt(1 - coschi * coschi);
/*<           if (coschi.lt.0.0)then >*/
		if (coschi < 0.) {
/*<             r1=0. >*/
		    r1 = 0.;
/*<             cota=0. >*/
		    cota = 0.;
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
		coef = 1 - c21 / 2. * (xe2 - 1) * xn - c03 / 6. * (xn2 - 3) * 
			xn;
/*<           coef=coef+c40/24.*(xe2*xe2-6*xe2+3) >*/
		coef += c40 / 24. * (xe2 * xe2 - xe2 * 6 + 3);
/*<           coef=coef+C04/24.*(xn2*xn2-6*xn2+3) >*/
		coef += c04 / 24. * (xn2 * xn2 - xn2 * 6 + 3);
/*<           coef=coef+C22/4.*(xe2-1)*(xn2-1) >*/
		coef += c22 / 4. * (xe2 - 1) * (xn2 - 1);
/*<           fonc0=0.5*coschi*coef*exp(-(xe2+xn2)/2.)/(costetn**4) >*/
/* Computing 4th power */
		d__1 = costetn, d__1 *= d__1;
		fonc0 = coschi * .5 * coef * exp(-(xe2 + xn2) / 2.) / (d__1 * 
			d__1);
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
	pond = hta * 2. * hfa * htb / pi / sqrt(sigmac) / sqrt(sigmau) / 3. / 
		3. / 3.;
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
