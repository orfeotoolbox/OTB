/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdMath_h
#define mvdMath_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

#define USE_VNL 0

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#if USE_VNL
#include "vnl_vector.h"
#endif

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
} // end of namespace 'mvd'.


/*****************************************************************************/
/* FUNCTIONS DECLARATION.                                                    */

namespace otb
{
} // end namespace 'otb'

namespace mvd
{
/**
 */
template <typename X, typename Y, typename K>
void Lerp2(X& x, Y& y, const K& k, const X& x0, const Y& y0, const X& x1, const Y& y1);
/**
 */
template <typename X, typename Y>
const Y& Lerp2(const X& x, const X& x0, const Y& y0, const X& x1, const Y& y1);

/**
 */
#if USE_VNL
template <typename T, unsigned int N>
inline const vnl_vector<T, N>& Lerp(const T& k, const vnl_vector<T, N>& v0, const vnl_vector<T, N>& v1);
#endif // USE_VNL

/**
 */
#if USE_VNL
template <typename T>
inline const T& Lerp(const T& k, const T& x0, const T& x1, const T& y0, const T& y1);
#endif // USE_VNL

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace otb
{
} // end namespace 'otb'.

namespace mvd
{
/*******************************************************************************/
template <typename X, typename Y, typename K>
void Lerp2(X& x, Y& y, const K& k, const X& x0, const Y& y0, const X& x1, const Y& y1)
{
  const K& _1_minus_k(1 - k);

  x = k * x1 + _1_minus_k * x0;
  y = k * y1 + _1_minus_k * y0;
}

/*******************************************************************************/
template <typename X, typename Y, typename K>
const Y& Lerp2(const X& x, const X& x0, const Y& y0, const X& x1, const Y& y1)
{
  return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

/*******************************************************************************/
#if USE_VNL
template <typename T>
inline const vnl_vector<T, N>& Lerp(const T& k, const vnl_vector<T, N>& v0, const vnl_vector<T, N>& v1)
{
  return k * v1 + (T(1) - k) * v2;
}
#endif // USE_VNL

/*******************************************************************************/
#if USE_VNL
template <typename T>
inline const T& Lerp(const T& k, const T& x0, const T& x1, const T& y0, const T& y1)
{
  return Lerp(k, vnl_vector<T, 2>(x0, x1), vnl_vector<T, 2>(y0, y1));
}
#endif // USE_VNL

} // end namespace 'mvd'

#endif // mvdMath_h
