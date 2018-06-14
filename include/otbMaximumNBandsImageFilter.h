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

#ifndef otbMaximumNBandsImageFilter_h
#define otbMaximumNBandsImageFilter_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
template<class TInput, class TOutput>
class MaxVectorImage
{
public:
  MaxVectorImage(){}
  virtual ~MaxVectorImage() {}

  TOutput operator() ( TInput input )
    {
    unsigned int size ( input.GetSize() ) ;

    TOutput output(1); 
    output = 0;
    typename TInput::ValueType min ( input[0] );

    for(unsigned int i=0; i<size; i++)
      {
      if (input[i]<min)
        {
        min = input[i] ;
        output = i;
        }
      }

    return output;
    }

}; //end class

} // end Functor


  // class MaximumNBandsImageFilter
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT MaximumNBandsImageFilter :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::MaxVectorImage<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef MaximumNBandsImageFilter Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::MaxVectorImage<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);



  protected:
  MaximumNBandsImageFilter() {}
  ~MaximumNBandsImageFilter() override {}

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    }

  private:
  MaximumNBandsImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of MaximumNBandsImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
