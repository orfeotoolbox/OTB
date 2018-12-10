/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSinclairToCircularCovarianceMatrixFunctor_h
#define otbSinclairToCircularCovarianceMatrixFunctor_h

#include <complex>
#include "otbSinclairToCovarianceMatrixFunctor.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCircularCovarianceMatrixFunctor
 *  \brief Construct the fully polarimetric circular covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{ll}.S_{ll}^{*} \f$
 *  - channel #1 : \f$ S_{ll}.S_{lr}^{*} \f$
 *  - channel #2 : \f$ S_{ll}.S_{rl}^{*} \f$
 *  - channel #3 : \f$ S_{ll}.S_{rr}^{*} \f$
 *  - channel #4 : \f$ S_{lr}.S_{lr}^{*} \f$
 *  - channel #5 : \f$ S_{lr}.S_{rl}^{*} \f$
 *  - channel #6 : \f$ S_{lr}.S_{rr}^{*} \f$
 *  - channel #7 : \f$ S_{rl}.S_{rl}^{*} \f$
 *  - channel #8 : \f$ S_{rl}.S_{rr}^{*} \f$
 *  - channel #9 : \f$ S_{rr}.S_{rr}^{*} \f$
 *
 * With:
 * - \f$ S_{ll} = 0.5 * (S_{hh}+i*S_{hv}+i*S_{vh}-S_{vv}) \f$
 * - \f$ S_{lr} = 0.5 * (i*S_{hh}+S_{hv}-S_{vh}+i*S_{vv}) \f$
 * - \f$ S_{rl} = 0.5 * (i*S_{hh}-S_{hv}+S_{vh}+i*S_{vv}) \f$
 * - \f$ S_{rr} = 0.5 * (-S_{hh}+i*S_{hv}+i*S_{vh}+S_{vv}) \f$
 *
 * Extract from Antennas for radar and communications Harold Mott p 317.
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuellerMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCircularCovarianceMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef double                                   RealType;
  typedef std::complex <RealType>                  ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  typedef SinclairToCovarianceMatrixFunctor<ComplexType, ComplexType, ComplexType, ComplexType, TOutput> SinclairToCovarianceFunctorType;
  inline void operator ()(TOutput & result, const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    const ComplexType jS_hv = S_hv * ComplexType(0., 1.);
    const ComplexType jS_vh = S_vh * ComplexType(0., 1.);
    const ComplexType jS_hh = S_hh * ComplexType(0., 1.);
    const ComplexType jS_vv = S_vv * ComplexType(0., 1.);

    const ComplexType coef(0.5);

    const ComplexType Sll = coef*( S_hh+jS_hv+jS_vh-S_vv );
    const ComplexType Slr = coef*( jS_hh+S_hv-S_vh+jS_vv );
    const ComplexType Srl = coef*( jS_hh-S_hv+S_vh+jS_vv );
    const ComplexType Srr = coef*( -S_hh+jS_hv+jS_vh+S_vv );

    SinclairToCovarianceFunctorType funct;
    funct( result, Sll, Slr, Srl, Srr );
  }

    constexpr size_t OutputSize(...) const
  {
    // Size of circular covariance matrix
    return 10;
  }

  /** Constructor */
  SinclairToCircularCovarianceMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToCircularCovarianceMatrixFunctor() {}
};

} // namespace Functor
} // namespace otb

#endif
