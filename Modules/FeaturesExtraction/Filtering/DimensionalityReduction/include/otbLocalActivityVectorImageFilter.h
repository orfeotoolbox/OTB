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

#ifndef otbLocalActivityVectorImageFilter_h
#define otbLocalActivityVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

#include <itkNumericTraits.h>

namespace otb
{

namespace Functor
{
/** \class LocalActivityOperator
 * \brief Performs the calculation of LocalActivity derivation
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInput, class TOutput>
class LocalActivityOperator
{
public:
  LocalActivityOperator()
  {
  }
  virtual ~LocalActivityOperator()
  {
  }

  TOutput operator()(const TInput& input)
  {
    /*
     * it is assumed that input and output have the same size
     */
    typename TInput::PixelType centerPixel = input.GetCenterPixel();

    unsigned int neighborSize = input.Size();
    unsigned int vectorSize   = centerPixel.Size();

    if (neighborSize == 1)
      return centerPixel;

    TOutput output(vectorSize);

    for (unsigned int i = 0; i < vectorSize; ++i)
    {
      typename TOutput::ValueType out = itk::NumericTraits<typename TOutput::ValueType>::Zero;
      for (unsigned int j = 0; j < neighborSize / 2; ++j)
      {
        out += input.GetPixel(j)[i];
      }
      for (unsigned int j = neighborSize / 2 + 1; j < neighborSize; ++j)
      {
        out += input.GetPixel(j)[i];
      }

      output[i] = centerPixel[i] - out / static_cast<double>(neighborSize - 1);
    }

    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class LocalActivityVectorImageFilter
 * \brief Implements the  LocalActivity Gradient to be processed on a vector image
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LocalActivityVectorImageFilter
    : public UnaryFunctorNeighborhoodVectorImageFilter<
          TInputImage, TOutputImage, Functor::LocalActivityOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs */
  typedef LocalActivityVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter<
      TInputImage, TOutputImage, Functor::LocalActivityOperator<typename itk::ConstNeighborhoodIterator<TInputImage>, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LocalActivityVectorImageFilter, ImageToImageFilter);

protected:
  LocalActivityVectorImageFilter()
  {
  }
  ~LocalActivityVectorImageFilter() override
  {
  }

private:
  LocalActivityVectorImageFilter(const Self&); // Not implemented
  void operator=(const Self&);                 // Not implemented
};                                             // end of class

} // end of namespace otb


#endif // otbLocalActivityVectorImageFilter_h
