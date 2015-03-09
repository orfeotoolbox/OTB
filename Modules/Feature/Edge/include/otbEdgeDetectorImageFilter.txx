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
#ifndef __otbEdgeDetectorImageFilter_txx
#define __otbEdgeDetectorImageFilter_txx

#include "otbEdgeDetectorImageFilter.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetection>
EdgeDetectorImageFilter<TInputImage, TOutputImage, TEdgeDetection>
::EdgeDetectorImageFilter()
{
  this->SetNumberOfRequiredInputs(1);

  m_Detector =  DetectionType::New();
  m_BinaryFilter = BinaryFilterType::New();
  m_BinaryFilter->SetInsideValue(static_cast<OutputImagePixelType>(0.));
  m_BinaryFilter->SetOutsideValue(static_cast<OutputImagePixelType>(1.));
  m_BinaryFilter->SetUpperThreshold(static_cast<InputImagePixelType>(255));
  m_BinaryFilter->SetLowerThreshold(static_cast<InputImagePixelType>(0));
}

/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetection>
EdgeDetectorImageFilter<TInputImage, TOutputImage, TEdgeDetection>
::~EdgeDetectorImageFilter()
{}

/**
 * threaded Generate Data
 */
template <class TInputImage, class TOutputImage, class TEdgeDetection>
void
EdgeDetectorImageFilter<TInputImage, TOutputImage, TEdgeDetection>
::GenerateData()
{
  m_Detector->SetInput(this->GetInput());
  m_Detector->Update();

  m_BinaryFilter->SetInput(m_Detector->GetOutput());
  m_BinaryFilter->GraftOutput(this->GetOutput());
  m_BinaryFilter->Update();
  this->GraftOutput(m_BinaryFilter->GetOutput());
}

/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetection>
void
EdgeDetectorImageFilter<TInputImage, TOutputImage, TEdgeDetection>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} /** end namesapce otb*/
#endif
