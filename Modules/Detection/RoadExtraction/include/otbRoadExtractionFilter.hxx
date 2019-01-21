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

#ifndef otbRoadExtractionFilter_hxx
#define otbRoadExtractionFilter_hxx

#include "otbRoadExtractionFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputPath>
RoadExtractionFilter<TInputImage, TOutputPath>
::RoadExtractionFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_SpectralAngleDistanceImageFilter = SpectralAngleDistanceImageFilterType::New();
  m_GenericRoadExtractionFilter = GenericRoadExtractionFilterType::New();

}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // Input images pointers
  typename InputImageType::ConstPointer inputImage     = this->GetInput();
  typename OutputPathListType::Pointer  outputPathList  = this->GetOutput();

  m_SpectralAngleDistanceImageFilter->SetInput(inputImage);

  m_GenericRoadExtractionFilter->SetInput(m_SpectralAngleDistanceImageFilter->GetOutput());

  m_GenericRoadExtractionFilter->Update();
  for (typename GenericRoadExtractionFilterType::OutputPathListType::ConstIterator it
         = m_GenericRoadExtractionFilter->GetOutput()->Begin();
       it != m_GenericRoadExtractionFilter->GetOutput()->End();
       ++it)
    {
    outputPathList->PushBack(it.Get());
    }
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "m_ReferencePixel: " << m_SpectralAngleDistanceImageFilter->GetReferencePixel() << std::endl;
}

} // End namespace otb
#endif
