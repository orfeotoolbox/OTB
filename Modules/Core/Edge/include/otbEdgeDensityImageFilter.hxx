/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbEdgeDensityImageFilter_hxx
#define otbEdgeDensityImageFilter_hxx

#include "otbEdgeDensityImageFilter.h"

namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>::EdgeDensityImageFilter()
{
  this->SetNumberOfRequiredInputs(1);

  m_NeighborhoodRadius.Fill(1);
  m_Detector           = DetectorType::New();
  m_DensityImageFilter = DensityImageType::New();
}

/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>::~EdgeDensityImageFilter()
{
}

/**
 * threaded Generate Data
 */
template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
void EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>::GenerateData()
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
void EdgeDensityImageFilter<TInputImage, TOutputImage, TEdgeDetector, TDensityCount>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Neighborhood Radius " << m_NeighborhoodRadius << std::endl;
}

} /** end namesapce otb*/
#endif
