/* POSGE.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/* Common Block Declarations */

Extern struct {
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/*<    >*/
/* Subroutine */ int posge_(integer *month, integer *jday, real *tu, integer *
	nc, integer *nl, real *asol, real *phi0, real *avis, real *phiv, real 
	*xlon, real *xlat)
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double tan(doublereal), sqrt(doublereal), cos(doublereal), asin(
	    doublereal), atan(doublereal), sin(doublereal), atan2(doublereal, 
	    doublereal);

    /* Local variables */
    real x, y, re, pi, sn, rp, yk, rs, xr, yr, xt, yt, zt, aaa, gam, cdr, crd,
	     val1, val2;
    extern /* Subroutine */ int print_error__(char *, ftnlen);
    real alti, teta, ylat, tanx, tany, ylon, cosx2, deltax, deltay;
    extern /* Subroutine */ int possol_(integer *, integer *, real *, real *, 
	    real *, real *, real *);

/*<       logical ier >*/
/*<       real tu,asol,phi0,avis,phiv,xlon,xlat,yr,xr,alti >*/
/*<       real re,aaa,rp,pi,cdr,crd,deltax,deltay,x,y,rs >*/
/*<       real tanx,tany,val1,val2,yk,cosx2,sn,zt,xt,yt >*/
/*<       real teta,ylat,ylon,gam >*/
/*<       integer month,jday,nc,nl,iwr >*/
/*<       common/sixs_ier/iwr,ier >*/
/*     goes east definition */
/*<       yr=float(nl)-8665.5 >*/
    yr = (real) (*nl) - (float)8665.5;
/*<       xr=float(nc)-6498.5 >*/
    xr = (real) (*nc) - (float)6498.5;
/*<       alti=42107.0-6378.155 >*/
    alti = (float)35728.845000000001;
/*<       re=6378.155 >*/
    re = (float)6378.155;
/*<       aaa=1./297. >*/
    aaa = (float).0033670033670033669;
/*<       rp=re/(1.+aaa) >*/
    rp = re / (aaa + (float)1.);
/*<       pi=3.1415926 >*/
    pi = (float)3.1415926;
/*<       cdr=pi/180. >*/
    cdr = pi / (float)180.;
/*<       crd=180./pi >*/
    crd = (float)180. / pi;
/*<       deltax=18.0/12997.0 >*/
    deltax = (float).0013849349849965377;
/*<       deltay=20.0/17331.0 >*/
    deltay = (float).0011540015002019502;
/*<       x=xr*deltax*cdr >*/
    x = xr * deltax * cdr;
/*<       y=yr*deltay*cdr >*/
    y = yr * deltay * cdr;
/*<       rs=re+alti >*/
    rs = re + alti;
/*<       tanx=tan(x) >*/
    tanx = tan(x);
/*<       tany=tan(y) >*/
    tany = tan(y);
/*<       val1=1.0+(tanx**2) >*/
/* Computing 2nd power */
    r__1 = tanx;
    val1 = r__1 * r__1 + (float)1.;
/*<       val2=1.0+(tany*(1.0+aaa))**2 >*/
/* Computing 2nd power */
    r__1 = tany * (aaa + (float)1.);
    val2 = r__1 * r__1 + (float)1.;
/*<       yk=rs/re >*/
    yk = rs / re;
/*<       cosx2=1./(val1*val2) >*/
    cosx2 = (float)1. / (val1 * val2);
/*<       if((1./cosx2).gt.((yk**2)/(yk**2-1.))) goto 1000 >*/
/* Computing 2nd power */
    r__1 = yk;
/* Computing 2nd power */
    r__2 = yk;
    if ((float)1. / cosx2 > r__1 * r__1 / (r__2 * r__2 - (float)1.)) {
	goto L1000;
    }
/*<       sn=(rs-(re*(sqrt((yk**2)-(yk**2-1.)*(1./cosx2)))))/(1./cosx2) >*/
/* Computing 2nd power */
    r__1 = yk;
/* Computing 2nd power */
    r__2 = yk;
    sn = (rs - re * sqrt(r__1 * r__1 - (r__2 * r__2 - (float)1.) * ((float)1. 
	    / cosx2))) / ((float)1. / cosx2);
/*<       zt=rs-sn >*/
    zt = rs - sn;
/*<       xt=-(sn*tanx) >*/
    xt = -(sn * tanx);
/*<       yt=sn*tany/cos(x) >*/
    yt = sn * tany / cos(x);
/*<       teta=asin(yt/rp) >*/
    teta = asin(yt / rp);
/*<       ylat=(atan(((tan(teta))*rp)/re)) >*/
    ylat = atan(tan(teta) * rp / re);
/*<       ylon=atan(xt/zt) >*/
    ylon = atan(xt / zt);
/*<       goto 1001 >*/
    goto L1001;
/*<  1 >*/
L1000:
    print_error__("no possibility to compute lat. and long.", 40L);
/*<       return >*/
    return 0;
/*<  1001 xlat=ylat*crd >*/
L1001:
    *xlat = ylat * crd;
/*<       xlon=ylon*crd-75. >*/
    *xlon = ylon * crd - (float)75.;
/*<    >*/
    possol_(month, jday, tu, xlon, xlat, asol, phi0);
/*<       if(ier)return >*/
    if (sixs_ier__1.ier) {
	return 0;
    }
/*<       ylon=xlon*pi/180.+75.*cdr >*/
    ylon = *xlon * pi / (float)180. + cdr * (float)75.;
/*<       ylat=xlat*pi/180. >*/
    ylat = *xlat * pi / (float)180.;
/*<       gam=sqrt(((1./cosx2)-1.)*cosx2) >*/
    gam = sqrt(((float)1. / cosx2 - (float)1.) * cosx2);
/*<       avis=asin((1.+alti/re)*(gam)) >*/
    *avis = asin((alti / re + (float)1.) * gam);
/*<       avis=avis*180./pi >*/
    *avis = *avis * (float)180. / pi;
/*<       phiv=atan2(tan(ylon),sin(ylat))+pi >*/
    *phiv = atan2(tan(ylon), sin(ylat)) + pi;
/*<       phiv=phiv*180./pi >*/
    *phiv = *phiv * (float)180. / pi;
/*<       return >*/
    return 0;
/*<       end >*/
} /* posge_ */

#ifdef __cplusplus
	}
#endif
