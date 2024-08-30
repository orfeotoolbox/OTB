/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbShiftScaleVectorImageFilter_hxx
#define otbShiftScaleVectorImageFilter_hxx

#include "otbShiftScaleVectorImageFilter.h"

namespace otb
{
/**
 * Generate output information.
 */
template <class TInputImage, class TOutputImage>
void ShiftScaleVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation(void)
{
  this->Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
}
/**
 * Generate input requested region.
 */
template <class TInputImage, class TOutputImage>
void ShiftScaleVectorImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion(void)
{
  if (this->GetInput())
  {
    typename TInputImage::Pointer    input = const_cast<TInputImage*>(this->GetInput());
    typename TInputImage::RegionType inputRegion;
    this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
    input->SetRequestedRegion(inputRegion);
  }
}
/**
 * Process to execute before entering the multithreaded section.
 */
template <class TInputImage, class TOutputImage>
void ShiftScaleVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  // set up the functor values
  this->GetFunctor().SetScaleValues(m_Scale);
  this->GetFunctor().SetShiftValues(m_Shift);
}

} // end namespace otb
#endif
