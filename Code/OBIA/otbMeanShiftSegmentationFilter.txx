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

   this->SetNumberOfOutputs(2);
   this->SetNthOutput(0, m_RegionMergingFilter->GetLabelOutput());
   this->SetNthOutput(1, m_RegionMergingFilter->GetClusteredOutput());
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::~MeanShiftSegmentationFilter()
{
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
void
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}


template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
TOutputLabelImage *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetLabelOutput()
{
  return static_cast<OutputLabelImageType *>(this->m_RegionMergingFilter->GetLabelOutput());
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
const TOutputLabelImage *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetLabelOutput() const
{
  return static_cast<OutputLabelImageType *>(this->m_RegionMergingFilter->GetLabelOutput());
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
typename MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>::OutputClusteredImageType *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetClusteredOutput()
{
  return static_cast<OutputClusteredImageType *>(this->m_RegionMergingFilter->GetClusteredOutput());
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
const typename MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>::OutputClusteredImageType *
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GetClusteredOutput() const
{
  return static_cast<OutputClusteredImageType *>(this->m_RegionMergingFilter->GetClusteredOutput());
}

template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
void
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  unsigned int numberOfComponentsPerPixel = this->GetInputSpectralImage()->GetNumberOfComponentsPerPixel();

  if(this->GetClusteredOutput())
    {
    this->GetClusteredOutput()->SetNumberOfComponentsPerPixel(numberOfComponentsPerPixel);
    }

}


template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage, class TKernel>
void
MeanShiftSegmentationFilter<TInputImage, TOutputLabelImage, TOutputClusteredImage, TKernel>
::GenerateData()
{

  this->m_MeanShiftFilter->SetInput(this->GetInput());

  this->m_RegionMergingFilter->SetInputLabelImage(this->m_RegionMergingFilter->GetLabelOutput());
  this->m_RegionMergingFilter->SetInputSpectralImage(this->m_RegionMergingFilter->GetRangeOutput());

  this->m_RegionMergingFilter->Update();

  this->GraftNthOutput(0, m_RegionMergingFilter->GetLabelOutput());
  this->GraftNthOutput(1, m_RegionMergingFilter->GetClusteredOutput());
}

} // end namespace otb
#endif
