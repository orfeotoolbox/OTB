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

#ifndef __ReciprocalPauliDecompImageFilter_h
#define __ReciprocalPauliDecompImageFilter_h

#include "otbUnaryFunctorImageFilter.h"


namespace otb
 {

namespace Functor {

/** \class otbPauliDecompFunctor
 * 
 * \brief Evaluate the Pauli decomposition from the reciprocal Sinclair matrix image.
 *
 * \ingroup SARPolarimetry
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalPauliDecompFunctor
{
public:

  typedef typename TInput::ValueType   InputValueType;
  typedef typename TOutput::ValueType  OutputValueType;


  inline TOutput operator()( const TInput & Sinclair ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);
    
    InputValueType sqrt2(std::sqrt(2.0),0.0);
    
    InputValueType Shh = static_cast<InputValueType>(Sinclair[0]);
    InputValueType Shv = static_cast<InputValueType>(Sinclair[1]);
    InputValueType Svv = static_cast<InputValueType>(Sinclair[2]);
    
    result[0] = (Shh+Svv)/sqrt2;
    result[1] = (Shh-Svv)/sqrt2;
    result[2] = sqrt2*Shv;
    
    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalPauliDecompFunctor() : m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalPauliDecompFunctor() {}

private:
   itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 3);
   const double m_Epsilon;
};
}


/** \class otbPauliDecompImageFilter
 * \brief Compute the Pauli decomposition image (3 complex channels)
 * from the Reciprocal Sinclair image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalPauliDecompFunctor.
 *
 * \ingroup SARPOlarimetry
 * \sa ReciprocalPauliDecompFunctor
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalPauliDecompImageFilter :
   public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalPauliDecompFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalPauliDecompImageFilter  Self;
   typedef typename Functor::ReciprocalPauliDecompFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalPauliDecompImageFilter, UnaryFunctorImageFilter);

protected:
   ReciprocalPauliDecompImageFilter() {this->SetNumberOfThreads(1);}
  virtual ~ReciprocalPauliDecompImageFilter() {}

private:
  ReciprocalPauliDecompImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
