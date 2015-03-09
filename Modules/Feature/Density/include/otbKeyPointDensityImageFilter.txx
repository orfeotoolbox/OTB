/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbKeyPointDensityImageFilter_txx
#define __otbKeyPointDensityImageFilter_txx

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
  m_PointSetToDensityImageFilter->SetSpacing(ptr->GetSpacing());
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
