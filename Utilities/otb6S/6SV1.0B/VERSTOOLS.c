/* VERSTOOLS.f -- translated by f2c (version 19970805).
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

static doublereal c_b2 = 1.5;

/*<          real function mvbp1 (option, angles, optics, struct, error) >*/
doublereal mvbp1_(integer *option, doublereal *angles, doublereal *optics, 
	doublereal *struct__, integer *error)
{
    /* Initialized data */

    static doublereal pi = 3.14159265;
    static doublereal pitwo = 6.28318531;
    static doublereal pio2me = 1.57;
    static doublereal coef1 = .42441318;
    static doublereal coef2 = .07957747;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal), acos(doublereal), tan(doublereal)
	    , sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    doublereal x1, x2, mu1, mu2, pvg, vpg, psi1, psi2, kmkm, coef4, hfun1, 
	    hfun2, asyf2, kappa1, kappa2, tante1, tante2, geofac, phaang, 
	    cospha, phafun, mulsca, cosphi, gotwor;

/* ***********************************************************************
 */

/*  subroutine mvbp1                    version 1.02, 5 apr 1991 */

/*  purpose: */

/*  to generate a single bidirectional reflectance value for a semi- */
/*  infinite medium, given the illumination and viewing geometry, the */
/*  optical and the structural properties of the scatterers. this model */
/*  has been used successfully to describe the reflectance of soils and */
/*  deep vegetation covers. */

/*  argument(s): */

/*<       integer      option (5), error >*/
/*<       real angles (3), optics (3), struct (4) >*/

/*  definitions: */

/*     angles       illumination and observation angles: */
/*        angles (1): illumination zenith angle, in radians */
/*        angles (2): observation zenith angle, in radians */
/*        angles (3): relative azimuth angle, in radians */
/*     error        error code: */
/*        0:   no error found, normal return. */
/*        if more than one error condition occurs, only the most recent */
/*        (usually the most severe) error code is returned. */
/*        error codes between 100 and 199 indicate warnings: computations 
*/
/*        are proceeding, but the result may not be what you expected. */
/*        101: option (1) not an acceptable value, reset to 1 */
/*        102: option (2) not an acceptable value, reset to 1 */
/*        103: option (3) not an acceptable value, reset to 1 */
/*        104: option (4) not an acceptable value, reset to 1 */
/*        105: option (5) not an acceptable value, reset to 1 */
/*        error codes above 200 indicate severe conditions that should */
/*        not be encountered. the reflectance value mvbp1 is set to -1 */
/*        and the execution is aborted. check your code and usage of this 
*/
/*        function. */
/*        201: illumination zenith angle (angles (1)) not an acceptable */
/*             value, execution aborted */
/*        202: observation zenith angle (angles (2)) not an acceptable */
/*             value, execution aborted */
/*        203: relative azimuth angle (angles (3)) not an acceptable */
/*             value, execution aborted */
/*        301: single scattering albedo (optics (1)) not an acceptable */
/*             value, execution aborted */
/*        302: phase function parameter (optics (2) not an acceptable */
/*             value, execution aborted */
/*        401: leaf area density (struct (1)) not an acceptable value, */
/*             execution aborted */
/*        402: radius of sun flecks (struct (2)) not an acceptable value, 
*/
/*             execution aborted */
/*        411: given value of kappa1 (struct (3) with option (3) = 0) */
/*             not an acceptable value, execution aborted */
/*        412: given value of kappa2 (struct (4) with option (3) = 0) */
/*             not an acceptable value, execution aborted */
/*        421: leaf orientation parameter chil (struct (3) with */
/*             option (3) = 1) not an acceptable value, execution aborted 
*/
/*     optics       optical characteristics of the scatterers: */
/*        optics (1): single scattering albedo, n/d value between 0.0 */
/*                    and 1.0 */
/*        optics (2): phase function parameter: */
/*           if option (4) = 0: this input is not used */
/*           if option (4) = 1: asymmetry factor, n/d value between -1.0 
*/
/*              and 1.0 */
/*           if option (4) = 2: first coefficient of legendre polynomial 
*/
/*        optics (3): phase function parameter: */
/*           if option (4) = 0: this input is not used */
/*           if option (4) = 1: this input is not used */
/*           if option (4) = 2: second coefficient of legendre polynomial 
*/
/*     option       integer array to select the desired output: */
/*        option (1): 0 for full theoretical model (see ref. 1) */
/*                    1 for parameterized model (see ref. 2) */
/*        option (2): 0 for absolute reflectance, normalized by the */
/*                      incoming radiant power [j0] (see ref. 1) */
/*                    1 for relative reflectance, or reflectance factor, 
*/
/*                      normalized by the incoming radiation on a */
/*                      horizontal surface [j0 cos(theta1)] and by the */
/*                      reflectance of a lambertian surface illuminated */
/*                      and viewed under identical conditions */
/*                      (see ref. 2) */
/*        option (3): 0 for given values of kappa (see struct below) */
/*                    1 for goudriaan's parameterization of kappa */
/*                    2 for dickinson et al's correction to goudriaan's */
/*                      parameterization of kappa (see ref. 3) */
/*        option (4): 0 for isotropic phase function */
/*                    1 for heyney and greensteins' phase function */
/*                    2 for legendre polynomial phase function */
/*        option (5): 0 for single scattering only */
/*                    1 for dickinson et al. parameterization of multiple 
*/
/*                      scattering */
/*     struct       structural parameters of the medium: */
/*        struct (1): 'leaf area density', in m2 m-3 */
/*        struct (2): radius of the sun flecks on the scatterer, in m */
/*        struct (3): leaf orientation parameter: */
/*           if option (3) = 0: struct (3) = kappa1 */
/*           if option (3) = 1 or 2: struct (3) = chil */
/*        struct (4): leaf orientation parameter: */
/*           if option (3) = 0: struct (4) = kappa2 */
/*           if option (3) = 1 or 2: this input is not used */

