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

#ifndef __ReciprocalHuynenDecompImageFilter_h
#define __ReciprocalHuynenDecompImageFilter_h

#include "otbUnaryFunctorImageFilter.h"


namespace otb
 {

namespace Functor {

/** \class otbHuynenDecompFunctor
 * \brief Evaluate the H-Alpha parameters from the reciprocal covariance matrix image.
 *
 *
 * \ingroup SARPolarimetry
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalHuynenDecompFunctor
{
public:

  typedef typename TOutput::ValueType   OutputValueType;


  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);
    
    OutputValueType A0 = static_cast<OutputValueType>(Covariance[0].real() / 2.0);
    OutputValueType B0 = static_cast<OutputValueType>((Covariance[3] + Covariance[5]).real() / 2.0);
    OutputValueType B = static_cast<OutputValueType>(Covariance[3].real() - B0);
    OutputValueType C = static_cast<OutputValueType>(Covariance[1].real());
    OutputValueType D = static_cast<OutputValueType>(-Covariance[1].imag());
    OutputValueType E = static_cast<OutputValueType>(Covariance[4].real());
    OutputValueType F = static_cast<OutputValueType>(Covariance[4].imag());
    OutputValueType G = static_cast<OutputValueType>(Covariance[2].imag());
    OutputValueType H = static_cast<OutputValueType>(Covariance[2].real());
    
    result[0] = A0;
    result[1] = B0;
    result[2] = B;
    result[3] = C;
    result[4] = D;
    result[5] = E;
    result[6] = F;
    result[7] = G;
    result[8] = H;

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalHuynenDecompFunctor() : m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalHuynenDecompFunctor() {}

private:
   itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 9);
   const double m_Epsilon;
};
}


/** \class otbHuynenDecompImageFilter
 * \brief Compute the H-Alpha image (3 channels)
 * from the Reciprocal Covariance image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalHuynenDecompFunctor.
 *
 * \ingroup SARPOlarimetry
 * \sa ReciprocalHuynenDecompFunctor
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalHuynenDecompImageFilter :
   public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalHuynenDecompFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalHuynenDecompImageFilter  Self;
   typedef typename Functor::ReciprocalHuynenDecompFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalHuynenDecompImageFilter, UnaryFunctorImageFilter);

protected:
   ReciprocalHuynenDecompImageFilter() {}
  virtual ~ReciprocalHuynenDecompImageFilter() {}

private:
  ReciprocalHuynenDecompImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
