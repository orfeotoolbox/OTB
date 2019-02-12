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

#ifndef otbMeanShiftSegmentationFilter_hxx
#define otbMeanShiftSegmentationFilter_hxx

#include "otbMeanShiftSegmentationFilter.h"

namespace otb {

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::MeanShiftSegmentationFilter()
{
   m_MeanShiftFilter = MeanShiftFilterType::New();
   m_RegionMergingFilter = RegionMergingFilterType::New();
   m_RegionPruningFilter = RegionPruningFilterType::New();
   m_RelabelFilter = RelabelComponentFilterType::New();
   this->SetMinRegionSize(100);
   this->SetNumberOfRequiredOutputs(2);
   this->SetNthOutput(0,TOutputLabelImage::New());
   this->SetNthOutput(1,TOutputClusteredImage::New());
   // default set MeanShiftFilter mode search to true because Merging and Pruning use LabelOutput
   m_MeanShiftFilter->SetModeSearch(true);
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::~MeanShiftSegmentationFilter()
{}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
TOutputLabelImage *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetLabelOutput()
{
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
const TOutputLabelImage *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetLabelOutput() const
{
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
typename MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>::OutputClusteredImageType *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetClusteredOutput()
{
  return static_cast<OutputClusteredImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
const typename MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>::OutputClusteredImageType *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetClusteredOutput() const
{
  return static_cast<OutputClusteredImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
void
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GenerateData()
{
  this->m_MeanShiftFilter->SetInput(this->GetInput());

  // Relabel output to avoid same label assigned to discontinuous areas

  m_RelabelFilter->SetInput(this->m_MeanShiftFilter->GetLabelOutput());
  this->m_RegionMergingFilter->SetInputLabelImage(this->m_RelabelFilter->GetOutput());

  this->m_RegionMergingFilter->SetInputSpectralImage(this->m_MeanShiftFilter->GetRangeOutput());
  this->m_RegionMergingFilter->SetRangeBandwidth(this->GetRangeBandwidth());
  if (this->GetMinRegionSize() == 0)
    {

    m_RegionMergingFilter->GraftNthOutput(0, this->GetLabelOutput());
    m_RegionMergingFilter->GraftNthOutput(1, this->GetClusteredOutput());
    this->m_RegionMergingFilter->Update();
    this->GraftNthOutput(0, m_RegionMergingFilter->GetLabelOutput());
    this->GraftNthOutput(1, m_RegionMergingFilter->GetClusteredOutput());
    }
  else
    {

    this->m_RegionPruningFilter->SetInputLabelImage(this->m_RegionMergingFilter->GetLabelOutput());
    this->m_RegionPruningFilter->SetInputSpectralImage(this->m_RegionMergingFilter->GetClusteredOutput());
    m_RegionPruningFilter->GraftNthOutput(0, this->GetLabelOutput());
    m_RegionPruningFilter->GraftNthOutput(1, this->GetClusteredOutput());

    this->m_RegionPruningFilter->Update();
    this->GraftNthOutput(0, m_RegionPruningFilter->GetLabelOutput());
    this->GraftNthOutput(1, m_RegionPruningFilter->GetClusteredOutput());
    }
}

} // end namespace otb
#endif
