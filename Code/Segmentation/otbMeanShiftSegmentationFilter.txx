/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMeanShiftSegmentationFilter_txx
#define __otbMeanShiftSegmentationFilter_txx

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
   this->SetNumberOfOutputs(2);
   this->SetNthOutput(0,TOutputLabelImage::New());
   this->SetNthOutput(1,TOutputClusteredImage::New());
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
