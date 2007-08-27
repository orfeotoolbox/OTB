/* OCEAALBE.f -- translated by f2c (version 19970805).
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

static doublereal c_b2 = 3.52;

/*<    >*/
/* Subroutine */ int oceaalbe_(real *pws, real *paw, real *xsal, real *pcl, 
	real *pwl, real *brdfalbe)
{
    /* Initialized data */

    static real ref[39] = { (float).22,(float).22,(float).22,(float).22,(
	    float).22,(float).22,(float).215,(float).21,(float).2,(float).19,(
	    float).175,(float).155,(float).13,(float).08,(float).1,(float)
	    .105,(float).1,(float).08,(float).045,(float).055,(float).065,(
	    float).06,(float).055,(float).04,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0. };

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    extern /* Subroutine */ int glitalbe_(real *, real *, real *, real *, 
	    real *);
    real a, c__, w, ni, nr, wl, rw;
    extern /* Subroutine */ int morcasiwat_(real *, real *, real *);
    real rwb, rwc;
    integer iwl;
    real azw, wlp, wspd, ref_i__;
    extern /* Subroutine */ int indwat_(real *, real *, real *, real *);
    real rogalbe;

/* INPUT:  pws=speed of wind (in m/s) */
/*         paw=azim. of sun - azim. of wind (in deg.) */
/* 	  xsal=salinity (in ppt) */
/* 	  pcl=pigment concentration (in mg.m-3) */
/*         pwl=wavelength of the computation (in micrometer) */
/* OUTPUT: brdfalbe=the spherical albedo of the ocean */

/*<       real Ref(39) >*/
/*<       real pwl,azw,pcl,wl,wspd,C,pws,brdfalbe,w,wlp,paw >*/
/*<       real ref_i,rwc,rw,rogalbe,a,rwb,xsal >*/
/*<       real nr,ni >*/
/*<       integer iwl >*/
/* effective reflectance of the whitecaps (Koepke, 1984) */
/*<    >*/
/* conversion of parameter */
/*<       C=pcl >*/
    c__ = *pcl;
/*<       wspd=pws >*/
    wspd = *pws;
/*<       azw=paw >*/
    azw = *paw;
/*<       wl=pwl >*/
    wl = *pwl;
/* COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN) */
/*<       W=2.95e-06*(wspd**3.52) >*/
    d__1 = (doublereal) wspd;
    w = pow_dd(&d__1, &c_b2) * (float)2.95e-6;
/*<       iwl=1+int((wl-0.2)/0.1) >*/
    iwl = (integer) ((wl - (float).2) / (float).1) + 1;
/*<       wlp=0.5+(iwl-1)*0.1 >*/
    wlp = (iwl - 1) * (float).1 + (float).5;
/*<       Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1)) >*/
    ref_i__ = ref[iwl] + (wl - wlp) / (float).1 * (ref[iwl - 1] - ref[iwl]);
/*<       Rwc=W*Ref_i >*/
    rwc = w * ref_i__;
/* COMPUTE WATER REFRACTION INDEX */
/*<       call indwat(wl,xsal,nr,ni) >*/
    indwat_(&wl, xsal, &nr, &ni);
/* COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN) */
/*  water reflectance below the sea surface */
/*<       call morcasiwat(wl,C,Rw) >*/
    morcasiwat_(&wl, &c__, &rw);
/* SUNGLINT spherical albedo */
/*<       call glitalbe(wspd,nr,ni,azw,rogalbe) >*/
    glitalbe_(&wspd, &nr, &ni, &azw, &rogalbe);
/*  water reflectance above the sea surface, (albedo re=0.485) */
/* albedo is a=re is taken from table 2 of Austin,1974,The remote sensing 
*/
/* of spectral radiance from below the ocean surface, in Optical */
/* Aspects of Oceanography (N.G. Jerlov and E. Steeman Nielsen,Eds), */
/* Academic,London,pp. 317-344 */
/*<       a=0.485 >*/
    a = (float).485;
/*<       Rwb=(1.-Rogalbe)*(1.-a)*Rw/(1-a*Rw) >*/
    rwb = ((float)1. - rogalbe) * ((float)1. - a) * rw / (1 - a * rw);
/* SPHERICAL ALBEDO OF SEA WATER */
/*<       brdfalbe=Rwc+(1-W)*Rogalbe+(1-Rwc)*Rwb >*/
    *brdfalbe = rwc + (1 - w) * rogalbe + (1 - rwc) * rwb;
/*<       return >*/
    return 0;
/*<       end >*/
} /* oceaalbe_ */

#ifdef __cplusplus
	}
#endif
