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
/* Subroutine */ int oceaalbe_(doublereal *pws, doublereal *paw, doublereal *
	xsal, doublereal *pcl, doublereal *pwl, doublereal *brdfalbe)
{
    /* Initialized data */

    static doublereal ref[39] = { .22,.22,.22,.22,.22,.22,.215,.21,.2,.19,
	    .175,.155,.13,.08,.1,.105,.1,.08,.045,.055,.065,.06,.055,.04,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0. };

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    extern /* Subroutine */ int glitalbe_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    doublereal a, c__, w, ni, nr, wl, rw;
    extern /* Subroutine */ int morcasiwat_(doublereal *, doublereal *, 
	    doublereal *);
    doublereal rwb, rwc;
    integer iwl;
    doublereal azw, wlp, wspd, ref_i__;
    extern /* Subroutine */ int indwat_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal rogalbe;

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
    w = pow_dd(&wspd, &c_b2) * 2.95e-6;
/*<       iwl=1+int((wl-0.2)/0.1) >*/
    iwl = (integer) ((wl - .2) / .1) + 1;
/*<       wlp=0.5+(iwl-1)*0.1 >*/
    wlp = (iwl - 1) * .1 + .5;
/*<       Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1)) >*/
    ref_i__ = ref[iwl] + (wl - wlp) / .1 * (ref[iwl - 1] - ref[iwl]);
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
    a = .485;
/*<       Rwb=(1.-Rogalbe)*(1.-a)*Rw/(1-a*Rw) >*/
    rwb = (1. - rogalbe) * (1. - a) * rw / (1 - a * rw);
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
