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
#ifndef __otbUrbanAreaDetectionFilter_txx
#define __otbUrbanAreaDetectionFilter_txx

#include "otbUrbanAreaDetectionImageFilter.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::UrbanAreaDetectionImageFilter()
{
  m_UrbanAreaExtractionFilter = UrbanAreaExtractionFilterType::New();
  m_ThresholdValue = 0.5;
}

/**
 * Init the pipeline
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateData()
{
  // Parameters for the UrbanAreaExtractionFilter
  m_UrbanAreaExtractionFilter->SetInput(this->GetInput());

  this->SetThreshold(m_ThresholdValue);

  m_UrbanAreaExtractionFilter->Update();



  /** GraftOutput */
  this->GraftOutput( m_UrbanAreaExtractionFilter->GetOutput() );

}

} // end namespace otb


#endif
