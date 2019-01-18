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

#ifndef otbEdgeDetectorImageFilter_hxx
#define otbEdgeDetectorImageFilter_hxx

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
