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

#ifndef otbComplexToIntensityImageFilter_h
#define otbComplexToIntensityImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class ComplexToIntensityImageFilter
 * \brief Computes pixel-wise the intensity of a complex image.
 *
 * \sa ComplexToPhaseImageFilter ComplexToModulusImageFilter
 * \ingroup IntensityImageFilters  Multithreaded
 *
 * \ingroup OTBCommon
 */

namespace Function
{

template <class TInput, class TOutput>
class ComplexToIntensity
{
public:
  ComplexToIntensity()
  {
  }
  ~ComplexToIntensity()
  {
  }
  bool operator!=(const ComplexToIntensity&) const
  {
    return false;
  }
  bool operator==(const ComplexToIntensity& other) const
  {
    return !(*this != other);
  }
  inline TOutput operator()(const TInput& A) const
  {
    return static_cast<TOutput>(A.real() * A.real() + A.imag() * A.imag());
  }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComplexToIntensityImageFilter
    : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                          Function::ComplexToIntensity<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef ComplexToIntensityImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                       Function::ComplexToIntensity<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexToIntensityImageFilter, UnaryFunctorImageFilter);

  typedef typename TInputImage::PixelType                        InputPixelType;
  typedef typename TOutputImage::PixelType                       OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::ValueType InputPixelValueType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck, (itk::Concept::Convertible<InputPixelValueType, OutputPixelType>));
/** End concept checking */
#endif


protected:
  ComplexToIntensityImageFilter()
  {
  }
  ~ComplexToIntensityImageFilter() override
  {
  }

private:
  ComplexToIntensityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
