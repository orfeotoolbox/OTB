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


#ifndef otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter_h
#define otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter_h

#include "otbFunctorImageFilter.h"
#include <complex>

namespace otb
{

namespace Functor
{

/** \class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 * \brief Compute the reciprocal Covariance circular matrix from the reciprocal Covariance linear matrix.
 *  Extract from Antennas for radar and communications Harold Mott p 317.
 *
 *  Output value are:
 *  - channel #0 : \f$ 0.25 * (C33-i.\sqrt{2}.C23-C13+i.\sqrt{2}.C23^{*}-C13^{*}+2.C22-i.\sqrt{2}.C12+i.\sqrt{2}.C12^{*}+C11) \f$
 *  - channel #1 : \f$ 0.25 * (i.\sqrt{2}.C33+2.C23-i.\sqrt{2}.C13+i.\sqrt{2}.C13^{*}+2.C12^{*}-i.\sqrt{2}.C11) \f$
 *  - channel #2 : \f$ 0.25 * (-C33+i.\sqrt{2}.C23+C13+i.\sqrt{2}.C23^{*}+C13^{*}+2.C22-i.\sqrt{2}.C12-i.\sqrt{2}.C12^{*}-C11 ) \f$
 *  - channel #3 : \f$ 0.25 * (2.C33+2.C13+2.C13^{*}+2.C11)\f$
 *  - channel #4 : \f$ 0.25 * (i.\sqrt{2}.C33+i.\sqrt{2}.C13+2.C23^{*}-i.\sqrt{2}.C13^{*}+2.C12-i.\sqrt{2}.C11) \f$
 *  - channel #5 : \f$ 0.25 * (C33+i.\sqrt{2}.C23-C13-i.\sqrt{2}.C23^{*}-C13^{*}+2.C22+i.\sqrt{2}.C12-i.\sqrt{2}.C12^{*}+C11) \f$
 *
 *  Where Cij are related to the elements of the reciprocal linear covariance matrix.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa CovarianceToCoherencyDegreeImageFilter
 * \sa CovarianceToCoherencyImageFilter
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput, class TOutput>
class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
{
public:
  typedef double                        RealType;
  typedef typename std::complex<double> ComplexType;
  typedef typename TOutput::ValueType   OutputValueType;

  inline void operator()(TOutput& result, const TInput& Covariance) const
  {
    const ComplexType C11 = static_cast<ComplexType>(Covariance[0]); //   <hh.hh*>
    const ComplexType C12 = static_cast<ComplexType>(Covariance[1]); //   <sqrt(2).hh.hv*>
    const ComplexType C13 = static_cast<ComplexType>(Covariance[2]); //   <hh.vv*>
    const ComplexType C22 = static_cast<ComplexType>(Covariance[3]); //   <2.hv.hv*>
    const ComplexType C23 = static_cast<ComplexType>(Covariance[4]); //   <sqrt(2).hv.vv*>
    const ComplexType C33 = static_cast<ComplexType>(Covariance[5]); //   <vv.vv*>


    const ComplexType cst1 = ComplexType(0.0, std::sqrt(2.0));
    const ComplexType two  = ComplexType(2.0, 0);

    result[0] =
        static_cast<ComplexType>(C33 - cst1 * C23 - C13 + cst1 * std::conj(C23) - std::conj(C13) + two * C22 - cst1 * C12 + cst1 * std::conj(C12) + C11);
    result[1] = static_cast<ComplexType>(cst1 * C33 + two * C23 - cst1 * C13 + cst1 * std::conj(C13) + two * std::conj(C12) - cst1 * C11);
    result[2] =
        static_cast<ComplexType>(-C33 + cst1 * C23 + C13 + cst1 * std::conj(C23) + std::conj(C13) + two * C22 - cst1 * C12 - cst1 * std::conj(C12) - C11);
    result[3] = static_cast<ComplexType>(two * C33 + two * C13 + two * std::conj(C13) + two * C11);
    result[4] = static_cast<ComplexType>(cst1 * C33 + cst1 * C13 + two * std::conj(C23) - cst1 * std::conj(C13) + two * C12 - cst1 * C11);
    result[5] =
        static_cast<ComplexType>(C33 + cst1 * C23 - C13 - cst1 * std::conj(C23) - std::conj(C13) + two * C22 + cst1 * C12 - cst1 * std::conj(C12) + C11);

    result /= 4.0;
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of the result (entropy, alpha, anisotropy)
    return 6;
  }
};
} // namespace Functor

/**
 * \typedef ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
 * \brief Applies otb::Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 * \sa otb::Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 *
 * Set inputs with:
 * \code
 * SetInput<0>(inputPtr);
 * \endcode
 *
 * \ingroup OTBPolarimetry
 */
template <typename TInputImage, typename TOutputImage>
using ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter = FunctorImageFilter<
    Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

} // end namespace otb

#endif
