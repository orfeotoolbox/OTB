/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef otbWaveletImageFilter_txx
#define otbWaveletImageFilter_txx

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
