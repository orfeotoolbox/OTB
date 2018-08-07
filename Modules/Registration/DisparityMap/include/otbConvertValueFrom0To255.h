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


#ifndef otbConvertValueFrom0To255_h
#define otbConvertValueFrom0To255_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class ConvertValue
 *
 *  \brief Functor to convert pixel values of the disparity map from [dmin;dmax] integer to [0;255]
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput, class TOutput>
class ConvertValue
{
public:
  int GetDispMax() const
    {
     return m_dispMax;     
    }

  int GetDispMin() const
    {
     return m_dispMin;     
    }

  int GetOffset() const
    {
     return m_offset;     
    }

  void SetDispMax(int disp)
    {
      m_dispMax = disp;
    }

  void SetDispMin(int disp)
    {
      m_dispMin = disp;
    }

  void SetOffset(int disp)
    {
      m_offset = disp ;
    }

  int GetSubstractionBetweenMinMaxDisparity() const
    {
    return m_dispSubtraction = m_dispMax-m_dispMin ;
    }


  TOutput operator() ( TInput input ) const
    {
    float grayMin = 255;
    float grayMax = 0;
    float a = (grayMax-grayMin)/(m_dispMax-m_dispMin) ;
    float b = (grayMin*m_dispMax - grayMax*m_dispMin)/(m_dispSubtraction);
    
    const TOutput output = a*input+0.5f + b + m_offset;    
    return output;
    }

  protected:
    int                             m_dispMin;
    int                             m_dispMax;
    int                             m_offset ;
    int                             m_dispSubtraction ;

}; //end class

} // end Functor



/** \class ConvertValueFrom0To255
 *
 *  \brief Scale each pixel value of the disparityMap to the range 0 - 255
 *
 * \ingroup OTBDisparityMap
 */
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


  int GetDispMax() const
    {
     return this->GetFunctor().GetDispMax();     
    }

  int GetDispMin() const
    {
     return this->GetFunctor().GetDispMin();     
    }

  int GetOffset() const
    {
     return this->GetFunctor().GetOffset();     
    }

  void SetDispMax(int disp)
    {
      this->GetFunctor().SetDispMax(disp);
    }

  void SetDispMin(int disp)
    {
      this->GetFunctor().SetDispMin(disp);
    }

  void SetOffset(int disp)
    {
      this->GetFunctor().SetOffset(disp);
    }


  protected:
  ConvertValueFrom0To255() = default ;
  ~ConvertValueFrom0To255() override = default ;


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
