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


#ifndef otbReciprocalPauliDecompImageFilter_h
#define otbReciprocalPauliDecompImageFilter_h

#include "otbUnaryFunctorImageFilter.h"


namespace otb
 {

namespace Functor {

/** \class ReciprocalPauliDecompFunctor
 * 
 * \brief Evaluate the Pauli decomposition from the reciprocal Sinclair matrix image.
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


/** \class ReciprocalPauliDecompImageFilter
 * \brief Compute the Pauli decomposition image (3 complex channels)
 * from the Reciprocal Sinclair image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalPauliDecompFunctor.
 *
 * \ingroup OTBPolarimetry
 * \sa ReciprocalPauliDecompFunctor
 *
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
  ~ReciprocalPauliDecompImageFilter() ITK_OVERRIDE {}

private:
  ReciprocalPauliDecompImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
