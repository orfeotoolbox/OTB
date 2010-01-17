/* VERSBRDF.f -- translated by f2c (version 19970805).
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

/* Table of constant values */

static integer c__1 = 1;

/*<    >*/
/* Subroutine */ int versbrdf_(integer *option, doublereal *optics, 
	doublereal *struct__, integer *mu, integer *np, doublereal *rm, 
	doublereal *rp, doublereal *brdfint)
{
    /* Initialized data */

    static char fmt[60*6+1] = "(i10)                                        \
               (e10.3)                                                     (\
1x, a10, 6 (i8, 2x))                                       (1x, a10, 6 (f8.3\
, 2x))                                     (1x, a, f9.6)                    \
                           (1x, a)                                          \
           ";
    static char errmsg[80*15+1] = "101: option (1) not an acceptable value, \
reset to 1                             102: option (2) not an acceptable val\
ue, reset to 1                             103: option (3) not an acceptable\
 value, reset to 1                             104: option (4) not an accept\
able value, reset to 1                             105: option (5) not an ac\
ceptable value, reset to 1                             201: illumination zen\
ith angle (angles (1)) not acceptable                      202: observation \
zenith angle (angles (2)) not acceptable                       203: relative\
 azimuth angle (angles (3)) not acceptable                         301: sing\
le scattering albedo (optics (1)) not acceptable                       302: \
phase function parameter (optics (2) not acceptable                        4\
01: leaf area density (struct (1)) not acceptable                           \
   402: radius of sun flecks (struct (2)) not acceptable                    \
       411: kappa1 (struct (3) with option (3) = 0) not acceptable          \
           412: kappa2 (struct (4) with option (3) = 0) not acceptable      \
               421: chil (struct (3) with option (3) = 1) not acceptable    \
                   ";

    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    cilist ci__1;

    /* Builtin functions */
    double atan(doublereal), acos(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();

    /* Local variables */
    integer j, k;
    doublereal fi, pi, mu1, mu2;
    extern doublereal mvbp1_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    integer error;
    doublereal angles[3];


/* interface between the computer code of the model of verstraete and pin 
*/
/* the computer code is courtesy of michel verstraete. */
/* see subroutine mvbp1 for a complete description */


/*  variables angles, error, optics, option, struct and mvbp1 are fully */
/*  described in the file mvbp1.for. */
/*     angdeg       angles in degrees */
/*     brdf         variable to receive the bidirectional reflectance */
/*                  value returned by the function mvbp1. */
/*     fmt          formats. */
/*     errmsg       error messages. */
/*<       integer np,mu >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       integer      option (5), error >*/
/*<       real         angles (3), optics (3), struct (4) >*/
/*<       real         mvbp1 >*/
/*<       character*60 fmt (6) >*/
/*<       character*80 errmsg (15) >*/
/*<       external     mvbp1 >*/
/*<       logical ier >*/
/*<       integer iwr,k,j >*/
/* THOMAS deplacement variable avant declaration data */
/*<       real mu1,mu2,fi >*/
/*<       real pi >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       data fmt (1) /'(i10)'/ >*/
    /* Parameter adjustments */
    --option;
    --optics;
    --struct__;
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
/*<       data fmt (2) /'(e10.3)'/ >*/
/*<       data fmt (3) /'(1x, a10, 6 (i8, 2x))'/ >*/
/*<       data fmt (4) /'(1x, a10, 6 (f8.3, 2x))'/ >*/
/*<       data fmt (5) /'(1x, a, f9.6)'/ >*/
/*<       data fmt (6) /'(1x, a)'/ >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/* THOMAS deplacement variable avant declaration data */
/*      real mu1,mu2,fi */
/*      real pi */
/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       mu1=rm(0) >*/
    mu1 = rm[0];
/*<       do 1 k=1,np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j=1,mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<       mu2=rm(j) >*/
	    mu2 = rm[j];
/*<       if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<          fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<          else >*/
	    } else {
/*<          fi=rp(k)+rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<       endif >*/
	    }
/* compute geometrical parameter for mvbp1 */
/*<       angles(1)=acos(mu1) >*/
	    angles[0] = acos(mu1);
/*<       angles(2)=acos(mu2) >*/
	    angles[1] = acos(mu2);
/*<       if (fi.lt.0.) fi=fi+2.*pi >*/
	    if (fi < 0.) {
		fi += pi * 2.;
	    }
/*<       if (fi.gt.(2.*pi)) fi=fi-2.*pi >*/
	    if (fi > pi * 2.) {
		fi -= pi * 2.;
	    }
/*<       angles(3)=fi >*/
	    angles[2] = fi;
/*<       brdfint(j,k)=mvbp1(option,angles,optics,struct,error) >*/
	    brdfint[j + k * brdfint_dim1] = mvbp1_(&option[1], angles, &
		    optics[1], &struct__[1], &error);
/*<       if (error .eq. 101) write (iwr, fmt (6)) errmsg (1) >*/
	    if (error == 101) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 102) write (iwr, fmt (6)) errmsg (2) >*/
	    if (error == 102) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 80, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 103) write (iwr, fmt (6)) errmsg (3) >*/
	    if (error == 103) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 160, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 104) write (iwr, fmt (6)) errmsg (4) >*/
	    if (error == 104) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 240, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 105) write (iwr, fmt (6)) errmsg (5) >*/
	    if (error == 105) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 320, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 201) write (iwr, fmt (6)) errmsg (6) >*/
	    if (error == 201) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 400, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 202) write (iwr, fmt (6)) errmsg (7) >*/
	    if (error == 202) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 480, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 203) write (iwr, fmt (6)) errmsg (8) >*/
	    if (error == 203) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 560, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 301) write (iwr, fmt (6)) errmsg (9) >*/
	    if (error == 301) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 640, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 302) write (iwr, fmt (6)) errmsg (10) >*/
	    if (error == 302) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 720, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 401) write (iwr, fmt (6)) errmsg (11) >*/
	    if (error == 401) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 800, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 402) write (iwr, fmt (6)) errmsg (12) >*/
	    if (error == 402) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 880, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 411) write (iwr, fmt (6)) errmsg (13) >*/
	    if (error == 411) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 960, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 412) write (iwr, fmt (6)) errmsg (14) >*/
	    if (error == 412) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 1040, 80L);
		e_wsfe();
	    }
/*<       if (error .eq. 421) write (iwr, fmt (6)) errmsg (15) >*/
	    if (error == 421) {
		ci__1.cierr = 0;
		ci__1.ciunit = sixs_ier__1.iwr;
		ci__1.cifmt = fmt + 300;
		s_wsfe(&ci__1);
		do_fio(&c__1, errmsg + 1120, 80L);
		e_wsfe();
	    }
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* versbrdf_ */

#ifdef __cplusplus
	}
#endif
