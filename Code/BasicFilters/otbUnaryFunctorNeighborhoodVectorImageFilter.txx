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
#ifndef __otbUnaryFunctorNeighborhoodVectorImageFilter_txx
#define __otbUnaryFunctorNeighborhoodVectorImageFilter_txx

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage, TFunction>
::UnaryFunctorNeighborhoodVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
  for ( unsigned int i = 0; i < InputImageDimension; ++i )
    m_Radius[i] = 3;
}

/**
 * Generate the output information
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage, TFunction>
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
UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  NeighborhoodIteratorType neighInputIt ( this->GetRadius(), this->GetInput(),
    inputRegionForThread );
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  neighInputIt.OverrideBoundaryCondition(&nbc);
  neighInputIt.GoToBegin();

  itk::ImageRegionIterator<TOutputImage> outputIt ( this->GetOutput(), outputRegionForThread );
  outputIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while ( !outputIt.IsAtEnd() && !neighInputIt.IsAtEnd() )
  {
    outputIt.Set( m_Functor( neighInputIt ) );

    ++neighInputIt;
    ++outputIt;

    progress.CompletedPixel();
  }
}

} // end namespace otb

#endif
