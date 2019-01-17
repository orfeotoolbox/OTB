/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentVectorizationImageFilter_hxx
#define otbPersistentVectorizationImageFilter_hxx

#include "otbPersistentVectorizationImageFilter.h"

#include "itkProgressReporter.h"

namespace otb
{

template<class TInputImage, class TOutputPath>
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::PersistentVectorizationImageFilter()
{
  m_MinMaxFilter = MinMaxFilterType::New();
  m_PathList = PathListType::New();
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::Reset()
{
  m_PathList->Clear();
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::Synthetize()
{}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // Compute the min max and handle mini-pipeline
  m_MinMaxFilter->SetInput(this->GetInput());
  m_MinMaxFilter->GraftOutput(this->GetOutput());
  m_MinMaxFilter->Update();
  this->GraftOutput(m_MinMaxFilter->GetOutput());

  for (PixelType label = m_MinMaxFilter->GetMinimum() + 1; label <= m_MinMaxFilter->GetMaximum(); ++label)
    {
    ImageToEdgePathFilterPointerType edgeFilter = ImageToEdgePathFilterType::New();
    edgeFilter->SetInput(m_MinMaxFilter->GetOutput());
    edgeFilter->SetForegroundValue(label);
    edgeFilter->Update();
    m_PathList->PushBack(edgeFilter->GetOutput());
    }
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
