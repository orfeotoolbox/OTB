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

#ifndef otbPerBandVectorImageFilter_hxx
#define otbPerBandVectorImageFilter_hxx

#include "otbPerBandVectorImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFilter>
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>::PerBandVectorImageFilter()
{
  m_Filter      = FilterType::New();
  m_OutputIndex = 0;
}

template <class TInputImage, class TOutputImage, class TFilter>
void PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>::GenerateOutputInformation()
{
  if (this->GetInput())
  {
    // Create a false monoband image
    typename InputImageType::Pointer dummyInputImage = InputImageType::New();
    dummyInputImage->CopyInformation(this->GetInput());
    dummyInputImage->SetNumberOfComponentsPerPixel(1);
    m_Filter->SetInput(dummyInputImage);
    m_Filter->UpdateOutputInformation();
    this->GetOutput()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
    this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
    
    // Override default metadata copying behavior and copy all metadata from input to output.
    this->GetOutput()->SetImageMetadata(this->GetInput()->GetImageMetadata());
  }
}

template <class TInputImage, class TOutputImage, class TFilter>
void PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>::GenerateInputRequestedRegion()
{
  InputVectorImageType*            inputPtr        = const_cast<InputVectorImageType*>(this->GetInput());
  typename InputImageType::Pointer dummyInputImage = InputImageType::New();
  dummyInputImage->CopyInformation(this->GetInput());
  dummyInputImage->SetNumberOfComponentsPerPixel(1);
  m_Filter->SetInput(dummyInputImage);
  m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_Filter->PropagateRequestedRegion(this->GetOutput());
  inputPtr->SetRequestedRegion(m_Filter->GetInput()->GetRequestedRegion());
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>::GenerateData()
{
  InputVectorImageType*          inputPtr   = const_cast<InputVectorImageType*>(this->GetInput());
  OutputVectorImagePointerType   outputPtr  = this->GetOutput();
  DecompositionFilterPointerType decomposer = DecompositionFilterType::New();
  ProcessingFilterPointerType    processor  = ProcessingFilterType::New();
  RecompositionFilterPointerType recomposer = RecompositionFilterType::New();

  inputPtr->UpdateOutputData();

  decomposer->SetInput(this->GetInput());
  processor->SetInput(decomposer->GetOutput());
  processor->SetFilter(m_Filter);
  processor->SetOutputIndex(m_OutputIndex);
  recomposer->SetInput(processor->GetOutput());
  recomposer->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());

  recomposer->GraftOutput(this->GetOutput());
  recomposer->Update();
  this->GraftOutput(recomposer->GetOutput());
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
