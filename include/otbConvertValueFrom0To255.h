
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

#ifndef otbConvertValueFrom0To255_h
#define otbConvertValueFrom0To255_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
template<class TInput, class TOutput>
class ConvertValue
{
public:
  ConvertValue(){}
  virtual ~ConvertValue() {} 

   int GetDispMax()
    {
     return m_dispMax;     
    }

  int GetDispMin()
    {
     return m_dispMin;     
    }

  void SetDispMax(int disp)
    {
      m_dispMax = disp;
    }

  void SetDispMin(int disp)
    {
      m_dispMin = disp;
    }



  TOutput operator() ( TInput input )
    {

    TOutput output(1); 
    output = 0;
    float grayMin = 255;
    float grayMax = 0;
    float a = (grayMax-grayMin)/(m_dispMax-m_dispMin) ;

    // output = (255-a*input[0]+0.5) ;
     output =a*input[0]+0.5f;
   
    return output;
    }

  protected:
    int                             m_dispMin;
    int                             m_dispMax;

}; //end class

} // end Functor


  // class ConvertValueFrom0To255
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT ConvertValueFrom0To255 :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::ConvertValue<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef ConvertValueFrom0To255 Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::ConvertValue<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  int GetDispMax()
    {
     return this->GetFunctor().GetDispMax();     
    }

  int GetDispMin()
    {
     return this->GetFunctor().GetDispMin();     
    }

  void SetDispMax(int disp)
    {
      this->GetFunctor().SetDispMax(disp);
    }

  void SetDispMin(int disp)
    {
      this->GetFunctor().SetDispMin(disp);
    }


  protected:
  ConvertValueFrom0To255() {}
  ~ConvertValueFrom0To255() override {}


  void GenerateOutputInformation(void) override
    {

    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    }


  private:
  ConvertValueFrom0To255(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of ConvertValueFrom0To255 class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