/*  remarks: */

/*  1. warning: all variables in this routine are declared explicitly, */
/*     and their names have been chosen solely as a function of their */
/*     meaning. do not assume that a variable is integer or real based */
/*     on its first letter. the function itself, mvbp1, starts with 'm' */
/*     but returns a real number. you must therefore declare the mvbp1 */
/*     identifier as both real and external in the calling routine. */
/*     failure to do so will yield unpredictable results. */
/*  2. this fortran function provides numerous options. if you specify */
/*     wrong option values, the function may return unexpected results, */
/*     issue a warning, or generate an error code and abort. please read 
*/
/*     carefully the documentation of this routine before using it. */
/*  3. the error function, needed for the computation of the theoretical 
*/
/*     value of the bidirectional reflectance, is calculated by calling */
/*     a function erf (x). this function is assumed to be available on */
/*     your machine at link time. if this is not the case, append such */
/*     a function to your code. press et al (1986) provide non-optimized 
*/
/*     codes for such a routine. */
/*  4. all computations in mvbp1 are normally made in single precision. */
/*     if you plan to make extensive use of the full theoretical model */
/*     (option (1) = 0), you may want to perform all computations in */
/*     double precision, including erf. */
/*     warning: the theoretical hot spot function includes the product */
/*     of a diverging exponential and an erf function that tends to zero. 
*/
/*     the analytical limit value is finite, but the numerical estimation 
*/
/*     of this expression may result in a floating point error, */
/*     especially with single precision variables on 16 bit computers. */
/*     to avoid this problem, use double precision variables on 32 bit */
/*     (or higher) computers and efficient algorithms to estimate erf, */
/*     or use the parametric model, which is not affected by these */
/*     numerical problems, even on small computers. */
/*  5. the computer code mvbp1 is intended to achieve the purpose for */
/*     which it was written, and has been extensively checked. it is */
/*     not possible, however, to guarantee that it will produce correct */
/*     results under all possible usages. comments, suggestions and bug */
/*     reports should be sent to */

/*        dr. michel m. verstraete */
/*        institute for remote sensing applications */
/*        cec joint research centre */
/*        ispra establishment, tp 440 */
/*        i-21020 ispra (va), italy */

/*  6. portability: every effort has been made to use standard fortran 77 
*/
/*     syntax. this function and the associated subprograms have been */
/*     developed under microsoft fortran 5.0 on a compaq 386/33l running 
*/
/*     dos 3.31. the source codes have been successfully ported without */
/*     changes on an ibm risc 6000 model 520 computer running aix */
/*     version 3. we would be interested in knowing of your experience */
/*     in porting this code to other machines or operating systems. */
/*  7. if you let us know that you are using these codes and keep us */
/*     informed of your work, we will continue to send you updates of */
/*     these and related codes under development. */

/*  copyrights: (c) michel m. verstraete and bernard pinty, 1991. */

