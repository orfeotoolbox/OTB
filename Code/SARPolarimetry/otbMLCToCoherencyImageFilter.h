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

#ifndef __MLCToCoherencyImageFilter_h
#define __MLCToCoherencyImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

template< class TInput, class TOutput>
class MLCToCoherencyFunctor
{
public:
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    ComplexType C11 =  static_cast<ComplexType>(Covariance[0]);
    ComplexType C12 =  static_cast<ComplexType>(Covariance[1]);
    ComplexType C13 =  static_cast<ComplexType>(Covariance[2]);
    ComplexType C22 =  static_cast<ComplexType>(Covariance[3]);
    ComplexType C23 =  static_cast<ComplexType>(Covariance[4]);
    ComplexType C33 =  static_cast<ComplexType>(Covariance[5]);

    ComplexType C21 =  vcl_conj(C12);
    ComplexType C31 =  vcl_conj(C13);
    ComplexType C32 =  vcl_conj(C23);

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
   MLCToCoherencyFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   ~MLCToCoherencyFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMLCToCoherencyImageFilter
 * \brief Compute the Coherency image (9 channels) from the MLC image (9 channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MLCToCoherencyFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MLCToCoherencyImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MLCToCoherencyImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MLCToCoherencyImageFilter,itk::UnaryFunctorImageFilter);


protected:
  MLCToCoherencyImageFilter() {}
  virtual ~MLCToCoherencyImageFilter() {}

private:
  MLCToCoherencyImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

 
 

};
  
} // end namespace otb

#endif
