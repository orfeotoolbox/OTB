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


/**
 * otbMath.h defines standard math macros, constants, and other
 * parameters.
 * All mathematics constans, functions (used in the OTB code) are the vcl version (vcl..., M_PI, ). 
 */
  
#ifndef __otbMath_h
#define __otbMath_h

// For Windows MSVC, defines these macros. 
#ifdef _USE_MATH_DEFINES
#define _MATH_DEFINES_DEFINED
#endif

#include "vcl_cmath.h"

/** Define constants if not define in the "vcl_cmath.h" */

/* Some useful constants.  */
#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif
#ifndef M_LOG2E
#define M_LOG2E        1.4426950408889634074   /* log_2 e */
#endif
#ifndef M_LOG10E
#define M_LOG10E       0.43429448190325182765  /* log_10 e */
#endif
#ifndef M_LN2
#define M_LN2          0.69314718055994530942  /* log_e 2 */
#endif
#ifndef M_LN10
#define M_LN10         2.30258509299404568402  /* log_e 10 */
#endif
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif
#ifndef M_PI_2
#define M_PI_2         1.57079632679489661923  /* pi/2 */
#endif
#ifndef M_PI_4
#define M_PI_4         0.78539816339744830962  /* pi/4 */
#endif
#ifndef M_1_PI
#define M_1_PI         0.31830988618379067154  /* 1/pi */
#endif
#ifndef M_2_PI
#define M_2_PI         0.63661977236758134308  /* 2/pi */
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI     1.12837916709551257390  /* 2/sqrt(pi) */
#endif
#ifndef M_SQRT2
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2      0.70710678118654752440  /* 1/sqrt(2) */
#endif

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#ifdef __USE_GNU

#ifndef M_El
#define M_El           2.7182818284590452353602874713526625L  /* e */
#endif
#ifndef M_LOG2El
#define M_LOG2El       1.4426950408889634073599246810018921L  /* log_2 e */
#endif
#ifndef M_LOG10El
#define M_LOG10El      0.4342944819032518276511289189166051L  /* log_10 e */
#endif
#ifndef M_LN2l
#define M_LN2l         0.6931471805599453094172321214581766L  /* log_e 2 */
#endif
#ifndef M_LN10l
#define M_LN10l        2.3025850929940456840179914546843642L  /* log_e 10 */
#endif
#ifndef M_PIl
#define M_PIl          3.1415926535897932384626433832795029L  /* pi */
#endif
#ifndef M_PI_2l
#define M_PI_2l        1.5707963267948966192313216916397514L  /* pi/2 */
#endif
#ifndef M_PI_4l
#define M_PI_4l        0.7853981633974483096156608458198757L  /* pi/4 */
#endif
#ifndef M_1_PIl
#define M_1_PIl        0.3183098861837906715377675267450287L  /* 1/pi */
#endif
#ifndef M_2_PIl
#define M_2_PIl        0.6366197723675813430755350534900574L  /* 2/pi */
#endif
#ifndef M_2_SQRTPIl
#define M_2_SQRTPIl    1.1283791670955125738961589031215452L  /* 2/sqrt(pi) */
#endif
#ifndef M_SQRT2l
#define M_SQRT2l       1.4142135623730950488016887242096981L  /* sqrt(2) */
#endif
#ifndef M_SQRT1_2l
#define M_SQRT1_2l     0.7071067811865475244008443621048490L  /* 1/sqrt(2) */
#endif

#endif

/** Define short operation macro */

#ifndef M_SQUARE
#define M_SQUARE(x) ((x)*(x))
#endif

#endif //end of __otbMath_h


