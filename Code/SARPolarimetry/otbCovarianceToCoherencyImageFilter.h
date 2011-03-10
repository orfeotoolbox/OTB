/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __CovarianceToCoherencyImageFilter_h
#define __CovarianceToCoherencyImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbCovarianceToCoherencyFunctor
 * \brief Evaluate the Coherency matrix from from the Covariance image
 *
 *   Output value are:
 *   channel #0 : \f$ 0.5 * (S_{hh}+S_{vv}.(S_{hh}+S_{vv})^{*} \f$
 *   channel #1 : \f$ 0.5 * (S_{hh}+S_{vv}.(S_{hh}-S_{vv})^{*} \f$
 *   channel #2 : \f$ (S_{hh}+S_{vv}.(S_{hv})^{*} \f$
 *   channel #3 : \f$ 0.5 * (S_{hh}-S_{vv}.(S_{hh}-S_{vv})^{*} \f$
 *   channel #4 : \f$ (S_{hh}-S_{vv}.(S_{hv})^{*}  \f$
 *   channel #5 : \f$ 2.0*S_{hv}.S_{hv}^{*} \f$
 *
 * \infgroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa CovarianceToCircularCoherencyDegreeImageFilter
 * \sa CovarianceToCoherencyDegreeImageFilter
 */
template< class TInput, class TOutput>
class CovarianceToCoherencyFunctor
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

    const ComplexType C21 =  vcl_conj(C12);
    const ComplexType C31 =  vcl_conj(C13);
    const ComplexType C32 =  vcl_conj(C23);

    result[0] = static_cast<OutputValueType>( 0.5*(C11 + C13 + C31 + C33) );
    result[1] = static_cast<OutputValueType>( 0.5*(C11 - C13 + C31 - C33) );
    result[2] = static_cast<OutputValueType>( C12 + C32 );
    result[3] = static_cast<OutputValueType>( 0.5*(C11 - C13 - C31 + C33) );
    result[4] = static_cast<OutputValueType>( C12 - C32 );
    result[5] = static_cast<OutputValueType>( 2.0 * C22 );

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   CovarianceToCoherencyFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   virtual ~CovarianceToCoherencyFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbCovarianceToCoherencyImageFilter
 * \brief Compute the Coherency image (6 complex channels)
 * from the Covariance image (6 complex channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::CovarianceToCoherencyFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT CovarianceToCoherencyImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef CovarianceToCoherencyImageFilter  Self;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(CovarianceToCoherencyImageFilter, UnaryFunctorImageFilter);


protected:
  CovarianceToCoherencyImageFilter() {}
  virtual ~CovarianceToCoherencyImageFilter() {}

private:
  CovarianceToCoherencyImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

 
 

};
  
} // end namespace otb

#endif
