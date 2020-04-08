/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVerticalSobelVectorImageFilter_h
#define otbVerticalSobelVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class VerticalSobelOperator
 * \brief Performs the calculation of vertical Sobel derivation
 *
 * \ingroup OTBEdge
 */
template <class TInput, class TOutput>
class VerticalSobelOperator
{
public:
  VerticalSobelOperator()
  {
  }
  virtual ~VerticalSobelOperator()
  {
  }

  TOutput operator()(const TInput& input)
  {
    unsigned int length = input.GetPixel(0).Size();
    TOutput      output(length);
    for (unsigned int i = 0; i < length; ++i)
    {
      output[i] = static_cast<typename TOutput::ValueType>((input.GetPixel(0)[i] - input.GetPixel(6)[i]) + 2 * (input.GetPixel(1)[i] - input.GetPixel(7)[i]) +
                                                           (input.GetPixel(2)[i] - input.GetPixel(8)[i]));
    }
    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class VerticalSobelVectorImageFilter
 * \brief Implements the Vertical Sobel Gradient to be processed on a vector image
 *
 * \ingroup OTBEdge
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VerticalSobelVectorImageFilter
    : public UnaryFunctorNeighborhoodVectorImageFilter<
          TInputImage, TOutputImage, Functor::VerticalSobelOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs */
  typedef VerticalSobelVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter<
      TInputImage, TOutputImage, Functor::VerticalSobelOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VerticalSobelVectorImageFilter, ImageToImageFilter);

protected:
  VerticalSobelVectorImageFilter()
  {
    typename Superclass::RadiusType radius = {{1, 1}};
    this->SetRadius(radius);
  }
  ~VerticalSobelVectorImageFilter() override
  {
  }

private:
  VerticalSobelVectorImageFilter(const Self&); // Not implemented
  void operator=(const Self&);                 // Not implemented
};                                             // end of class

} // end of namespace otb


#endif // otbVerticalSobelVectorImageFilter_h