/*     the copyrights on this and associated computer codes (with the */
/*     exception of the mathematical routines to compute erf) remain */
/*     with the authors, drs. michel m. verstraete and bernard pinty. */
/*  academic users: you are authorized to use this code for your */
/*     research and teaching, but you must acknowledge use of this */
/*     routine explicitly and refer to the first two papers below in */
/*     any publication or work for which you used these codes. you are */
/*     encouraged to distribute, free of charge, the unmodified version */
/*     of these codes to colleagues involved in similar activities, */
/*     provided you include all the in-line documentation and */
/*     associated files. they, in turn, must agree with and abide by */
/*     the same rules. you may not sell this code to anybody, and you */
/*     may not distribute it to commercial interests under any */
/*     circumstances. */
/*  commercial and other users: use of this code (mvbp1) in commercial */
/*     applications is strictly forbidden without a written approval of */
/*     the authors. even with such an authorization to use the code, you 
*/
/*     may not distribute or sell it to any other commercial or business 
*/
/*     partners under any circumstances. please contact us at the above */
/*     address. */
/*  warning: the fortran subroutines used to compute erf on the */
/*     distribution disk for this code are taken from press et al., 1986. 
*/
/*     please see the preface of that book for further restrictions and */
/*     clarifications on the use of these mathematical routines. */

/*  disclaimer: */

/*     the authors make no warranty, express or implied, that this */
/*     routine is free of error, or is consistent with any particular */
/*     standard of merchantability, or that it will meet your */
/*     requirements for any particular application. the authors disclaim 
*/
/*     all liability for direct or consequential damages resulting from */
/*     your use of this routine. */

/*  references: */

/*  1. verstraete, m. m., b. pinty and r. e. dickinson (1990) a physical 
*/
/*     model of the bidirectional reflectance of vegetation canopies, */
/*     part 1: theory, journal of geophysical research, 95, 11755-11765. 
*/
/*  2. pinty, b., m. m. verstraete and r. e. dickinson (1990) a physical 
*/
/*     model of the bidirectional reflectance of vegetation canopies, */
/*     part 2: inversion and validation, journal of geophysical */
/*     research, 95, 11767-11775. */
/*  3. dickinson, r. e., b. pinty and m. m. verstraete (1990) relating */
/*     surface albedos in gcm to remotely sensed data, agricultural and */
/*     forest meteorology, 52, 109-131. */
/*  4. pinty, b. and m. m. verstraete (1991) extracting information on */
/*     surface properties from bidirectional reflectance measurements, */
/*     journal of geophysical research, 96, 2865-2874. */
/*  5. press, w. h., et al. (1986) numerical recipes: the art of */
/*     scientific computing, cambridge university press, 818 pp. */

/*  copies of the first four papers can be obtained from the authors. */

/*  history of releases: */

/*  date     version  notes */
/*  ====     =======  ===== */
/*  91-01-31  1.0     initial release. */
/*  91-02-10  1.01    bug fix in the computation of psi1 for kappa. */
/*  91-04-05  1.02    additional comments and first version released */
/*                    to potential users. */

/* ***********************************************************************
 */

/*  local declaration(s): */

/*<    >*/

/*     asyf2        square of the asymmetry factor */
/*     coefi        miscellaneous coefficients (i = 1, 5) */
/*        coef1 = 4 / 3 pi */
/*        coef2 = 1 / 4 pi */
/*     cospha       cosine of the phase angle */
/*     cosphi       cosine of the relative azimuth angle */
/*     geofac       geometric factor g */
/*     hfuni        h functions for the computation of the multiple */
/*                  scattering contribution (i = 1, 2) */
/*     kappai       average cosine of the angle between the normal to */
/*                  the scatterers and the direction of illumination */
/*                  (i = 1) or viewing (i = 2) */
/*     kmkm         kappa1 * mu2 + kappa2 * mu1 */
/*     mui          cosine of the zenith angles (i = 1, 2) */
/*     mulsca       multiple scattering contribution */
/*     phaang       phase angle g between the incoming and outgoing */
/*                  radiation */
/*     phafun       phase function value */
/*     pi           mathematical constant 3.14 */
/*     pitwo        2.0 * pi */
/*     pio2me       pi/2 minus epsilon: a value corresponding to about */
/*                  88 degrees, used to avoid illumination or viewing */
/*                  angles too close to the horizon */
/*     psii         parameters used in the computation of kappai */
/*                  (i = 1, 2) */
/*     pvg          parameterized hot spot function */
/*     tantei       tangent of the illumination and viewing angles */
/*                  (i = 1, 2) */
/*     termi        terms of the theoretical hotspot function (i = 1, 2) 
*/
/*     gotwor       geofac / 2.0 * struct (2) */
/*     vpg          variable used in computing pvg */
/*     xi           temporary variables */

