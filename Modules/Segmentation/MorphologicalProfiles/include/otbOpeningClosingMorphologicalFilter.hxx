/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOpeningClosingMorphologicalFilter_hxx
#define otbOpeningClosingMorphologicalFilter_hxx

#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"
#include "itkProgressAccumulator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TKernel>
OpeningClosingMorphologicalFilter<TInputImage, TOutputImage, TKernel>::OpeningClosingMorphologicalFilter()
{
  m_Kernel.SetRadius(1);
  m_Kernel.CreateStructuringElement();
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TKernel>
void OpeningClosingMorphologicalFilter<TInputImage, TOutputImage, TKernel>::GenerateData()
{
  // Filters Typedefs (this class is actually a composite filter)
  typedef itk::GrayscaleMorphologicalOpeningImageFilter<InputImageType, OutputImageType, KernelType> OpenFilterType;
  typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType, OutputImageType, KernelType> CloseFilterType;
  // Filters initialization
  typename OpenFilterType::Pointer  opening = OpenFilterType::New();
  typename CloseFilterType::Pointer closing = CloseFilterType::New();
  // Set the kernel
  opening->SetKernel(this->GetKernel());
  closing->SetKernel(this->GetKernel());
  // Progression report management
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(opening, .5f);
  progress->RegisterInternalFilter(closing, .5f);
  // Internal Pipeline connexion
  closing->SetInput(this->GetInput());
  opening->SetInput(closing->GetOutput());
  // Output connexion
  opening->GraftOutput(this->GetOutput());
  opening->Update();
  this->GraftOutput(opening->GetOutput());
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage, class TKernel>
void OpeningClosingMorphologicalFilter<TInputImage, TOutputImage, TKernel>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Kernel: " << m_Kernel << std::endl;
}
} // End namespace otb
#endif
