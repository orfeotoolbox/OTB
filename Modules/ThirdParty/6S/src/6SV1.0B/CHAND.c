/* CHAND.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*< 	s >*/
/* Subroutine */ int chand_(doublereal *xphi, doublereal *xmuv, doublereal *
	xmus, doublereal *xtau, doublereal *xrray)
{
    /* Initialized data */

    static doublereal as0[10] = { .33243832,-.06777104,.1628537,.001577425,
	    -.30924818,-.01240906,-.10324388,.03241678,.11493334,-.03503695 };
    static doublereal as1[2] = { .19666292,-.05439061 };
    static doublereal as2[2] = { .14545937,-.02910845 };

    /* Builtin functions */
    double cos(doublereal), sqrt(doublereal), exp(doublereal), log(doublereal)
	    ;

    /* Local variables */
    integer i__;
    doublereal pi, pl[10], fs0, fs1, fs2, xp1, xp2, xp3, fac, xfd, xph1, xph2,
	     xph3, xdep, xitm, phios, cfonc1, cfonc2, cfonc3, xbeta2, xcosf1, 
	    xcosf2, xcosf3, xitot1, xitot2, xitot3, xlntau;

/* input parameters: xphi,xmus,xmuv,xtau */
/* xphi: azimuthal difference between sun and observation (xphi=0, */
/* in backscattering) and expressed in degree (0.:360.) */
/* xmus: cosine of the sun zenith angle */
/* xmuv: cosine of the observation zenith angle */
/* xtau: molecular optical depth */
/* output parameter: xrray : molecular reflectance (0.:1.) */
/* constant : xdep: depolarization factor (0.0279) */
/*< 	real xdep,pl(10) >*/
/*< 	real fs0,fs1,fs2 >*/
/*< 	real as0(10),as1(2),as2(2) >*/
/*<         real xphi,xmus,fac,xmuv,xtau,xrray,pi,phios,xcosf1,xcosf2 >*/
/*<         real xcosf3,xbeta2,xfd,xph1,xph2,xph3,xitm, xp1, xp2, xp3 >*/
/*<         real cfonc1,cfonc2,cfonc3,xlntau,xitot1,xitot2,xitot3 >*/
/*<         integer i >*/
/*< 	d >*/
/*< 	data (as1(i),i=1,2) /.19666292, -5.439061e-02/ >*/
/*< 	data (as2(i),i=1,2) /.14545937,-2.910845e-02/ >*/
/*< 	pi=3.1415927 >*/
    pi = 3.1415927;
/*< 	fac=pi/180. >*/
    fac = pi / 180.;
/*< 	phios=180.-xphi >*/
    phios = 180. - *xphi;
/*< 	xcosf1=1. >*/
    xcosf1 = 1.;
/*< 	xcosf2=cos(phios*fac) >*/
    xcosf2 = cos(phios * fac);
/*< 	xcosf3=cos(2*phios*fac) >*/
    xcosf3 = cos(phios * 2 * fac);
/*< 	xbeta2=0.5 >*/
    xbeta2 = .5;
/*< 	xdep=0.0279 >*/
    xdep = .0279;
/*< 	xfd=xdep/(2-xdep) >*/
    xfd = xdep / (2 - xdep);
/*< 	xfd=(1-xfd)/(1+2*xfd) >*/
    xfd = (1 - xfd) / (xfd * 2 + 1);
/*< 	xph1=1+(3*xmus*xmus-1)*(3*xmuv*xmuv-1)*xfd/8. >*/
    xph1 = (*xmus * 3 * *xmus - 1) * (*xmuv * 3 * *xmuv - 1) * xfd / 8. + 1;
/*< 	xph2=-xmus*xmuv*sqrt(1-xmus*xmus)*sqrt(1-xmuv*xmuv) >*/
    xph2 = -(*xmus) * *xmuv * sqrt(1 - *xmus * *xmus) * sqrt(1 - *xmuv * *
	    xmuv);
/*< 	xph2=xph2*xfd*xbeta2*1.5 >*/
    xph2 = xph2 * xfd * xbeta2 * 1.5;
/*< 	xph3=(1-xmus*xmus)*(1-xmuv*xmuv) >*/
    xph3 = (1 - *xmus * *xmus) * (1 - *xmuv * *xmuv);
/*< 	xph3=xph3*xfd*xbeta2*0.375 >*/
    xph3 = xph3 * xfd * xbeta2 * .375;
/*< 	xitm=(1-exp(-xtau*(1/xmus+1/xmuv)))*xmus/(4*(xmus+xmuv)) >*/
    xitm = (1 - exp(-(*xtau) * (1 / *xmus + 1 / *xmuv))) * *xmus / ((*xmus + *
	    xmuv) * 4);
/*< 	xp1=xph1*xitm >*/
    xp1 = xph1 * xitm;
/*< 	xp2=xph2*xitm >*/
    xp2 = xph2 * xitm;
/*< 	xp3=xph3*xitm >*/
    xp3 = xph3 * xitm;
/*< 	xitm=(1-exp(-xtau/xmus))*(1-exp(-xtau/xmuv)) >*/
    xitm = (1 - exp(-(*xtau) / *xmus)) * (1 - exp(-(*xtau) / *xmuv));
/*< 	cfonc1=xph1*xitm >*/
    cfonc1 = xph1 * xitm;
/*< 	cfonc2=xph2*xitm >*/
    cfonc2 = xph2 * xitm;
/*< 	cfonc3=xph3*xitm >*/
    cfonc3 = xph3 * xitm;
/*< 	xlntau=log(xtau) >*/
    xlntau = log(*xtau);
/*< 	pl(1)=1. >*/
    pl[0] = 1.;
/*< 	pl(2)=xlntau >*/
    pl[1] = xlntau;
/*< 	pl(3)=xmus+xmuv >*/
    pl[2] = *xmus + *xmuv;
/*< 	pl(4)=xlntau*pl(3) >*/
    pl[3] = xlntau * pl[2];
/*< 	pl(5)=xmus*xmuv >*/
    pl[4] = *xmus * *xmuv;
/*< 	pl(6)=xlntau*pl(5) >*/
    pl[5] = xlntau * pl[4];
/*< 	pl(7)=xmus*xmus+xmuv*xmuv >*/
    pl[6] = *xmus * *xmus + *xmuv * *xmuv;
/*< 	pl(8)=xlntau*pl(7) >*/
    pl[7] = xlntau * pl[6];
/*< 	pl(9)=xmus*xmus*xmuv*xmuv >*/
    pl[8] = *xmus * *xmus * *xmuv * *xmuv;
/*< 	pl(10)=xlntau*pl(9) >*/
    pl[9] = xlntau * pl[8];
/*< 	fs0=0. >*/
    fs0 = 0.;
/*< 	do i=1,10 >*/
    for (i__ = 1; i__ <= 10; ++i__) {
/*< 	fs0=fs0+pl(i)*as0(i) >*/
	fs0 += pl[i__ - 1] * as0[i__ - 1];
/*< 	enddo >*/
    }
/*< 	fs1=pl(1)*as1(1)+pl(2)*as1(2) >*/
    fs1 = pl[0] * as1[0] + pl[1] * as1[1];
/*< 	fs2=pl(1)*as2(1)+pl(2)*as2(2) >*/
    fs2 = pl[0] * as2[0] + pl[1] * as2[1];
/*< 	xitot1=xp1+cfonc1*fs0*xmus >*/
    xitot1 = xp1 + cfonc1 * fs0 * *xmus;
/*< 	xitot2=xp2+cfonc2*fs1*xmus >*/
    xitot2 = xp2 + cfonc2 * fs1 * *xmus;
/*< 	xitot3=xp3+cfonc3*fs2*xmus >*/
    xitot3 = xp3 + cfonc3 * fs2 * *xmus;
/*< 	xrray=xitot1*xcosf1 >*/
    *xrray = xitot1 * xcosf1;
/*< 	xrray=xrray+xitot2*xcosf2*2 >*/
    *xrray += xitot2 * xcosf2 * 2;
/*< 	xrray=xrray+xitot3*xcosf3*2 >*/
    *xrray += xitot3 * xcosf3 * 2;
/*< 	xrray=xrray/xmus >*/
    *xrray /= *xmus;
/*< 	return >*/
    return 0;
/*< 	end >*/
} /* chand_ */

#ifdef __cplusplus
	}
#endif
