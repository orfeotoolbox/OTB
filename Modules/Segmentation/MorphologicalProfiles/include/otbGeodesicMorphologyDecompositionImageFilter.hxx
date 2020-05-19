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

#ifndef otbGeodesicMorphologyDecompositionImageFilter_hxx
#define otbGeodesicMorphologyDecompositionImageFilter_hxx

#include "otbGeodesicMorphologyDecompositionImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>::GeodesicMorphologyDecompositionImageFilter()
{
  this->SetNumberOfRequiredOutputs(3);
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());

  m_Radius.Fill(1);

  // Create a process accumulator for tracking the progress of minipipeline
  m_Progress = itk::ProgressAccumulator::New();
  m_Progress->SetMiniPipelineFilter(this);

  m_OpeningFilter  = OpeningFilterType::New();
  m_ClosingFilter  = ClosingFilterType::New();
  m_LevelingFilter = LevelingFilterType::New();
  m_ConvexFilter   = ConvexFilterType::New();
  m_ConcaveFilter  = ConcaveFilterType::New();

  // Register Internal Filter for progress
  m_Progress->RegisterInternalFilter(m_OpeningFilter, 0.2);
  m_Progress->RegisterInternalFilter(m_ClosingFilter, 0.2);
  m_Progress->RegisterInternalFilter(m_LevelingFilter, 0.2);
  m_Progress->RegisterInternalFilter(m_ConvexFilter, 0.2);
  m_Progress->RegisterInternalFilter(m_ConcaveFilter, 0.2);


  m_FullyConnected      = true;
  m_PreserveIntensities = true;
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
void GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>::GenerateData()
{
  StructuringElementType se;
  se.SetRadius(m_Radius);
  se.CreateStructuringElement();

  m_OpeningFilter->SetInput(this->GetInput());
  m_OpeningFilter->SetKernel(se);
  m_OpeningFilter->SetPreserveIntensities(m_PreserveIntensities);
  m_OpeningFilter->SetFullyConnected(m_FullyConnected);

  m_ClosingFilter->SetInput(this->GetInput());
  m_ClosingFilter->SetKernel(se);
  m_ClosingFilter->SetPreserveIntensities(m_PreserveIntensities);
  m_ClosingFilter->SetFullyConnected(m_FullyConnected);

  m_ConvexFilter->SetInput1(this->GetInput());
  m_ConvexFilter->SetInput2(m_OpeningFilter->GetOutput());

  m_ConcaveFilter->SetInput1(m_ClosingFilter->GetOutput());
  m_ConcaveFilter->SetInput2(this->GetInput());

  using namespace otb::Functor::LevelingFunctor_tags;
  // Template keyword mandatory to avoid parsing error when using
  // template methods within template code
  m_LevelingFilter->template SetInput<pixel>(this->GetInput());
  m_LevelingFilter->template SetInput<convex_pixel>(m_ConvexFilter->GetOutput());
  m_LevelingFilter->template SetInput<concave_pixel>(m_ConcaveFilter->GetOutput());

  m_ConvexFilter->GraftOutput(this->GetConvexMap());
  m_ConvexFilter->Update();
  this->GraftNthOutput(1, m_ConvexFilter->GetOutput());

  m_ConcaveFilter->GraftOutput(this->GetConcaveMap());
  m_ConcaveFilter->Update();
  this->GraftNthOutput(2, m_ConcaveFilter->GetOutput());

  m_LevelingFilter->GraftOutput(this->GetOutput());
  m_LevelingFilter->Update();
  this->GraftOutput(m_LevelingFilter->GetOutput());
}

/**
 * Get the convex map
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
TOutputImage* GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>::GetConvexMap()
{
  if (this->GetNumberOfOutputs() < 2)
  {
    return nullptr;
  }
  return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(1));
}

/**
 * Get the concave map
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
TOutputImage* GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>::GetConcaveMap()
{
  if (this->GetNumberOfOutputs() < 3)
  {
    return nullptr;
  }
  return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(2));
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
void GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
