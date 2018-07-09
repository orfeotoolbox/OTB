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

#include "otbUnaryFunctorImageFilter.h"

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

  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

   
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

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalCovarianceToReciprocalCoherencyFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   virtual ~ReciprocalCovarianceToReciprocalCoherencyFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbReciprocalCovarianceToReciprocalCoherencyImageFilter
 * \brief Compute the Coherency image (6 complex channels)
 * from the Covariance image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalCovarianceToReciprocalCoherencyFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalCovarianceToReciprocalCoherencyFunctor
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalCovarianceToReciprocalCoherencyImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalCovarianceToReciprocalCoherencyImageFilter  Self;
   typedef Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalCovarianceToReciprocalCoherencyImageFilter, UnaryFunctorImageFilter);


protected:
  ReciprocalCovarianceToReciprocalCoherencyImageFilter() {}
  ~ReciprocalCovarianceToReciprocalCoherencyImageFilter() override {}

private:
  ReciprocalCovarianceToReciprocalCoherencyImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;


};

} // end namespace otb

#endif
