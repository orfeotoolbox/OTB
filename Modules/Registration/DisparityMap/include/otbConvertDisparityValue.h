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


#ifndef otbConvertDisparityValue_h
#define otbConvertDisparityValue_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class ComputeDisparityValue
 *
 * \brief Functor to add an integer value of each pixel of an image.
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput, class TOutput>
class ComputeDisparityValue
{
public:
  int GetDisp() const
    {
     return m_disp;     
    }
  void SetDisp(int disp)
    {
      m_disp = disp;
    }

  TOutput operator() ( TInput input ) const
    {
    const TOutput output = input + m_disp ; 
    return output;
    }

private:
  int                             m_disp;
}; //end class

} // end Functor



/** \class ConvertDisparityValue
 *
 * \brief Add an integer value to each pixel in an image
 *
 * \ingroup OTBDisparityMap
 */
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

  /** Get the disparity */
  int GetDisp() const
    {
     return this->GetFunctor().GetDisp();     
    }

  /** Set the disparity */
  void SetDisp(int disp)
    {
      this->GetFunctor().SetDisp(disp);
    }

protected:
  ConvertDisparityValue() = default ;
  ~ConvertDisparityValue() override = default ;

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
