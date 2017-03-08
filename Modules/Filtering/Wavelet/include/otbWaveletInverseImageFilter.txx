/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletInverseImageFilter_txx
#define __otbWaveletInverseImageFilter_txx

#include "otbWaveletInverseImageFilter.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::WaveletInverseImageFilter() : m_NumberOfDecompositions(2)
{
  m_SynopsisImageToWaveletsBandsList = WaveletsSynopsisImageToWaveletsBandsListFilterType::New();
  m_SynopsisImageToWaveletsBandsList->SetDecimationRatio(2);

  m_WaveletTransform = WaveletInverseTransformFilterType::New();
  m_WaveletTransform->SetSubsampleImageFactor(2);
  m_WaveletTransform->SetInput( m_SynopsisImageToWaveletsBandsList->GetOutput() );

}

/** Destructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::~WaveletInverseImageFilter()
{
}

/** Destructor */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::Modified() const
{
  Superclass::Modified();
  m_SynopsisImageToWaveletsBandsList->Modified();
  m_WaveletTransform->Modified();
}

/**
 * GenerateInputRequestedRegion
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
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
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::GenerateData()
{
  m_SynopsisImageToWaveletsBandsList->SetInput( this->GetInput() );
  m_SynopsisImageToWaveletsBandsList->SetNumberOfLevels( m_NumberOfDecompositions );

  m_WaveletTransform->GraftOutput( this->GetOutput() );
  m_WaveletTransform->Update();
  this->GraftOutput( m_WaveletTransform->GetOutput() );
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletInverseImageFilter<TInputImage, TOutputImage, TMotherWaveletOperator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
