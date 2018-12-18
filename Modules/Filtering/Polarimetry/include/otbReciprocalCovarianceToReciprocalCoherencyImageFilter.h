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


#ifndef otbReciprocalCovarianceToReciprocalCoherencyImageFilter_h
#define otbReciprocalCovarianceToReciprocalCoherencyImageFilter_h

#include "otbFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class ReciprocalCovarianceToReciprocalCoherencyFunctor
 * \brief Evaluate the Coherency matrix from the Covariance image
 *
 * Output value are:
 * - channel #0 : \f$ 0.5 . (C33 + C13 + C13^{*} + C11) \f$
 * - channel #1 : \f$ 0.5 . (-C33 - C13 + C13^{*} + C11) \f$
 * - channel #2 : \f$ 0.5 . (\sqrt{2}.C12 + \sqrt{2}.C23^{*}) \f$
 * - channel #3 : \f$ 0.5 . (C33 - C13 - C13^{*} + C11 \f$)
 * - channel #4 : \f$ 0.5 . (\sqrt{2}.C12 - \sqrt{2}.C23^{*}) \f$
 * - channel #5 : \f$ 0.5 . (2 . C22) \f$
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * Use otbReciprocalCovarianceToReciprocalCoherencyImageFilter to apply
 * it to an image.
 *
 * \sa CovarianceToCircularCoherencyDegreeImageFilter
 * \sa ReciprocalCovarianceToReciprocalCoherencyDegreeImageFilter
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalCovarianceToReciprocalCoherencyFunctor
{
public:
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline void operator()(TOutput& result, const TInput& Covariance) const
  {
    const ComplexType C11 =  static_cast<ComplexType>(Covariance[0]);
    const ComplexType C12 =  static_cast<ComplexType>(Covariance[1]);
    const ComplexType C13 =  static_cast<ComplexType>(Covariance[2]);
    const ComplexType C22 =  static_cast<ComplexType>(Covariance[3]);
    const ComplexType C23 =  static_cast<ComplexType>(Covariance[4]);
    const ComplexType C33 =  static_cast<ComplexType>(Covariance[5]);

    const ComplexType two = ComplexType(2.0, 0.0);
    const ComplexType rootTwo = ComplexType(std::sqrt(2.0), 0.0);

    result[0] = static_cast<OutputValueType>( C33 + C13 + std::conj(C13) + C11 );
    result[1] = static_cast<OutputValueType>( -C33 - C13 + std::conj(C13) + C11 );
    result[2] = static_cast<OutputValueType>( rootTwo*C12 + rootTwo*std::conj(C23) );
    result[3] = static_cast<OutputValueType>( C33 - C13 - std::conj(C13) + C11 );
    result[4] = static_cast<OutputValueType>( rootTwo*C12 - rootTwo*std::conj(C23) );
    result[5] = static_cast<OutputValueType>( two * C22 );

    result /= 2.0;
    }

    constexpr size_t OutputSize(...) const
    {
      // Size of the result
      return 6;
    }
};
} // namespace Functor

/**
 * \typedef ReciprocalCovarianceToReciprocalCoherencyImageFilter
 * \brief Applies ReciprocalCovarianceToReciprocalCoherencyFunctor
 * \sa ReciprocalCovarianceToReciprocalCoherencyFunctor
 *
 * Set inputs with:
 * \code
 * SetVariadicInput<0>(inputPtr);
 * \endcode
 *
 *
 */
template <typename TInputImage, typename TOutputImage>
using ReciprocalCovarianceToReciprocalCoherencyImageFilter =
    FunctorImageFilter<Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

} // end namespace otb

#endif
