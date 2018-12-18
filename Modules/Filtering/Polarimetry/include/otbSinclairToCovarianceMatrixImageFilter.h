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

#ifndef otbSinclairToCovarianceMatrixImageFilter_h
#define otbSinclairToCovarianceMatrixImageFilter_h

#include <complex>
#include "otbFunctorImageFilter.h"
#include "otbPolarimetryTags.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCovarianceMatrixFunctor
 *  \brief Construct the fully polarimetric covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{hh}.S_{hh}^{*} \f$
 *  - channel #1 : \f$ S_{hh}.S_{hv}^{*} \f$
 *  - channel #2 : \f$ S_{hh}.S_{vh}^{*} \f$
 *  - channel #3 : \f$ S_{hh}.S_{vv}^{*} \f$
 *  - channel #4 : \f$ S_{hv}.S_{hv}^{*} \f$
 *  - channel #5 : \f$ S_{hv}.S_{vh}^{*} \f$
 *  - channel #6 : \f$ S_{hv}.S_{vv}^{*} \f$
 *  - channel #7 : \f$ S_{vh}.S_{vh}^{*} \f$
 *  - channel #8 : \f$ S_{vh}.S_{vv}^{*} \f$
 *  - channel #9 : \f$ S_{vv}.S_{vv}^{*} \f$
 *
 *  The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 *  Element are stored from left to right, line by line.
 *
 *  Use otb::SinclairToCovarianceMatrixImageFilter to apply
 *  it to an image.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *  \ingroup OTBPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCovarianceMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline void operator()(TOutput& result, const TInput1& Shh, const TInput2& Shv, const TInput3& Svh, const TInput4& Svv) const
  {
    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    result[0] = static_cast<OutputValueType>( std::norm(S_hh) );
    result[1] = static_cast<OutputValueType>( S_hh*std::conj(S_hv) );
    result[2] = static_cast<OutputValueType>( S_hh*std::conj(S_vh) );
    result[3] = static_cast<OutputValueType>( S_hh*std::conj(S_vv) );
    result[4] = static_cast<OutputValueType>( std::norm(S_hv) );
    result[5] = static_cast<OutputValueType>( S_hv*std::conj(S_vh) );
    result[6] = static_cast<OutputValueType>( S_hv*std::conj(S_vv) );
    result[7] = static_cast<OutputValueType>( std::norm(S_vh) );
    result[8] = static_cast<OutputValueType>( S_vh*std::conj(S_vv) );
    result[9] = static_cast<OutputValueType>( std::norm(S_vv) );
  }

  constexpr size_t OutputSize(...) const
  {
    // Number of components in the covariance matrix
    return 10;
  }
};
} // namespace Functor

/**
 * \typedef SinclairToCovarianceMatrixImageFilter
 * \brief Applies otb::Functor::SinclairToCovarianceMatrixFunctor
 * \sa otb::Functor::SinclairToCovarianceMatrixFunctor
 *
 * Set inputs with:
 * \code
 *
 * SetVariadicNamedInput<polarimetry_tags::hh>(inputPtr);
 * SetVariadicNamedInput<polarimetry_tags::hv>(inputPtr);
 * SetVariadicNamedInput<polarimetry_tags::vh>(inputPtr);
 * SetVariadicNamedInput<polarimetry_tags::vv>(inputPtr);
 *
 * \endcode
 *
 * \ingroup OTBPolarimetry
 */
template <typename TInputImage, typename TOutputImage>
using SinclairToCovarianceMatrixImageFilter = FunctorImageFilter<
    Functor::SinclairToCovarianceMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType, typename TInputImage::PixelType,
                                               typename TInputImage::PixelType, typename TOutputImage::PixelType>,
    std::tuple<polarimetry_tags::hh, polarimetry_tags::hv, polarimetry_tags::vh, polarimetry_tags::vv>>;

} // namespace otb

#endif