/*  data initialization: */

/*<       data pi     /3.14159265/ >*/
    /* Parameter adjustments */
    --struct__;
    --optics;
    --angles;
    --option;

    /* Function Body */
/*<       data pitwo  /6.28318531/ >*/
/*<       data pio2me /1.57/ >*/
/*<       data coef1  /0.42441318/ >*/
/*<       data coef2  /7.957747e-2/ >*/

/*<       error = 0 >*/
    *error = 0;

/*  verify validity of options: */

/*<       if ((option (1) .lt. 0) .or. (option (1) .gt. 1)) then >*/
    if (option[1] < 0 || option[1] > 1) {
/*<          error = 101 >*/
	*error = 101;
/*<          option (1) = 1 >*/
	option[1] = 1;
/*<       end if >*/
    }
/*<       if ((option (2) .lt. 0) .or. (option (2) .gt. 1)) then >*/
    if (option[2] < 0 || option[2] > 1) {
/*<          error = 102 >*/
	*error = 102;
/*<          option (2) = 1 >*/
	option[2] = 1;
/*<       end if >*/
    }
/*<       if ((option (3) .lt. 0) .or. (option (3) .gt. 2)) then >*/
    if (option[3] < 0 || option[3] > 2) {
/*<          error = 103 >*/
	*error = 103;
/*<          option (3) = 1 >*/
	option[3] = 1;
/*<       end if >*/
    }
/*<       if ((option (4) .lt. 0) .or. (option (4) .gt. 2)) then >*/
    if (option[4] < 0 || option[4] > 2) {
/*<          error = 104 >*/
	*error = 104;
/*<          option (4) = 1 >*/
	option[4] = 1;
/*<       end if >*/
    }
/*<       if ((option (5) .lt. 0) .or. (option (5) .gt. 1)) then >*/
    if (option[5] < 0 || option[5] > 1) {
/*<          error = 105 >*/
	*error = 105;
/*<          option (5) = 1 >*/
	option[5] = 1;
/*<       end if >*/
    }

/*  verify validity of angles: */

/*<       if ((angles (1) .lt. 0.0) .or. (angles (1) .gt. pio2me)) then >*/
    if (angles[1] < 0. || angles[1] > pio2me) {
/*<          error = 201 >*/
	*error = 201;
/*<       end if >*/
    }
/*<       if ((angles (2) .lt. 0.0) .or. (angles (2) .gt. pio2me)) then >*/
    if (angles[2] < 0. || angles[2] > pio2me) {
/*<          error = 202 >*/
	*error = 202;
/*<       end if >*/
    }
/*<       if ((angles (3) .lt. 0.0) .or. (angles (3) .gt. pitwo)) then >*/
    if (angles[3] < 0. || angles[3] > pitwo) {
/*<          error = 203 >*/
	*error = 203;
/*<       end if >*/
    }

/*  verify validity of optical parameters: */

/*<       if ((optics (1) .lt. 0.0) .or. (optics (1) .gt. 1.0)) then >*/
    if (optics[1] < 0. || optics[1] > 1.) {
/*<          error = 301 >*/
	*error = 301;
/*<       end if >*/
    }
/*<       if (option (4) .eq. 1) then >*/
    if (option[4] == 1) {
/*<          if ((optics (2) .lt. -1.0) .or. (optics (2) .gt. 1.0)) then >*/
	if (optics[2] < -1. || optics[2] > 1.) {
/*<             error = 302 >*/
	    *error = 302;
/*<          end if >*/
	}
/*<       end if >*/
    }

/*  verify validity of structural parameters: */

/*<       if (struct (1) .le. 0.0) then >*/
    if (struct__[1] <= 0.) {
/*<          error = 401 >*/
	*error = 401;
/*<       endif >*/
    }
/*<       if (struct (2) .le. 0.0) then >*/
    if (struct__[2] <= 0.) {
/*<          error = 402 >*/
	*error = 402;
/*<       endif >*/
    }
