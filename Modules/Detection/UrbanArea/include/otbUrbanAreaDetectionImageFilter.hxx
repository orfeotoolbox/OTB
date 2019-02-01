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

#ifndef otbUrbanAreaDetectionImageFilter_hxx
#define otbUrbanAreaDetectionImageFilter_hxx

#include "otbUrbanAreaDetectionImageFilter.h"
#include "itkProgressAccumulator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::UrbanAreaDetectionImageFilter()
{

  m_IntensityFilter = IntensityFilterType::New();
  m_EdgeDensityFilter = EdgeDensityFilterType::New();
  m_SobelFilter = SobelDetectorType::New();
  m_Thresholder = ThresholdFilterType::New();
  m_UrbanAreaExtractionFilter = UrbanAreaExtractionFilterType::New();

  m_MultiplyFilter = MultiplyImageFilterType::New();

  /** Init the Pipeline */
  // EdgeDensity
  m_EdgeDensityFilter->SetInput(m_IntensityFilter->GetOutput());
  m_EdgeDensityFilter->SetDetector(m_SobelFilter);
  // Threshold
  m_Thresholder->SetInput(m_EdgeDensityFilter->GetOutput());
  // Mask Image
  m_MultiplyFilter->SetInput2(m_Thresholder->GetOutput());
  // NonVegetationNonWaterIndex
  m_UrbanAreaExtractionFilter->SetInput(m_MultiplyFilter->GetOutput());

  m_ThresholdValue = 0.5;
  m_ThresholdDensity = 0.1;
  m_SobelLowerThreshold = -100.0;
  m_SobelUpperThreshold = 200.0;
}

/**
 * Parameters the filters of the pipeline
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateData()
{

  // Intensity
  m_IntensityFilter->SetInput(this->GetInput());

  // Edge Density
  m_SobelFilter->SetLowerThreshold(m_SobelLowerThreshold);
  m_SobelFilter->SetUpperThreshold(m_SobelUpperThreshold);
  SizeType lSize;
  lSize[0] = static_cast<unsigned int>(10);
  lSize[1] = static_cast<unsigned int>(10);
  m_EdgeDensityFilter->SetNeighborhoodRadius(lSize);

  // Threshold
  m_Thresholder->SetInsideValue(0);
  m_Thresholder->SetOutsideValue(1);
  m_Thresholder->SetLowerThreshold(-10.);
  m_Thresholder->SetUpperThreshold(m_ThresholdDensity);

  // Apply the mask on the input image
  m_MultiplyFilter->SetInput1(this->GetInput());

  // Give a threshold to urbanAreaFilter
  m_UrbanAreaExtractionFilter->GetFunctor().SetLowerThreshold(m_ThresholdValue);

  // Progress accumulator
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(m_UrbanAreaExtractionFilter, 0.5f);

  /** GraftOutput */
  m_UrbanAreaExtractionFilter->GraftOutput(this->GetOutput());
  m_UrbanAreaExtractionFilter->Update();
  this->GraftOutput(m_UrbanAreaExtractionFilter->GetOutput());

}

} // end namespace otb

#endif
