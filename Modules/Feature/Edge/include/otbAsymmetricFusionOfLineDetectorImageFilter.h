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

#ifndef otbAsymmetricFusionOfLineDetectorImageFilter_h
#define otbAsymmetricFusionOfLineDetectorImageFilter_h

#include "itkImageToImageFilter.h"

#include "otbLineRatioDetectorImageFilter.h"
#include "otbLineCorrelationDetectorImageFilter.h"
#include "otbAssociativeSymmetricalSumImageFilter.h"

namespace otb
{

/** \class AsymmetricFusionOfLineDetectorImageFilter
 *
 * This class implements a composite filter that combines three filters:
 * two filters of line detector ( a line detector by ratio and a line
 * detector by croos-correlation) and a filter that produces a fusion of
 * those two line detector filters.
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TOutputImageDirection = TOutputImage,
          class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage>>
class ITK_EXPORT AsymmetricFusionOfLineDetectorImageFilter : public LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
{
public:
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef AsymmetricFusionOfLineDetectorImageFilter Self;
  typedef LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(AsymmetricFusionOfLineDetectorImageFilter, LineDetectorImageFilterBase);

  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;

  typedef OutputImageType InputImageType1;
  typedef OutputImageType InputImageType2;

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

protected:
  AsymmetricFusionOfLineDetectorImageFilter();
  ~AsymmetricFusionOfLineDetectorImageFilter() override
  {
  }

  typedef otb::LineRatioDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType, InterpolatorType>       LineRatioType;
  typedef otb::LineCorrelationDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType, InterpolatorType> LineCorrelationType;
  typedef otb::AssociativeSymmetricalSumImageFilter<InputImageType1, InputImageType2, OutputImageType> AssSymSumType;

  void GenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  AsymmetricFusionOfLineDetectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  typename LineRatioType::Pointer       m_LineRatio;
  typename LineCorrelationType::Pointer m_LineCorrelation;
  typename AssSymSumType::Pointer       m_AssSymSum;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAsymmetricFusionOfLineDetectorImageFilter.hxx"
#endif

#endif
