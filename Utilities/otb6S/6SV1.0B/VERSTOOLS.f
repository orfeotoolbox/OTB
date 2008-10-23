         real function mvbp1 (option, angles, optics, struct, error)
c***********************************************************************
c
c  subroutine mvbp1                    version 1.02, 5 apr 1991
c
c  purpose:
c
c  to generate a single bidirectional reflectance value for a semi-
c  infinite medium, given the illumination and viewing geometry, the
c  optical and the structural properties of the scatterers. this model
c  has been used successfully to describe the reflectance of soils and
c  deep vegetation covers.
c
c  argument(s):
c
      integer      option (5), error
      real angles (3), optics (3), struct (4)
c
c  definitions:
c
c     angles       illumination and observation angles:
c        angles (1): illumination zenith angle, in radians
c        angles (2): observation zenith angle, in radians
c        angles (3): relative azimuth angle, in radians
c     error        error code:
c        0:   no error found, normal return.
c        if more than one error condition occurs, only the most recent
c        (usually the most severe) error code is returned.
c        error codes between 100 and 199 indicate warnings: computations
c        are proceeding, but the result may not be what you expected.
c        101: option (1) not an acceptable value, reset to 1
c        102: option (2) not an acceptable value, reset to 1
c        103: option (3) not an acceptable value, reset to 1
c        104: option (4) not an acceptable value, reset to 1
c        105: option (5) not an acceptable value, reset to 1
c        error codes above 200 indicate severe conditions that should
c        not be encountered. the reflectance value mvbp1 is set to -1
c        and the execution is aborted. check your code and usage of this
c        function.
c        201: illumination zenith angle (angles (1)) not an acceptable
c             value, execution aborted
c        202: observation zenith angle (angles (2)) not an acceptable
c             value, execution aborted
c        203: relative azimuth angle (angles (3)) not an acceptable
c             value, execution aborted
c        301: single scattering albedo (optics (1)) not an acceptable
c             value, execution aborted
c        302: phase function parameter (optics (2) not an acceptable
c             value, execution aborted
c        401: leaf area density (struct (1)) not an acceptable value,
c             execution aborted
c        402: radius of sun flecks (struct (2)) not an acceptable value,
c             execution aborted
c        411: given value of kappa1 (struct (3) with option (3) = 0)
c             not an acceptable value, execution aborted
c        412: given value of kappa2 (struct (4) with option (3) = 0)
c             not an acceptable value, execution aborted
c        421: leaf orientation parameter chil (struct (3) with
c             option (3) = 1) not an acceptable value, execution aborted
c     optics       optical characteristics of the scatterers:
c        optics (1): single scattering albedo, n/d value between 0.0
c                    and 1.0
c        optics (2): phase function parameter:
c           if option (4) = 0: this input is not used
c           if option (4) = 1: asymmetry factor, n/d value between -1.0
c              and 1.0
c           if option (4) = 2: first coefficient of legendre polynomial
c        optics (3): phase function parameter:
c           if option (4) = 0: this input is not used
c           if option (4) = 1: this input is not used
c           if option (4) = 2: second coefficient of legendre polynomial
c     option       integer array to select the desired output:
c        option (1): 0 for full theoretical model (see ref. 1)
c                    1 for parameterized model (see ref. 2)
c        option (2): 0 for absolute reflectance, normalized by the
c                      incoming radiant power [j0] (see ref. 1)
c                    1 for relative reflectance, or reflectance factor,
c                      normalized by the incoming radiation on a
c                      horizontal surface [j0 cos(theta1)] and by the
c                      reflectance of a lambertian surface illuminated
c                      and viewed under identical conditions
c                      (see ref. 2)
c        option (3): 0 for given values of kappa (see struct below)
c                    1 for goudriaan's parameterization of kappa
c                    2 for dickinson et al's correction to goudriaan's
c                      parameterization of kappa (see ref. 3)
c        option (4): 0 for isotropic phase function
c                    1 for heyney and greensteins' phase function
c                    2 for legendre polynomial phase function
c        option (5): 0 for single scattering only
c                    1 for dickinson et al. parameterization of multiple
c                      scattering
c     struct       structural parameters of the medium:
c        struct (1): 'leaf area density', in m2 m-3
c        struct (2): radius of the sun flecks on the scatterer, in m
c        struct (3): leaf orientation parameter:
c           if option (3) = 0: struct (3) = kappa1
c           if option (3) = 1 or 2: struct (3) = chil
c        struct (4): leaf orientation parameter:
c           if option (3) = 0: struct (4) = kappa2
c           if option (3) = 1 or 2: this input is not used
c
c  remarks:
c
c  1. warning: all variables in this routine are declared explicitly,
c     and their names have been chosen solely as a function of their
c     meaning. do not assume that a variable is integer or real based
c     on its first letter. the function itself, mvbp1, starts with 'm'
c     but returns a real number. you must therefore declare the mvbp1
c     identifier as both real and external in the calling routine.
c     failure to do so will yield unpredictable results.
c  2. this fortran function provides numerous options. if you specify
c     wrong option values, the function may return unexpected results,
c     issue a warning, or generate an error code and abort. please read
c     carefully the documentation of this routine before using it.
c  3. the error function, needed for the computation of the theoretical
c     value of the bidirectional reflectance, is calculated by calling
c     a function erf (x). this function is assumed to be available on
c     your machine at link time. if this is not the case, append such
c     a function to your code. press et al (1986) provide non-optimized
c     codes for such a routine.
c  4. all computations in mvbp1 are normally made in single precision.
c     if you plan to make extensive use of the full theoretical model
c     (option (1) = 0), you may want to perform all computations in
c     double precision, including erf.
c     warning: the theoretical hot spot function includes the product
c     of a diverging exponential and an erf function that tends to zero.
c     the analytical limit value is finite, but the numerical estimation
c     of this expression may result in a floating point error,
c     especially with single precision variables on 16 bit computers.
c     to avoid this problem, use double precision variables on 32 bit
c     (or higher) computers and efficient algorithms to estimate erf,
c     or use the parametric model, which is not affected by these
c     numerical problems, even on small computers.
c  5. the computer code mvbp1 is intended to achieve the purpose for
c     which it was written, and has been extensively checked. it is
c     not possible, however, to guarantee that it will produce correct
c     results under all possible usages. comments, suggestions and bug
c     reports should be sent to
c
c        dr. michel m. verstraete
c        institute for remote sensing applications
c        cec joint research centre
c        ispra establishment, tp 440
c        i-21020 ispra (va), italy
c
c  6. portability: every effort has been made to use standard fortran 77
c     syntax. this function and the associated subprograms have been
c     developed under microsoft fortran 5.0 on a compaq 386/33l running
c     dos 3.31. the source codes have been successfully ported without
c     changes on an ibm risc 6000 model 520 computer running aix
c     version 3. we would be interested in knowing of your experience
c     in porting this code to other machines or operating systems.
c  7. if you let us know that you are using these codes and keep us
c     informed of your work, we will continue to send you updates of
c     these and related codes under development.
c
c  copyrights: (c) michel m. verstraete and bernard pinty, 1991.
c
c     the copyrights on this and associated computer codes (with the
c     exception of the mathematical routines to compute erf) remain
c     with the authors, drs. michel m. verstraete and bernard pinty.
c  academic users: you are authorized to use this code for your
c     research and teaching, but you must acknowledge use of this
c     routine explicitly and refer to the first two papers below in
c     any publication or work for which you used these codes. you are
c     encouraged to distribute, free of charge, the unmodified version
c     of these codes to colleagues involved in similar activities,
c     provided you include all the in-line documentation and
c     associated files. they, in turn, must agree with and abide by
c     the same rules. you may not sell this code to anybody, and you
c     may not distribute it to commercial interests under any
c     circumstances.
c  commercial and other users: use of this code (mvbp1) in commercial
c     applications is strictly forbidden without a written approval of
c     the authors. even with such an authorization to use the code, you
c     may not distribute or sell it to any other commercial or business
c     partners under any circumstances. please contact us at the above
c     address.
c  warning: the fortran subroutines used to compute erf on the
c     distribution disk for this code are taken from press et al., 1986.
c     please see the preface of that book for further restrictions and
c     clarifications on the use of these mathematical routines.
c
c  disclaimer:
c
c     the authors make no warranty, express or implied, that this
c     routine is free of error, or is consistent with any particular
c     standard of merchantability, or that it will meet your
c     requirements for any particular application. the authors disclaim
c     all liability for direct or consequential damages resulting from
c     your use of this routine.
c
c  references:
c
c  1. verstraete, m. m., b. pinty and r. e. dickinson (1990) a physical
c     model of the bidirectional reflectance of vegetation canopies,
c     part 1: theory, journal of geophysical research, 95, 11755-11765.
c  2. pinty, b., m. m. verstraete and r. e. dickinson (1990) a physical
c     model of the bidirectional reflectance of vegetation canopies,
c     part 2: inversion and validation, journal of geophysical
c     research, 95, 11767-11775.
c  3. dickinson, r. e., b. pinty and m. m. verstraete (1990) relating
c     surface albedos in gcm to remotely sensed data, agricultural and
c     forest meteorology, 52, 109-131.
c  4. pinty, b. and m. m. verstraete (1991) extracting information on
c     surface properties from bidirectional reflectance measurements,
c     journal of geophysical research, 96, 2865-2874.
c  5. press, w. h., et al. (1986) numerical recipes: the art of
c     scientific computing, cambridge university press, 818 pp.
c
c  copies of the first four papers can be obtained from the authors.
c
c  history of releases:
c
c  date     version  notes
c  ====     =======  =====
c  91-01-31  1.0     initial release.
c  91-02-10  1.01    bug fix in the computation of psi1 for kappa.
c  91-04-05  1.02    additional comments and first version released
c                    to potential users.
c
c***********************************************************************
c
c  local declaration(s):
c
      real         asyf2, coef1, coef2, coef4,
     *             cospha, cosphi, geofac, hfun1, hfun2, kappa1,
     *             kappa2, kmkm, mu1, mu2, mulsca, phaang,
     *             phafun, pi, pitwo, pio2me, psi1, psi2, pvg, tante1,
     *             tante2,gotwor, vpg, x1, x2
 
c
c     asyf2        square of the asymmetry factor
c     coefi        miscellaneous coefficients (i = 1, 5)
c        coef1 = 4 / 3 pi
c        coef2 = 1 / 4 pi
c     cospha       cosine of the phase angle
c     cosphi       cosine of the relative azimuth angle
c     geofac       geometric factor g
c     hfuni        h functions for the computation of the multiple
c                  scattering contribution (i = 1, 2)
c     kappai       average cosine of the angle between the normal to
c                  the scatterers and the direction of illumination
c                  (i = 1) or viewing (i = 2)
c     kmkm         kappa1 * mu2 + kappa2 * mu1
c     mui          cosine of the zenith angles (i = 1, 2)
c     mulsca       multiple scattering contribution
c     phaang       phase angle g between the incoming and outgoing
c                  radiation
c     phafun       phase function value
c     pi           mathematical constant 3.14
c     pitwo        2.0 * pi
c     pio2me       pi/2 minus epsilon: a value corresponding to about
c                  88 degrees, used to avoid illumination or viewing
c                  angles too close to the horizon
c     psii         parameters used in the computation of kappai
c                  (i = 1, 2)
c     pvg          parameterized hot spot function
c     tantei       tangent of the illumination and viewing angles
c                  (i = 1, 2)
c     termi        terms of the theoretical hotspot function (i = 1, 2)
c     gotwor       geofac / 2.0 * struct (2)
c     vpg          variable used in computing pvg
c     xi           temporary variables
c
c  data initialization:
c
      data pi     /3.14159265/
      data pitwo  /6.28318531/
      data pio2me /1.57/
      data coef1  /0.42441318/
      data coef2  /7.957747e-2/
c
      error = 0
c
c  verify validity of options:
c
      if ((option (1) .lt. 0) .or. (option (1) .gt. 1)) then
         error = 101
         option (1) = 1
      end if
      if ((option (2) .lt. 0) .or. (option (2) .gt. 1)) then
         error = 102
         option (2) = 1
      end if
      if ((option (3) .lt. 0) .or. (option (3) .gt. 2)) then
         error = 103
         option (3) = 1
      end if
      if ((option (4) .lt. 0) .or. (option (4) .gt. 2)) then
         error = 104
         option (4) = 1
      end if
      if ((option (5) .lt. 0) .or. (option (5) .gt. 1)) then
         error = 105
         option (5) = 1
      end if
c
c  verify validity of angles:
c
      if ((angles (1) .lt. 0.0) .or. (angles (1) .gt. pio2me)) then
         error = 201
      end if
      if ((angles (2) .lt. 0.0) .or. (angles (2) .gt. pio2me)) then
         error = 202
      end if
      if ((angles (3) .lt. 0.0) .or. (angles (3) .gt. pitwo)) then
         error = 203
      end if
c
c  verify validity of optical parameters:
c
      if ((optics (1) .lt. 0.0) .or. (optics (1) .gt. 1.0)) then
         error = 301
      end if
      if (option (4) .eq. 1) then
         if ((optics (2) .lt. -1.0) .or. (optics (2) .gt. 1.0)) then
            error = 302
         end if
      end if
c
c  verify validity of structural parameters:
c
      if (struct (1) .le. 0.0) then
         error = 401
      endif
      if (struct (2) .le. 0.0) then
         error = 402
      endif
      if (option (3) .eq. 0) then
         if ((struct (3) .lt. -1.0) .or. (struct (3) .gt. 1.0)) then
            error = 411
         end if
         if ((struct (4) .lt. -1.0) .or. (struct (4) .gt. 1.0)) then
            error = 412
         end if
      end if
      if ((option (3) .eq. 1) .or. (option (3) .eq. 2)) then
         if ((struct (3) .lt. -0.4) .or. (struct (3) .gt. 0.6)) then
            error = 421
         end if
      end if
c
      if (error .ge. 200) then
         mvbp1 = -1.0
         return
      end if
c
c  compute various trigonometric expressions:
c
      mu1 = cos (angles (1))
      mu2 = cos (angles (2))
      cosphi = cos (angles (3))
      cospha = mu1 * mu2 + sin (angles (1)) * sin (angles (2)) * cosphi
      phaang = acos (cospha)
      tante1 = tan (angles (1))
      tante2 = tan (angles (2))
      geofac = sqrt (tante1 * tante1 + tante2 * tante2 -
     *   2.0 * tante1 * tante2 * cosphi)
c
c  compute the leaf orientation parameters kappa:
c
      if (option (3) .eq. 0) then
         kappa1 = struct (3)
         kappa2 = struct (4)
      endif
      if (option (3) .eq. 1) then
         psi1 = 0.5 - (0.6333 + 0.33 * struct (3)) * struct (3)
         psi2 = 0.877 * (1.0 - 2.0 * psi1)
         kappa1 = psi1 + psi2 * mu1
         kappa2 = psi1 + psi2 * mu2
      endif
      if (option (3) .eq. 2) then
         psi1 = 0.5 - (0.489 + 0.11 * struct (3)) * struct (3)
         psi2 = 1.0 * (1.0 - 2.0 * psi1)
         kappa1 = psi1 + psi2 * mu1
         kappa2 = psi1 + psi2 * mu2
      endif
c
c  compute the phase function:
c
      if (option (4) .eq. 0) then
         phafun = 1.0
      endif
      if (option (4) .eq. 1) then
         asyf2 = optics (2) * optics (2)
         x1 = (1.0 + asyf2 - 2.0 * optics (2) * cos (pi - phaang))
         phafun = (1.0 - asyf2) / (x1 ** 1.5)
      endif
      if (option (4) .eq. 2) then
         phafun = 1.0 + optics (2) * cospha +
     *      optics (3) * (0.5 * (3.0 * cospha * cospha - 1.0))
      endif
c
c  compute the opposition (hot spot) function:
c
      kmkm = kappa1 * mu2 + kappa2 * mu1
      gotwor = geofac / (2.0 * struct (2))
c
c  equation (52) of reference (1) above: case impossible
c
c     if (option (1) .eq. 0) then
c        cofa = coef3 * kappa2 * struct (1) / mu2
c        cofb = (kappa1 * struct (1)) / (2.0 * mu1)
c        term1 = (0.5 * struct (1) * kmkm) / (mu1 * mu2)
c        term1 = term1 * sqrt (pi / cofa) * exp (cofb * cofb / cofa)
c        x2 = cofb / sqrt (cofa)
c        x1 = (sqrt (cofa) / gotwor) + x2
c        term1 = term1 * (erf (x1) - erf (x2))
c        x1 = (coef1 / gotwor) * (kappa2 * struct (1) / mu2)
c        x2 = -struct (1) * kmkm / (mu1 * mu2 * gotwor)
c        term2 = exp (x1) * exp (x2)
c        pvg = term1 + term2
c     endif
c
c  equation (6) of reference (2) above:
c
      if (option (1) .eq. 1) then
         vpg = 4.0 * (1.0 - coef1) * mu2 * gotwor /
     *      (struct (1) * kappa2)
         pvg = 1.0 + (1.0 / (1.0 + vpg))
      endif
c
c  compute the multiple scattering contribution:
c
      if (option (5) .eq. 0) then
         mulsca = 0.0
      else
         x1 = mu1 / kappa1
         x2 = mu2 / kappa2
         hfun1 = (1.0 + x1) / (1.0 + x1 * sqrt (1.0 - optics (1)))
         hfun2 = (1.0 + x2) / (1.0 + x2 * sqrt (1.0 - optics (1)))
         mulsca = hfun1 * hfun2 - 1.0
      endif
c
c  compute the bidirectional reflectance:
c
      if (option (2) .eq. 0) then
         coef4 = coef2 * optics (1) * kappa1 * mu1 / kmkm
      endif
      if (option (2) .eq. 1) then
         coef4 = 0.25 * optics (1) * kappa1 / kmkm
      endif
      mvbp1 = coef4 * (pvg * phafun + mulsca)
c
      return
      end
