/* POSMTO.f -- translated by f2c (version 19970805).
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
/* Subroutine */ int posmto_(integer *month, integer *jday, doublereal *tu, 
	integer *nc, integer *nl, doublereal *asol, doublereal *phi0, 
	doublereal *avis, doublereal *phiv, doublereal *xlon, doublereal *
	xlat)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double tan(doublereal), sqrt(doublereal), cos(doublereal), asin(
	    doublereal), atan(doublereal), sin(doublereal), atan2(doublereal, 
	    doublereal);

    /* Local variables */
    doublereal x, y, re, pi, sn, rp, yk, rs, xr, yr, xt, yt, zt, aaa, gam, 
	    cdr, crd, val1, val2;
    extern /* Subroutine */ int print_error__(char *, ftnlen);
    doublereal alti, teta, ylat, tanx, tany, ylon, cosx2, deltax, deltay;
    extern /* Subroutine */ int possol_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);

/*<       logical ier >*/
/*<       real tu,asol,phi0,avis,phiv,xlon,xlat,yr,xr,alti >*/
/*<       real re,aaa,rp,pi,cdr,crd,deltax,deltay,x,y,rs >*/
/*<       real tanx,tany,val1,val2,yk,cosx2,sn,zt,xt,yt >*/
/*<       real teta,ylat,ylon,gam >*/
/*<       integer month,jday,nc,nl,iwr >*/
/*<       common/sixs_ier/iwr,ier >*/
/*     meteosat definition */
/*<       yr=float(nl)-1250.5 >*/
    yr = (doublereal) (*nl) - 1250.5;
/*<       xr=float(nc)-2500.5 >*/
    xr = (doublereal) (*nc) - 2500.5;
/*<       alti=42164.0-6378.155 >*/
    alti = 35785.845000000001;
/*<       re=6378.155 >*/
    re = 6378.155;
/*<       aaa=1./297. >*/
    aaa = .0033670033670033669;
/*<       rp=re/(1.+aaa) >*/
    rp = re / (aaa + 1.);
/*<       pi=3.1415926 >*/
    pi = 3.1415926;
/*<       cdr=pi/180. >*/
    cdr = pi / 180.;
/*<       crd=180./pi >*/
    crd = 180. / pi;
/*<       deltax=18.0/5000.0 >*/
    deltax = .0035999999999999999;
/*<       deltay=18.0/2500.0 >*/
    deltay = .0071999999999999998;
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
    d__1 = tanx;
    val1 = d__1 * d__1 + 1.;
/*<       val2=1.0+(tany*(1.0+aaa))**2 >*/
/* Computing 2nd power */
    d__1 = tany * (aaa + 1.);
    val2 = d__1 * d__1 + 1.;
/*<       yk=rs/re >*/
    yk = rs / re;
/*<       cosx2=1./(val1*val2) >*/
    cosx2 = 1. / (val1 * val2);
/*<       if((1./cosx2).gt.((yk**2)/(yk**2-1.))) goto 1000 >*/
/* Computing 2nd power */
    d__1 = yk;
/* Computing 2nd power */
    d__2 = yk;
    if (1. / cosx2 > d__1 * d__1 / (d__2 * d__2 - 1.)) {
	goto L1000;
    }
/*<       sn=(rs-(re*(sqrt((yk**2)-(yk**2-1.)*(1./cosx2)))))/(1./cosx2) >*/
/* Computing 2nd power */
    d__1 = yk;
/* Computing 2nd power */
    d__2 = yk;
    sn = (rs - re * sqrt(d__1 * d__1 - (d__2 * d__2 - 1.) * (1. / cosx2))) / (
	    1. / cosx2);
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
/*<       xlon=ylon*crd >*/
    *xlon = ylon * crd;
/*<    >*/
    possol_(month, jday, tu, xlon, xlat, asol, phi0);
/*<       if(ier)return >*/
    if (sixs_ier__1.ier) {
	return 0;
    }
/*<       ylon=xlon*pi/180. >*/
    ylon = *xlon * pi / 180.;
/*<       ylat=xlat*pi/180. >*/
    ylat = *xlat * pi / 180.;
/*<       gam=sqrt(((1./cosx2)-1.)*cosx2) >*/
    gam = sqrt((1. / cosx2 - 1.) * cosx2);
/*<       avis=asin((1.+alti/re)*(gam)) >*/
    *avis = asin((alti / re + 1.) * gam);
/*<       avis=avis*180./pi >*/
    *avis = *avis * 180. / pi;
/*<       phiv=atan2(tan(ylon),sin(ylat))+pi >*/
    *phiv = atan2(tan(ylon), sin(ylat)) + pi;
/*<       phiv=phiv*180./pi >*/
    *phiv = *phiv * 180. / pi;
/*<       return >*/
    return 0;
/*<       end >*/
} /* posmto_ */

#ifdef __cplusplus
	}
#endif
