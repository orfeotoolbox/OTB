/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLocalGradientVectorImageFilter_h
#define otbLocalGradientVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

#include <itkNumericTraits.h>

namespace otb
{

namespace Functor
{
/** \class LocalGradientOperator
 * \brief Performs the calculation of LocalGradient derivation
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutput>
class LocalGradientOperator
{
public:
  LocalGradientOperator()
  {
  }
  virtual ~LocalGradientOperator()
  {
  }

  TOutput operator()(const TInput& input)
  {
    /*
     * it is assumed that input and output have the same size
     */
    unsigned int length = input.GetPixel(0).Size();
    TOutput      output(length);
    for (unsigned int i = 0; i < length; ++i)
    {
      output[i] = static_cast<typename TOutput::ValueType>(input.GetCenterPixel()[i] - input.GetPixel(5)[i] / 2. - input.GetPixel(7)[i] / 2.);
    }
    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class LocalGradientVectorImageFilter
 * \brief Implements the 3x3 Local Gradient to be processed on a vector image
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LocalGradientVectorImageFilter
    : public UnaryFunctorNeighborhoodVectorImageFilter<
          TInputImage, TOutputImage, Functor::LocalGradientOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs */
  typedef LocalGradientVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter<
      TInputImage, TOutputImage, Functor::LocalGradientOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LocalGradientVectorImageFilter, ImageToImageFilter);

protected:
  LocalGradientVectorImageFilter()
  {
    typename Superclass::RadiusType radius = {{1, 1}};
    this->SetRadius(radius);
  }
  ~LocalGradientVectorImageFilter() override
  {
  }

private:
  LocalGradientVectorImageFilter(const Self&); // Not implemented
  void operator=(const Self&);                 // Not implemented
};                                             // end of class

} // end of namespace otb


#endif // otbLocalGradientVectorImageFilter_h
