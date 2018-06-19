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

#ifndef otbMinimumNBandsImageFilter_h
#define otbMinimumNBandsImageFilter_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
template<class TInput, class TOutput>
class MinVectorImage
{
public:
  MinVectorImage(){}
  virtual ~MinVectorImage() {} 





  TOutput operator() ( TInput input )
    {

    unsigned int size ( input.GetSize() ) ;
    int grayMin=255;
    int grayMax=0;

    TOutput output(1); 
    output = 0;
    typename TInput::ValueType min ( input[0] );

    for(unsigned int i=0; i<size; i++)
      {
      if (input[i]<min)
        {
        min = input[i] ;
        output = -i ;
        //output = 255+i*((grayMax-grayMin)/(m_dispMax-m_dispMin));
        }
      }

    return output;
    }

  void SetDispMax( int disp)
    {
       m_dispMax = disp ;
    }

   int GetDispMax()
    {
      return m_dispMax ;
    }

  void SetDispMin( int disp)
    {
       m_dispMin = disp ;
    }

   int GetDispMin()
    {
      return m_dispMin ;
    }

  protected:    
     int                   m_dispMin;
     int                   m_dispMax;


}; //end class

} // end Functor


  // class MinimumNBandsImageFilter
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT MinimumNBandsImageFilter :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::MinVectorImage<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef MinimumNBandsImageFilter Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::MinVectorImage<
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
  MinimumNBandsImageFilter() {}
  ~MinimumNBandsImageFilter() override {}


  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    }


  private:
  MinimumNBandsImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of MinimumNBandsImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
