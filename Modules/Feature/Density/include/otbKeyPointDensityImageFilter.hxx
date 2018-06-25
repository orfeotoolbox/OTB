/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbKeyPointDensityImageFilter_hxx
#define otbKeyPointDensityImageFilter_hxx

#include "otbKeyPointDensityImageFilter.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TDetector>
KeyPointDensityImageFilter<TInputImage, TOutputImage, TDetector>
::KeyPointDensityImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_NeighborhoodRadius = 1;
  m_Detector =  DetectorType::New();
  m_PointSetToDensityImageFilter = PointSetToDensityImageType::New();
}

/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TDetector>
KeyPointDensityImageFilter<TInputImage, TOutputImage, TDetector>
::~KeyPointDensityImageFilter()
{}

/**
 * threaded Generate Data
 */

/**
* GenerateData Performs the pixel-wise addition
*/
template <class TInputImage, class TOutputImage, class TDetector>
void
KeyPointDensityImageFilter<TInputImage, TOutputImage, TDetector>
::GenerateData()
//::GenerateData( const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId )
{
  typename Superclass::OutputImagePointer outputImage = this->GetOutput();
  InputImagePointerType                   ptr = const_cast<InputImageType *>(this->GetInput());
  if (!ptr) return;

  /** Detector*/
  m_Detector->SetInput(ptr);

  /** Applying the pointsetTodensityImageFilter*/
  m_PointSetToDensityImageFilter->SetInput(m_Detector->GetOutput());
  m_PointSetToDensityImageFilter->SetRadius(m_NeighborhoodRadius);
  m_PointSetToDensityImageFilter->SetSpacing(ptr->GetSignedSpacing());
  m_PointSetToDensityImageFilter->SetSize(ptr->GetLargestPossibleRegion().GetSize());
  m_PointSetToDensityImageFilter->SetOrigin(ptr->GetOrigin());
  m_PointSetToDensityImageFilter->Update();

  /** updating the output*/
  this->GraftOutput(m_PointSetToDensityImageFilter->GetOutput());
}

/**
 * Set Detector
 */
template <class TInputImage, class TOutputImage, class TDetector>
void
KeyPointDensityImageFilter<TInputImage, TOutputImage, TDetector>
::SetDetector(DetectorType* detector)
{
  m_Detector = detector;
}

/**
 * Get Detector
 */
template <class TInputImage, class TOutputImage, class TDetector>
typename KeyPointDensityImageFilter<TInputImage,  TOutputImage, TDetector>
::DetectorType *
KeyPointDensityImageFilter<TInputImage, TOutputImage, TDetector>
::GetDetector()
{
  return m_Detector;
}

/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TDetector>
void
KeyPointDensityImageFilter<TInputImage,  TOutputImage, TDetector>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Neighborhood Radius " << m_NeighborhoodRadius   << std::endl;
}

} /** end namesapce otb*/
#endif
