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

#ifndef otbWatershedSegmentationFilter_hxx
#define otbWatershedSegmentationFilter_hxx

#include "otbWatershedSegmentationFilter.h"

namespace otb {

template <class TInputImage,  class TOutputLabelImage>
WatershedSegmentationFilter<TInputImage, TOutputLabelImage>
::WatershedSegmentationFilter()
{
   m_WatershedFilter = WatershedFilterType::New();
   m_CastFilter      = CastImageFilterType::New();
   m_CastFilter->SetInput(m_WatershedFilter->GetOutput());
   this->SetNthOutput(0,TOutputLabelImage::New());
}

template <class TInputImage,  class TOutputLabelImage>
WatershedSegmentationFilter<TInputImage, TOutputLabelImage>
::~WatershedSegmentationFilter()
{}


template <class TInputImage,  class TOutputLabelImage>
void
WatershedSegmentationFilter<TInputImage, TOutputLabelImage>
::GenerateData()
{
  this->m_WatershedFilter->SetInput(this->GetInput());
  m_CastFilter->GraftOutput(this->GetOutput());
  m_CastFilter->Update();
  this->GraftOutput(m_CastFilter->GetOutput());

  // Since WatershedFilterType outputs an itk::Image,
  // we loose the additional metadata of OTB like ProjectionRef.
  // Import them before exiting
  this->GetOutput()->CopyInformation( this->GetInput() );
}

} // end namespace otb
#endif
