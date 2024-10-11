/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSinclairToReciprocalCircularCovarianceMatrixImageFilter_h
#define otbSinclairToReciprocalCircularCovarianceMatrixImageFilter_h

#include <complex>
#include "otbSinclairToReciprocalCovarianceMatrixImageFilter.h"

#include "otbFunctorImageFilter.h"
#include "otbPolarimetryTags.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \brief Construct the fully reciprocal polarimetric circular covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{ll}.S_{ll}^{*} \f$
 *  - channel #1 : \f$ S_{ll}.S_{lr}^{*} \f$
 *  - channel #2 : \f$ S_{ll}.S_{rr}^{*} \f$
 *  - channel #3 : \f$ S_{lr}.S_{lr}^{*} \f$
 *  - channel #4 : \f$ S_{lr}.S_{rr}^{*} \f$
 *  - channel #5 : \f$ S_{rr}.S_{rr}^{*} \f$
 *
 * This is a adaptation of the SinclairToCircularCovarianceMatrixFunctor, where \f$ S_{hv}=S_{vh} \f$.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * Use otb::SinclairToReciprocalCircularCovarianceMatrixImageFilter to apply it to an image.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairImageFilter
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuellerMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class SinclairToReciprocalCircularCovarianceMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef double                      RealType;
  typedef std::complex<RealType>      ComplexType;
  typedef typename TOutput::ValueType OutputValueType;
  typedef SinclairToReciprocalCovarianceMatrixFunctor<ComplexType, ComplexType, ComplexType, TOutput> SinclairToReciprocalCovarianceFunctorType;

  inline void operator()(TOutput& result, const TInput1& Shh, const TInput2& Shv, const TInput3& Svv) const
  {
    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    const ComplexType coef(0.5);

    const ComplexType j2S_hv = S_hv * ComplexType(0.0, 2.0);
    const ComplexType jS_hh  = S_hh * ComplexType(0.0, 1.0);
    const ComplexType jS_vv  = S_vv * ComplexType(0.0, 1.0);


    const ComplexType Sll = coef * (S_hh + j2S_hv - S_vv);
    const ComplexType Slr = coef * (jS_hh + jS_vv);
    const ComplexType Srr = coef * (-S_hh + j2S_hv + S_vv);


    SinclairToReciprocalCovarianceFunctorType funct;
    funct(result, Sll, Slr, Srr);
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of the  matrix
    return 6;
  }
};

} // namespace Functor
  /**
   * \typedef SinclairToReciprocalCircularCovarianceMatrixImageFilter
   * \brief Applies otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor
   * \sa otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor
   *
   * Set inputs with:
   * \code
   *
   * SetInput<polarimetry_tags::hh>(inputPtr);
   * SetInput<polarimetry_tags::hv_or_vh>(inputPtr);
   * SetInput<polarimetry_tags::vv>(inputPtr);
   *
   * \endcode
   *
   * \ingroup OTBPolarimetry
   */
template <typename TInputImage, typename TOutputImage>
using SinclairToReciprocalCircularCovarianceMatrixImageFilter =
    FunctorImageFilter<Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType,
                                                                                    typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<polarimetry_tags::hh, polarimetry_tags::hv_or_vh, polarimetry_tags::vv>>;
} // namespace otb

#endif
