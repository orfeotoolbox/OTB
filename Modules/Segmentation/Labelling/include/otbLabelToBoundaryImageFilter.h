/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelToBoundaryImageFilter_h
#define otbLabelToBoundaryImageFilter_h

#include "otbUnaryFunctorNeighborhoodImageFilter.h"

namespace otb
{
namespace Functor
{

/**
 * \class LabelToBoundaryFunctor
 *
 * \brief Functor to extract segmentation boundaries
 *
 * Functor intended to work with 3x3 neighborhood and scalar label image
 * The generated boundary is 1-pixel wide, so it is not symmetric.
 * Output value is 1 on the boundaries and 0 in the background
 *
 * \ingroup OTBLabelling
 */
template <class TInput, class TOutput>
class LabelToBoundaryFunctor
{
public:
  LabelToBoundaryFunctor()
  {
  }
  virtual ~LabelToBoundaryFunctor()
  {
  }

  TOutput operator()(const TInput& input)
  {
    unsigned char output = 1;
    if (input.GetCenterPixel() == input.GetPixel(5) && input.GetCenterPixel() == input.GetPixel(7) && input.GetCenterPixel() == input.GetPixel(8))
    {
      output = 0;
    }
    return static_cast<TOutput>(output);
  }
}; // end of class

} // end of Functor namespace

/**
 * \class LabelToBoundaryImageFilter
 *
 * \brief Filter to extract boundaries of a label image
 *
 * Filter intended to work with a scalar label image.
 * The generated boundary is 1-pixel wide, so it is not symmetric.
 * Output value is 1 on the boundaries and 0 in the background
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelToBoundaryImageFilter
    : public UnaryFunctorNeighborhoodImageFilter<
          TInputImage, TOutputImage, Functor::LabelToBoundaryFunctor<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
{
public:
  typedef LabelToBoundaryImageFilter Self;
  typedef UnaryFunctorNeighborhoodImageFilter<
      TInputImage, TOutputImage, Functor::LabelToBoundaryFunctor<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(LabelToBoundaryImageFilter, UnaryFunctorNeighborhoodImageFilter);

protected:
  LabelToBoundaryImageFilter()
  {
    this->SetRadius(1);
  }
  ~LabelToBoundaryImageFilter() override
  {
  }

private:
  LabelToBoundaryImageFilter(const Self&); // Not implemented
  void operator=(const Self&);             // Not implemented
};                                         // end of class

} // end of otb namespace

#endif
