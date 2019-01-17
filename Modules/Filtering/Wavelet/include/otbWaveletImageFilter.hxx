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


#ifndef otbWaveletImageFilter_hxx
#define otbWaveletImageFilter_hxx

#include "otbWaveletImageFilter.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::WaveletImageFilter() : m_NumberOfDecompositions(2)
{
  m_WaveletTransform                   = WaveletTransformFilterType::New();
  m_WaveletTransform->SetSubsampleImageFactor(2);

  m_WaveletBandsListToWaveletsSynopsis = WaveletBandsListToWaveletsSynopsisImageFilterType::New();
  m_WaveletBandsListToWaveletsSynopsis->SetInput( m_WaveletTransform->GetOutput() );
}

/** Destructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::~WaveletImageFilter()
{
}

/**
 * GenerateInputRequestedRegion
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input  =
    const_cast<InputImageType *> (this->GetInput());

  if ( !input )
    {
    return;
    }

  input->SetRequestedRegionToLargestPossibleRegion();
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::GenerateData()
{
  m_WaveletTransform->SetInput( this->GetInput() );
  m_WaveletTransform->SetNumberOfDecompositions( m_NumberOfDecompositions );
  m_WaveletBandsListToWaveletsSynopsis->GraftOutput( this->GetOutput() );
  m_WaveletBandsListToWaveletsSynopsis->Update();
  this->GraftOutput( m_WaveletBandsListToWaveletsSynopsis->GetOutput() );
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
