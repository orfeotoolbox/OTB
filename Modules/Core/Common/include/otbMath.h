/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbMath_h
#define __otbMath_h

/**
 * otbMath.h defines standard math macros, constants, and other
 * parameters.
 * All mathematics constans, functions (used in the OTB code) are the vcl version (vcl..., CONST_PI, ).
 */

#include "vcl_cmath.h"

namespace otb {

// For Windows MSVC, defines these macros.
#ifdef _USE_MATH_DEFINES
#define _MATH_DEFINES_DEFINED
#endif

/** Define constants if not define in the "vcl_cmath.h" */

/* Some useful constants.  */
const double CONST_E =        2.7182818284590452354;   /* e */
const double CONST_LOG2E =    1.4426950408889634074;   /* log_2 e */
const double CONST_LOG10E =   0.43429448190325182765;  /* log_10 e */
const double CONST_LN2 =      0.69314718055994530942;  /* log_e 2 */
const double CONST_LN10 =     2.30258509299404568402;  /* log_e 10 */
const double CONST_PI =       3.14159265358979323846;  /* pi */
const double CONST_PI_2 =     1.57079632679489661923;  /* pi/2 */
const double CONST_PI_4 =     0.78539816339744830962;  /* pi/4 */
const double CONST_PI_8 =     0.39269908169872415481;  /* pi/8 */
const double CONST_1_PI =     0.31830988618379067154;  /* 1/pi */
const double CONST_2_PI =     0.63661977236758134308;  /* 2/pi */
const double CONST_2PI =      6.28318530717958647693;  /* 2*pi */
const double CONST_PI_180 =   0.01745329251994329577;  /* pi/180 */
const double CONST_180_PI =   57.2957795130823208768;  /* 180/pi */
const double CONST_LNPI =     std::log(CONST_PI);      /* ln(pi)*/
const double CONST_SQRTPI =   std::sqrt(CONST_PI);     /* sqrt(pi)*/
const double CONST_2_SQRTPI = 1.12837916709551257390;  /* 2/sqrt(pi) */
const double CONST_SQRT2 =    1.41421356237309504880;  /* sqrt(2) */
const double CONST_SQRT1_2 =  0.70710678118654752440;  /* 1/sqrt(2) */
const double CONST_SQRT3 =    1.73205080756887729353;  /* sqrt(3) */
const double CONST_EULER =    0.57721566490153286060;  /* euler constant*/

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#ifdef __USE_GNU

const long double CONST_El =        2.7182818284590452353602874713526625L;  /* e */
const long double CONST_LOG2El =    1.4426950408889634073599246810018921L;  /* log_2 e */
const long double CONST_LOG10El =   0.4342944819032518276511289189166051L;  /* log_10 e */
const long double CONST_LN2l =      0.6931471805599453094172321214581766L;  /* log_e 2 */
const long double CONST_LN10l =     2.3025850929940456840179914546843642L;  /* log_e 10 */
const long double CONST_PIl =       3.1415926535897932384626433832795029L;  /* pi */
const long double CONST_PI_2l =     1.5707963267948966192313216916397514L;  /* pi/2 */
const long double CONST_PI_4l =     0.7853981633974483096156608458198757L;  /* pi/4 */
const long double CONST_PI_8l =     0.3926990816987241548078304229099379L;  /* pi/8 */
const long double CONST_2_PIl =     0.6366197723675813430755350534900574L;  /* 2/pi */
const long double CONST_2_SQRTPIl = 1.1283791670955125738961589031215452L;  /* 2/sqrt(pi) */
const long double CONST_2PIl =      6.2831853071795864769252867665590058L;  /* 2*pi */
const long double CONST_PI_180l =   0.0174532925199432957692369076848861L;  /* pi/180 */
const long double CONST_180_PIl =   57.295779513082320876798154814105170L;  /* 180/pi */
const long double CONST_LNPIl =     std::log(CONST_PIl);                    /* ln(pi)*/
const long double CONST_SQRTPIl =   std::sqrt(CONST_PIl);                   /* sqrt(pi)*/
const long double CONST_SQRT2l =    1.4142135623730950488016887242096981L;  /* sqrt(2) */
const long double CONST_SQRT1_2l =  0.7071067811865475244008443621048490L;  /* 1/sqrt(2) */
const long double CONST_SQRT3l =    1.7320508075688772935274463415058724L;  /* sqrt(3) */
const long double CONST_EULERl =    0.5772156649015328606065120900824024L;  /* euler constant*/

#endif

template <typename T> T InverseValue(const T & value)
{
  return ((value > 0 || value > 0) ? 1/value : 0);
}

template <typename T> T SignOfValue(const T & value)
{
  return (value > 0) ? 1 : ((value < 0) ? -1 : 0);
}

}

#endif
