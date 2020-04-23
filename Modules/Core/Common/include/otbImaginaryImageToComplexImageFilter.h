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

#ifndef otbImaginaryImageToComplexImageFilter_h
#define otbImaginaryImageToComplexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class ImaginaryImageToComplexImageFilter
 * \brief Computes pixel-wise the complex image of a imaginary image.
 *
 * \sa ImaginaryImageToComplexImageFilter
 * \ingroup Multithreaded
 *
 * \ingroup OTBCommon
 */

namespace Function
{

template <class TInputImageImaginaryPart, class TOutput>
class ImaginaryToComplex
{
public:
  typedef typename TOutput::value_type OutputValueType;

  ImaginaryToComplex()
  {
  }
  ~ImaginaryToComplex()
  {
  }
  bool operator!=(const ImaginaryToComplex&) const
  {
    return false;
  }
  bool operator==(const ImaginaryToComplex& other) const
  {
    return !(*this != other);
  }
  inline TOutput operator()(const TInputImageImaginaryPart& imaginary) const
  {
    return TOutput(static_cast<OutputValueType>(0.0), static_cast<OutputValueType>(imaginary));
  }
};
}

template <class TInputImageImaginaryPart, class TOutputImage>
class ITK_EXPORT ImaginaryImageToComplexImageFilter
    : public itk::UnaryFunctorImageFilter<TInputImageImaginaryPart, TOutputImage,
                                          Function::ImaginaryToComplex<typename TInputImageImaginaryPart::PixelType, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef ImaginaryImageToComplexImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImageImaginaryPart, TOutputImage,
                                       Function::ImaginaryToComplex<typename TInputImageImaginaryPart::PixelType, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ImaginaryImageToComplexImageFilter, UnaryFunctorImageFilter);

  typedef typename TInputImageImaginaryPart::PixelType            InputImaginaryPartPixelType;
  typedef typename TOutputImage::PixelType                        OutputPixelType;
  typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputPixelValueType;

protected:
  ImaginaryImageToComplexImageFilter()
  {
  }
  ~ImaginaryImageToComplexImageFilter() override
  {
  }

private:
  ImaginaryImageToComplexImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
