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

#ifndef otbSinclairToReciprocalCovarianceMatrixFunctor_h
#define otbSinclairToReciprocalCovarianceMatrixFunctor_h

#include <complex>
#include "otbMath.h"
#include "vnl/vnl_matrix.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToReciprocalCovarianceMatrixFunctor
 *  \brief Construct the reciprocal fully polarimetric covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{hh}.S_{hh}^{*} \f$
 *  - channel #1 : \f$ \sqrt{2}.S_{hh}.S_{hv}^{*} \f$
 *  - channel #2 : \f$ S_{hh}.S_{vv}^{*} \f$
 *  - channel #3 : \f$ 2.S_{hv}.S_{hv}^{*} \f$
 *  - channel #4 : \f$ \sqrt{2}.S_{hv}.S_{vv}^{*} \f$
 *  - channel #5 : \f$ S_{vv}.S_{vv}^{*} \f$
 *
 * This is a adaptation of the SinclairToCovarianceMatrixFunctor, where \f$ S_{hv}=S_{vh} \f$.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyeMatrixFunctor
 *  \sa SinclairToMuellereMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyeMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class SinclairToReciprocalCovarianceMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef vnl_matrix<ComplexType>       		   VNLMatrixType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv, const TInput3& Svv)
  {
    TOutput result;

    result.SetSize(NumberOfComponentsPerPixel);

    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);
    
    VNLMatrixType f3l(3, 1, 0.);
    f3l[0][0]=S_hh;
    f3l[1][0]=ComplexType(std::sqrt(2.0),0.0)*S_hv;
    f3l[2][0]=S_vv;
    
    VNLMatrixType res = f3l*f3l.conjugate_transpose();
    
    result[0] = static_cast<OutputValueType>( res[0][0] );
    result[1] = static_cast<OutputValueType>( res[0][1] );
    result[2] = static_cast<OutputValueType>( res[0][2] );
    result[3] = static_cast<OutputValueType>( res[1][1] );
    result[4] = static_cast<OutputValueType>( res[1][2] );
    result[5] = static_cast<OutputValueType>( res[2][2] );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToReciprocalCovarianceMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToReciprocalCovarianceMatrixFunctor() {}

protected:


private:
  //itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 6);
  static const unsigned int NumberOfComponentsPerPixel = 6;

};

} // namespace Functor
} // namespace otb

#endif
