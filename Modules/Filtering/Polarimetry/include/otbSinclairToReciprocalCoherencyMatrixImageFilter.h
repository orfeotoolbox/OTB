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

#ifndef otbSinclairToReciprocalCoherencyMatrixImageFilter_h
#define otbSinclairToReciprocalCoherencyMatrixImageFilter_h

#include "itkMacro.h"
#include <complex>
#include "otbMath.h"
#include "vnl/vnl_matrix.h"

#include "otbFunctorImageFilter.h"
#include "otbPolarimetryTags.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToReciprocalCoherencyMatrixFunctor
 *  \brief Construct the reciprocal fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 *  - channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #2 : \f$ (S_{hh}+S_{vv}).(2*S_{hv})^{*} \f$
 *  - channel #3 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #4 : \f$ (S_{hh}-S_{vv}).(2*S_{hv})^{*} \f$
 *  - channel #5 : \f$ (2*S_{hv}).(2*S_{hv})^{*} \f$
 *
 * This is a adaptation of the SinclairToCoherencyFunctor, where \f$ S_{hv}=S_{vh} \f$.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * Use otb::SinclairToReciprocalCoherencyMatrixImageFilter to apply it to an image.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuellerMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class SinclairToReciprocalCoherencyMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef vnl_matrix<ComplexType>       		   VNLMatrixType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline void operator()(TOutput& result, const TInput1& Shh, const TInput2& Shv, const TInput3& Svv) const
  {
    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    VNLMatrixType f3p(3, 1, 0.);
    f3p[0][0]= (S_hh + S_vv) / ComplexType( std::sqrt(2.0) , 0.0);
    f3p[1][0]= (S_hh - S_vv) / ComplexType( std::sqrt(2.0) , 0.0);
    f3p[2][0]= ComplexType( std::sqrt(2.0) , 0.0) * S_hv;

    VNLMatrixType res = f3p*f3p.conjugate_transpose();

    result[0] = static_cast<OutputValueType>( res[0][0] );
    result[1] = static_cast<OutputValueType>( res[0][1] );
    result[2] = static_cast<OutputValueType>( res[0][2] );
    result[3] = static_cast<OutputValueType>( res[1][1] );
    result[4] = static_cast<OutputValueType>( res[1][2] );
    result[5] = static_cast<OutputValueType>( res[2][2] );
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of the  matrix
    return 6;
  }
};

} // namespace Functor

/**
 * \typedef SinclairToReciprocalCoherencyMatrixImageFilter
 * \brief Applies otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor
 * \sa otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor
 *
 * Set inputs with:
 * \code
 *
 * SetVariadicNamedInput<polarimetry_tags::hh>(inputPtr);
 * SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(inputPtr);
 * SetVariadicNamedInput<polarimetry_tags::vv>(inputPtr);
 *
 * \endcode
 *
 * \ingroup OTBPolarimetry
 */
template <typename TInputImage, typename TOutputImage>
using SinclairToReciprocalCoherencyMatrixImageFilter =
    FunctorImageFilter<Functor::SinclairToReciprocalCoherencyMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType,
                                                                           typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<polarimetry_tags::hh, polarimetry_tags::hv_or_vh, polarimetry_tags::vv>>;

} // namespace otb

#endif
