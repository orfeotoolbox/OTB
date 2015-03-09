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
#ifndef __otbWatershedSegmentationFilter_txx
#define __otbWatershedSegmentationFilter_txx

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
  // we loose the additionnal metadata of OTB like ProjectionRef.
  // Import them before exiting
  this->GetOutput()->CopyInformation( this->GetInput() );
}

} // end namespace otb
#endif