/*<       if (option (3) .eq. 0) then >*/
    if (option[3] == 0) {
/*<          if ((struct (3) .lt. -1.0) .or. (struct (3) .gt. 1.0)) then >*/
	if (struct__[3] < -1. || struct__[3] > 1.) {
/*<             error = 411 >*/
	    *error = 411;
/*<          end if >*/
	}
/*<          if ((struct (4) .lt. -1.0) .or. (struct (4) .gt. 1.0)) then >*/
	if (struct__[4] < -1. || struct__[4] > 1.) {
/*<             error = 412 >*/
	    *error = 412;
/*<          end if >*/
	}
/*<       end if >*/
    }
/*<       if ((option (3) .eq. 1) .or. (option (3) .eq. 2)) then >*/
    if (option[3] == 1 || option[3] == 2) {
/*<          if ((struct (3) .lt. -0.4) .or. (struct (3) .gt. 0.6)) then >*/
	if (struct__[3] < -.4 || struct__[3] > .6) {
/*<             error = 421 >*/
	    *error = 421;
/*<          end if >*/
	}
/*<       end if >*/
    }

/*<       if (error .ge. 200) then >*/
    if (*error >= 200) {
/*<          mvbp1 = -1.0 >*/
	ret_val = -1.;
/*<          return >*/
	return ret_val;
/*<       end if >*/
    }

/*  compute various trigonometric expressions: */

/*<       mu1 = cos (angles (1)) >*/
    mu1 = cos(angles[1]);
/*<       mu2 = cos (angles (2)) >*/
    mu2 = cos(angles[2]);
/*<       cosphi = cos (angles (3)) >*/
    cosphi = cos(angles[3]);
/*<       cospha = mu1 * mu2 + sin (angles (1)) * sin (angles (2)) * cosphi >*/
    cospha = mu1 * mu2 + sin(angles[1]) * sin(angles[2]) * cosphi;
/*<       phaang = acos (cospha) >*/
    phaang = acos(cospha);
/*<       tante1 = tan (angles (1)) >*/
    tante1 = tan(angles[1]);
/*<       tante2 = tan (angles (2)) >*/
    tante2 = tan(angles[2]);
/*<    >*/
    geofac = sqrt(tante1 * tante1 + tante2 * tante2 - tante1 * 2. * tante2 * 
	    cosphi);

/*  compute the leaf orientation parameters kappa: */

/*<       if (option (3) .eq. 0) then >*/
    if (option[3] == 0) {
/*<          kappa1 = struct (3) >*/
	kappa1 = struct__[3];
/*<          kappa2 = struct (4) >*/
	kappa2 = struct__[4];
/*<       endif >*/
    }
/*<       if (option (3) .eq. 1) then >*/
    if (option[3] == 1) {
/*<          psi1 = 0.5 - (0.6333 + 0.33 * struct (3)) * struct (3) >*/
	psi1 = .5 - (struct__[3] * .33 + .6333) * struct__[3];
/*<          psi2 = 0.877 * (1.0 - 2.0 * psi1) >*/
	psi2 = (1. - psi1 * 2.) * .877;
/*<          kappa1 = psi1 + psi2 * mu1 >*/
	kappa1 = psi1 + psi2 * mu1;
/*<          kappa2 = psi1 + psi2 * mu2 >*/
	kappa2 = psi1 + psi2 * mu2;
/*<       endif >*/
    }
/*<       if (option (3) .eq. 2) then >*/
    if (option[3] == 2) {
/*<          psi1 = 0.5 - (0.489 + 0.11 * struct (3)) * struct (3) >*/
	psi1 = .5 - (struct__[3] * .11 + .489) * struct__[3];
/*<          psi2 = 1.0 * (1.0 - 2.0 * psi1) >*/
	psi2 = (1. - psi1 * 2.) * 1.;
/*<          kappa1 = psi1 + psi2 * mu1 >*/
	kappa1 = psi1 + psi2 * mu1;
/*<          kappa2 = psi1 + psi2 * mu2 >*/
	kappa2 = psi1 + psi2 * mu2;
/*<       endif >*/
    }

/*  compute the phase function: */

/*<       if (option (4) .eq. 0) then >*/
    if (option[4] == 0) {
/*<          phafun = 1.0 >*/
	phafun = 1.;
/*<       endif >*/
    }
