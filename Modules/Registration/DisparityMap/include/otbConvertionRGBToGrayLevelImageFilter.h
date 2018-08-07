/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
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


#ifndef otbConvertionRGBToGrayLevelImageFilter_h
#define otbConvertionRGBToGrayLevelImageFilter_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class RGBToGrayLevelOperator
 *
 *  \brief Functor to perform the gray-level conversion of a RGB image. 
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput, class TOutput>
class RGBToGrayLevelOperator
{
public:
  TOutput operator() ( TInput input )  
    {
    TOutput output(0);
    return output = 0.299*input[0] + 0.587*input[1] + 0.0721*input[2] ;
    }
}; //end class

} // end Functor



/** \class ConvertionRGBToGrayLevelImageFilter
 *
 *  \brief Perform gray-level conversion of RGB image. 
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ConvertionRGBToGrayLevelImageFilter :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::RGBToGrayLevelOperator<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ConvertionRGBToGrayLevelImageFilter Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::RGBToGrayLevelOperator<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  ConvertionRGBToGrayLevelImageFilter() =default ;
  ~ConvertionRGBToGrayLevelImageFilter() override =default;


  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    }

private:
  ConvertionRGBToGrayLevelImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of ConvertionRGBToGrayLevelImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
