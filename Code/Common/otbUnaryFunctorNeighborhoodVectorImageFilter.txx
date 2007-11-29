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
#ifndef _otbUnaryFunctorNeighborhoodVectorImageFilter_txx
#define _otbUnaryFunctorNeighborhoodVectorImageFilter_txx

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
template <class TInputImage, class TOutputImage, class TFunction  >
UnaryFunctorNeighborhoodVectorImageFilter<TInputImage,TOutputImage,TFunction>
::UnaryFunctorNeighborhoodVectorImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();
  m_Radius = 1;
}


/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage, class TFunction >
void
UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId)
{ 
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  TInputImage * inputPtr = static_cast<TInputImage *>( ProcessObjectType::GetInput(0) );

  RadiusType r;
  r.Fill(m_Radius);
  NeighborhoodIteratorType neighInputIt;

  // This is the main difference from UnaryFunctorNeighborhoodVectorImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId)
  OutputImagePointer outputPtr = this->GetOutput(); 
  outputPtr->SetNumberOfComponentsPerPixel( inputPtr->GetNumberOfComponentsPerPixel() );
  outputPtr->Allocate();

  itk::ImageRegionIterator<TOutputImage> outputIt;

  // Find the data-set boundary "faces"
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> TypeFace;
  typename TypeFace::FaceListType::iterator fit;
  typename TypeFace::FaceListType faceList;
  TypeFace bC;

  faceList = bC( inputPtr, outputRegionForThread, r );

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
 
  // Process each of the boundary faces.  
  // Center first and then left, right, up, down borders
  for ( fit=faceList.begin(); fit != faceList.end(); ++fit )
    { 
      NeighborhoodIteratorType neighInputIt( r, inputPtr, *fit );
      neighInputIt.OverrideBoundaryCondition( &nbc );
      neighInputIt.GoToBegin();
      
      outputIt = itk::ImageRegionIterator<TOutputImage> ( outputPtr, *fit );
      outputIt.GoToBegin();
      
      while ( !outputIt.IsAtEnd() )
	{
	  outputIt.Set( m_Functor( neighInputIt ) );
	  
	  ++neighInputIt;
	  ++outputIt;

	  progress.CompletedPixel();
	}
    }
}

} // end namespace otb

#endif