/*<       if (option (4) .eq. 1) then >*/
    if (option[4] == 1) {
/*<          asyf2 = optics (2) * optics (2) >*/
	asyf2 = optics[2] * optics[2];
/*<          x1 = (1.0 + asyf2 - 2.0 * optics (2) * cos (pi - phaang)) >*/
	x1 = asyf2 + 1. - optics[2] * 2. * cos(pi - phaang);
/*<          phafun = (1.0 - asyf2) / (x1 ** 1.5) >*/
	phafun = (1. - asyf2) / pow_dd(&x1, &c_b2);
/*<       endif >*/
    }
/*<       if (option (4) .eq. 2) then >*/
    if (option[4] == 2) {
/*<    >*/
	phafun = optics[2] * cospha + 1. + optics[3] * ((cospha * 3. * cospha 
		- 1.) * .5);
/*<       endif >*/
    }

/*  compute the opposition (hot spot) function: */

/*<       kmkm = kappa1 * mu2 + kappa2 * mu1 >*/
    kmkm = kappa1 * mu2 + kappa2 * mu1;
/*<       gotwor = geofac / (2.0 * struct (2)) >*/
    gotwor = geofac / (struct__[2] * 2.);

/*  equation (52) of reference (1) above: case impossible */

/*     if (option (1) .eq. 0) then */
/*        cofa = coef3 * kappa2 * struct (1) / mu2 */
/*        cofb = (kappa1 * struct (1)) / (2.0 * mu1) */
/*        term1 = (0.5 * struct (1) * kmkm) / (mu1 * mu2) */
/*        term1 = term1 * sqrt (pi / cofa) * exp (cofb * cofb / cofa) */
/*        x2 = cofb / sqrt (cofa) */
/*        x1 = (sqrt (cofa) / gotwor) + x2 */
/*        term1 = term1 * (erf (x1) - erf (x2)) */
/*        x1 = (coef1 / gotwor) * (kappa2 * struct (1) / mu2) */
/*        x2 = -struct (1) * kmkm / (mu1 * mu2 * gotwor) */
/*        term2 = exp (x1) * exp (x2) */
/*        pvg = term1 + term2 */
/*     endif */

/*  equation (6) of reference (2) above: */

/*<       if (option (1) .eq. 1) then >*/
    if (option[1] == 1) {
/*<    >*/
	vpg = (1. - coef1) * 4. * mu2 * gotwor / (struct__[1] * kappa2);
/*<          pvg = 1.0 + (1.0 / (1.0 + vpg)) >*/
	pvg = 1. / (vpg + 1.) + 1.;
/*<       endif >*/
    }

/*  compute the multiple scattering contribution: */

/*<       if (option (5) .eq. 0) then >*/
    if (option[5] == 0) {
/*<          mulsca = 0.0 >*/
	mulsca = 0.;
/*<       else >*/
    } else {
/*<          x1 = mu1 / kappa1 >*/
	x1 = mu1 / kappa1;
/*<          x2 = mu2 / kappa2 >*/
	x2 = mu2 / kappa2;
/*<          hfun1 = (1.0 + x1) / (1.0 + x1 * sqrt (1.0 - optics (1))) >*/
	hfun1 = (x1 + 1.) / (x1 * sqrt(1. - optics[1]) + 1.);
/*<          hfun2 = (1.0 + x2) / (1.0 + x2 * sqrt (1.0 - optics (1))) >*/
	hfun2 = (x2 + 1.) / (x2 * sqrt(1. - optics[1]) + 1.);
/*<          mulsca = hfun1 * hfun2 - 1.0 >*/
	mulsca = hfun1 * hfun2 - 1.;
/*<       endif >*/
    }

/*  compute the bidirectional reflectance: */

/*<       if (option (2) .eq. 0) then >*/
    if (option[2] == 0) {
/*<          coef4 = coef2 * optics (1) * kappa1 * mu1 / kmkm >*/
	coef4 = coef2 * optics[1] * kappa1 * mu1 / kmkm;
/*<       endif >*/
    }
/*<       if (option (2) .eq. 1) then >*/
    if (option[2] == 1) {
/*<          coef4 = 0.25 * optics (1) * kappa1 / kmkm >*/
	coef4 = optics[1] * .25 * kappa1 / kmkm;
/*<       endif >*/
    }
/*<       mvbp1 = coef4 * (pvg * phafun + mulsca) >*/
    ret_val = coef4 * (pvg * phafun + mulsca);

/*<       return >*/
    return ret_val;
/*<       end >*/
} /* mvbp1_ */

#ifdef __cplusplus
	}
#endif
