/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbUnaryFunctorVectorImageFilter_txx
#define __otbUnaryFunctorVectorImageFilter_txx

#include "otbUnaryFunctorVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>
::UnaryFunctorVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
}

/**
 * Generate the output information
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(
    this->GetInput()->GetNumberOfComponentsPerPixel() );
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  itk::ImageRegionConstIterator< InputImageType > inputIt ( this->GetInput(), inputRegionForThread );
  inputIt.GoToBegin();

  itk::ImageRegionIterator< OutputImageType > outputIt ( this->GetOutput(), outputRegionForThread );
  outputIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while ( !outputIt.IsAtEnd() && !inputIt.IsAtEnd() )
  {
    outputIt.Set( m_Functor( inputIt.Get() ) );

    ++inputIt;
    ++outputIt;

    progress.CompletedPixel();
  }
}

} // end namespace otb

#endif
