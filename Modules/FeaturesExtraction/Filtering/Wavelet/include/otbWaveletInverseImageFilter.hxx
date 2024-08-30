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


#ifndef __otbWaveletInverseImageFilter_hxx
#define __otbWaveletInverseImageFilter_hxx

#include "otbWaveletInverseImageFilter.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::WaveletInverseImageFilter() : m_NumberOfDecompositions(2)
{
  m_SynopsisImageToWaveletsBandsList = WaveletsSynopsisImageToWaveletsBandsListFilterType::New();
  m_SynopsisImageToWaveletsBandsList->SetDecimationRatio(2);

  m_WaveletTransform = WaveletInverseTransformFilterType::New();
  m_WaveletTransform->SetSubsampleImageFactor(2);
  m_WaveletTransform->SetInput(m_SynopsisImageToWaveletsBandsList->GetOutput());
}

/** Destructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::~WaveletInverseImageFilter()
{
}

/** Destructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::Modified() const
{
  Superclass::Modified();
  m_SynopsisImageToWaveletsBandsList->Modified();
  m_WaveletTransform->Modified();
}

/**
 * GenerateInputRequestedRegion
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input = const_cast<InputImageType*>(this->GetInput());

  if (!input)
  {
    return;
  }

  input->SetRequestedRegionToLargestPossibleRegion();
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::GenerateData()
{
  m_SynopsisImageToWaveletsBandsList->SetInput(this->GetInput());
  m_SynopsisImageToWaveletsBandsList->SetNumberOfLevels(m_NumberOfDecompositions);

  m_WaveletTransform->GraftOutput(this->GetOutput());
  m_WaveletTransform->Update();
  this->GraftOutput(m_WaveletTransform->GetOutput());
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
}

#endif
