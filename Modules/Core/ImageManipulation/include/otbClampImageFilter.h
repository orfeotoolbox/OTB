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

#ifndef otbClampImageFilter_h
#define otbClampImageFilter_h

#include "otbConvertTypeFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class ClampImageFilter
 * \brief Clamp image values to be below, over, or between threshold values.
 *
 * ClampImageFilter  clamp image values to be between an upper
 * and lower value. Values lower than m_Lower values are set to lower,
 * and values greater than upper threshold are set to upper threshold
 * value.
 * This filter can also be used to cast any type of image into any other type
 * as long as those types are arithmetics or complex.
 *
 * By default lower and upper thresholds are set to the maximum and
 * minimum bounds of the image internal pixel value.
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT ClampImageFilter
: public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  using Self         = ClampImageFilter;
  using Superclass   = itk::ImageToImageFilter<TInputImage, TOutputImage>;
  using FunctorType  = Functor::ConvertTypeFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClampImageFilter, OSEF);


  /** Some additional typedefs.  */
  using InputImageType          = TInputImage;
  using InputImageRegionType    = typename InputImageType::RegionType;
  using InputImagePixelType     = typename InputImageType::PixelType;

  /** Some additional typedefs.  */
  using OutputImageType         = TOutputImage;
  using OutputImageRegionType   = typename OutputImageType::RegionType;
  using OutputImagePixelType    = typename OutputImageType::PixelType;
  using OutputInternalPixelType = typename OutputImageType::InternalPixelType;
  using OutputPixelValueType    = typename itk::NumericTraits<OutputInternalPixelType>::ValueType;

  /** The values greater than or equal to the value are set to \p thresh. */
  void ClampAbove(const OutputPixelValueType& thresh);

  /** The values less than or equal to the value are set to \p thresh. */
  void ClampBelow(const OutputPixelValueType& thresh);

  /** The values outside the range are set to \p lower or \p upper. */
  void ClampOutside(const OutputPixelValueType& lower, const OutputPixelValueType& upper);

  /** Set/Get methods to set the lower and upper threshold */
  void SetThresholds(OutputPixelValueType lowerVal, OutputPixelValueType upperVal);
  itkGetConstMacro(Lower, OutputPixelValueType);
  itkGetConstMacro(Upper, OutputPixelValueType);


protected:
  ClampImageFilter();
  ~ClampImageFilter() override = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateOutputInformation(void) override
  {
    Superclass::GenerateOutputInformation();
    unsigned int sizeIn = this->GetInput()->GetNumberOfComponentsPerPixel();
    this->GetFunctor().SetInputComponents(sizeIn);
    this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetFunctor().GetOutputSize());
  }

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  FunctorType      & GetFunctor()       noexcept { return m_Functor; }
  FunctorType const& GetFunctor() const noexcept { return m_Functor; }

private:
  ClampImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  OutputPixelValueType m_Lower;
  OutputPixelValueType m_Upper;
  FunctorType          m_Functor;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClampImageFilter.hxx"
#endif

#endif
