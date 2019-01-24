/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbMath_h
#define otbMath_h

/**
 * otbMath.h defines standard math macros, constants, and other
 * parameters.
 * All mathematics constans, functions (used in the OTB code) are the vcl version (vcl..., CONST_PI, ).
 */

#include <cmath>

namespace otb {

// For Windows MSVC, defines these macros.
#ifdef _USE_MATH_DEFINES
#define _MATH_DEFINES_DEFINED
#endif

/** Define constants if not define in the "cmath" */

/* Some useful constants.  */
constexpr double CONST_E        = 2.7182818284590452354;  /* e */
constexpr double CONST_LOG2E    = 1.4426950408889634074;  /* log_2 e */
constexpr double CONST_LOG10E   = 0.43429448190325182765; /* log_10 e */
constexpr double CONST_LN2      = 0.69314718055994530942; /* log_e 2 */
constexpr double CONST_LN10     = 2.30258509299404568402; /* log_e 10 */
constexpr double CONST_PI       = 3.14159265358979323846; /* pi */
constexpr double CONST_PI_2     = CONST_PI / 2.0;         /* pi/2 */
constexpr double CONST_PI_4     = CONST_PI / 4.0;         /* pi/4 */
constexpr double CONST_PI_8     = CONST_PI / 8.0;         /* pi/8 */
constexpr double CONST_1_PI     = 1.0 / CONST_PI;         /* 1/pi */
constexpr double CONST_2_PI     = 2.0 / CONST_PI;         /* 2/pi */
constexpr double CONST_2PI      = 2.0 * CONST_PI;         /* 2*pi */
constexpr double CONST_PI_180   = CONST_PI / 180.0;       /* pi/180 */
constexpr double CONST_180_PI   = 180.0 / CONST_PI;       /* 180/pi */
const double     CONST_LNPI     = std::log(CONST_PI);     /* ln(pi)*/
const double     CONST_SQRTPI   = std::sqrt(CONST_PI);    /* sqrt(pi)*/
constexpr double CONST_2_SQRTPI = 1.12837916709551257390; /* 2/sqrt(pi) */
constexpr double CONST_SQRT2    = 1.41421356237309504880; /* sqrt(2) */
constexpr double CONST_SQRT1_2  = 0.70710678118654752440; /* 1/sqrt(2) */
constexpr double CONST_SQRT3    = 1.73205080756887729353; /* sqrt(3) */
constexpr double CONST_EULER    = 0.57721566490153286060; /* euler constant*/

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#ifdef __USE_GNU

constexpr long double CONST_El        = 2.7182818284590452353602874713526625L; /* e */
constexpr long double CONST_LOG2El    = 1.4426950408889634073599246810018921L; /* log_2 e */
constexpr long double CONST_LOG10El   = 0.4342944819032518276511289189166051L; /* log_10 e */
constexpr long double CONST_LN2l      = 0.6931471805599453094172321214581766L; /* log_e 2 */
constexpr long double CONST_LN10l     = 2.3025850929940456840179914546843642L; /* log_e 10 */
constexpr long double CONST_PIl       = 3.1415926535897932384626433832795029L; /* pi */
constexpr long double CONST_PI_2l     = CONST_PI / 2.0L;                       /* pi/2 */
constexpr long double CONST_PI_4l     = CONST_PI / 4.0L;                       /* pi/4 */
constexpr long double CONST_PI_8l     = CONST_PI / 8.0L;                       /* pi/8 */
constexpr long double CONST_2_PIl     = 2.0L / CONST_PI;                       /* 2/pi */
constexpr long double CONST_2_SQRTPIl = 1.1283791670955125738961589031215452L; /* 2/sqrt(pi) */
constexpr long double CONST_2PIl      = 2.0L * CONST_PI;                       /* 2*pi */
constexpr long double CONST_PI_180l   = CONST_PI / 180.0L;                     /* pi/180 */
constexpr long double CONST_180_PIl   = 180.0L / CONST_PI;                     /* 180/pi */
// Defined but not used?
//const long double CONST_LNPIl =     std::log(CONST_PIl);                    /* ln(pi)*/
//const long double CONST_SQRTPIl =   std::sqrt(CONST_PIl);                   /* sqrt(pi)*/
constexpr long double CONST_SQRT2l   = 1.4142135623730950488016887242096981L; /* sqrt(2) */
constexpr long double CONST_SQRT1_2l = 0.7071067811865475244008443621048490L; /* 1/sqrt(2) */
constexpr long double CONST_SQRT3l   = 1.7320508075688772935274463415058724L; /* sqrt(3) */
constexpr long double CONST_EULERl   = 0.5772156649015328606065120900824024L; /* euler constant*/

#endif

template <typename T> T InverseValue(const T & value)
{
  return ((value > 0 || value < 0) ? 1/value : 0);
}

template <typename T> T SignOfValue(const T & value)
{
  return (value > 0) ? 1 : ((value < 0) ? -1 : 0);
}

}

#endif
