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
#ifndef __otbEdgeDensityImageFilter_txx
#define __otbEdgeDensityImageFilter_txx

#include "otbEdgeDensityImageFilter.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>
::EdgeDensityImageFilter()
{
  this->SetNumberOfRequiredInputs(1);

  m_NeighborhoodRadius.Fill(1);
  m_Detector =  DetectorType::New();
  m_DensityImageFilter = DensityImageType::New();
}

/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>
::~EdgeDensityImageFilter()
{}

/**
 * threaded Generate Data
 */
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
void
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>
::GenerateData()
{
  m_Detector->SetInput(this->GetInput());

  m_DensityImageFilter->SetNeighborhoodRadius(m_NeighborhoodRadius);
  m_DensityImageFilter->SetInput(m_Detector->GetOutput());

  m_DensityImageFilter->GraftOutput(this->GetOutput());
  m_DensityImageFilter->Update();
  this->GraftOutput(m_DensityImageFilter->GetOutput());
}

/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
void
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Neighborhood Radius " << m_NeighborhoodRadius   << std::endl;
}

} /** end namesapce otb*/
#endif
