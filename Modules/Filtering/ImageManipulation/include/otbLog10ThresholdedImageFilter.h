/*
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

#ifndef otbLog10ThresholdedImageFilter_h
#define otbLog10ThresholdedImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{
/** \class Log10ThresholdedImageFilter
 * \brief Computes the 10 * std::log10(x) pixel-wise with a threshold for the lowest values
 *
 * \ingroup OTBImageManipulation
 */
namespace Functor
{
template <class TInput, class TOutput>
class Log10Thresholded
{
public:
  Log10Thresholded()
  {
    m_ThresholdValue = 0.001; // -> -30dB
  }
  ~Log10Thresholded()
  {
  }

  bool operator!=(const Log10Thresholded&) const
  {
    return false;
  }
  bool operator==(const Log10Thresholded& other) const
  {
    return !(*this != other);
  }
  inline TOutput operator()(const TInput& A) const
  {
    if ((static_cast<double>(A) >= m_ThresholdValue) && (A == A))
    {
      return static_cast<TOutput>(10.0 * std::log10(static_cast<double>(A)));
    }
    else // If (A != A) then A is a NaN
    {
      return static_cast<TOutput>(10.0 * std::log10(m_ThresholdValue));
    }
  }

  /** Set ThresholdValue method */
  void SetThresholdValue(const double value)
  {
    if (value > 0.0)
    {
      m_ThresholdValue = value;
    }
  }

private:
  double m_ThresholdValue;
};
} // End namespace Functor

template <class TInputImage, class TOutputImage>
class ITK_EXPORT Log10ThresholdedImageFilter
    : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                          Functor::Log10Thresholded<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef Log10ThresholdedImageFilter Self;
  typedef Functor::Log10Thresholded<typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(Log10ThresholdedImageFilter, itk::UnaryFunctorImageFilter);

  void SetThresholdedValue(double val)
  {
    this->GetFunctor().SetThresholdValue(val);
  }

protected:
  Log10ThresholdedImageFilter()
  {
  }
  ~Log10ThresholdedImageFilter() override
  {
  }

private:
  Log10ThresholdedImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
