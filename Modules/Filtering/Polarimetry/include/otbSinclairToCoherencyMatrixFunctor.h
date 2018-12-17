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

#ifndef otbSinclairToCoherencyMatrixImageFilter_h
#define otbSinclairToCoherencyMatrixImageFilter_h

#include <complex>
#include "otbFunctorImageFilter.h"
#include "otbPolarimetryTags.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCoherencyMatrixFunctor
 *  \brief Construct the fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 *  - channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #2 : \f$ (S_{hh}+S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #3 : \f$ (S_{hh}+S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #4 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #5 : \f$ (S_{hh}-S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #6 : \f$ (S_{hh}-S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #7 : \f$ (S_{hv}+S_{vh}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #8 : \f$ (S_{hv}+S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #9 : \f$ j(S_{hv}-S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 * Use otb::SinclairToCoherencyMatrixImageFilter to apply it to an image.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCoherencyMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef double                                   RealType;
  typedef std::complex <RealType>                  ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline void operator ()(TOutput & result, const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv) const
  {
    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    const ComplexType HHPlusVV   = S_hh + S_vv;
    const ComplexType HHMinusVV  = S_hh - S_vv;
    const ComplexType HVPlusVH   =  S_hv + S_vh;
    const ComplexType jHVMinusVH = (S_hv - S_vh) * ComplexType(0., 1.);

    result[0] = static_cast<OutputValueType>( std::norm(HHPlusVV) );
    result[1] = static_cast<OutputValueType>( HHPlusVV * std::conj(HHMinusVV) );
    result[2] = static_cast<OutputValueType>( HHPlusVV * std::conj(HVPlusVH) );
    result[3] = static_cast<OutputValueType>( HHPlusVV * std::conj(jHVMinusVH) );
    result[4] = static_cast<OutputValueType>( std::norm(HHMinusVV) );
    result[5] = static_cast<OutputValueType>( HHMinusVV * std::conj(HVPlusVH) );
    result[6] = static_cast<OutputValueType>( HHMinusVV * std::conj(jHVMinusVH) );
    result[7] = static_cast<OutputValueType>( std::norm(HVPlusVH) );
    result[8] = static_cast<OutputValueType>( HVPlusVH * std::conj(jHVMinusVH) );
    result[9] = static_cast<OutputValueType>( std::norm(jHVMinusVH) );

    result /= 2.0;
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of coherency matrix
    return 10;
  }

  /** Constructor */
  SinclairToCoherencyMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToCoherencyMatrixFunctor() {}
};

} // namespace Functor

  /**
   * \typedef SinclairToCoherencyMatrixImageFilter
   * \brief Applies otb::Functor::SinclairToCoherencyMatrixFunctor
   * \sa otb::Functor::SinclairToCoherencyMatrixFunctor
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
using SinclairToCoherencyMatrixImageFilter = FunctorImageFilter<
    Functor::SinclairToCoherencyMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType, typename TInputImage::PixelType,
                                              typename TInputImage::PixelType, typename TOutputImage::PixelType>,
    std::tuple<polarimetry_tags::hh, polarimetry_tags::hv, polarimetry_tags::vh, polarimetry_tags::vv>>;
} // namespace otb

#endif
