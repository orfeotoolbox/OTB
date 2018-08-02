
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

#ifndef otbConvertDisparityValue_h
#define otbConvertDisparityValue_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
template<class TInput, class TOutput>
class ComputeDisparityValue
{
public:
  ComputeDisparityValue(){}
  virtual ~ComputeDisparityValue() {} 

   int GetDisp()
    {
     return m_disp;     
    }

  void SetDisp(int disp)
    {
      m_disp = disp;
    }

  TOutput operator() ( TInput input )
    {

    TOutput output(1); 
    output = 0;
    output = input + m_disp ;
   
    return output;
    }

  protected:
    int                             m_disp;

}; //end class

} // end Functor


  // class ConvertDisparityValue
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT ConvertDisparityValue :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::ComputeDisparityValue<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef ConvertDisparityValue Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::ComputeDisparityValue<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  int GetDisp()
    {
     return this->GetFunctor().GetDisp();     
    }

  void SetDisp(int disp)
    {
      this->GetFunctor().SetDisp(disp);
    }


  protected:
  ConvertDisparityValue() {}
  ~ConvertDisparityValue() override {}


  void GenerateOutputInformation(void) override
    {

    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    }


  private:
  ConvertDisparityValue(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of ConvertDisparityValue class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
