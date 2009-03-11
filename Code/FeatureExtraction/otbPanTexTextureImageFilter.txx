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
#ifndef __otbPanTexTextureImageFilter_txx
#define __otbPanTexTextureImageFilter_txx

#include "otbPanTexTextureImageFilter.h"


namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
PanTexTextureImageFilter<TInputImage,TOutputImage>
::PanTexTextureImageFilter()
{
  this->SetRadius( 4 );
  OffsetType off; off.Fill(2);
  this->SetOffset( off );
}



/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage>
void
PanTexTextureImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId)
{
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbcOff;
  // We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImage so it cannot be used for the second input.
  typename InputImageType::ConstPointer inputPtr = dynamic_cast<const TInputImage*>(ProcessObjectType::GetInput(0));
  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionIterator<TOutputImage> outputIt;
 
  // Neighborhood+offset iterator
  RadiusType rOff;
  rOff[0] = this->GetRadius()[0] + vcl_abs(this->GetOffset()[0]);
  rOff[1] = this->GetRadius()[1] + vcl_abs(this->GetOffset()[1]);
  NeighborhoodIteratorType neighInputOffIt;
  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType faceListOff;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage> bCOff;
  faceListOff = bCOff(inputPtr, outputRegionForThread, rOff);
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fitOff;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  fitOff=faceListOff.begin();

  while ( fitOff!=faceListOff.end() )
  {
    // Neighborhood+offset iterator
    neighInputOffIt = itk::ConstNeighborhoodIterator<TInputImage>(rOff, inputPtr, *fitOff);
    neighInputOffIt.OverrideBoundaryCondition(&nbcOff);
    neighInputOffIt.GoToBegin();

    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fitOff);

    while ( ! outputIt.IsAtEnd() )
    {

      outputIt.Set( this->m_FunctorList[threadId]( neighInputOffIt.GetNeighborhood() ) );

      ++neighInputOffIt;
      ++outputIt;
      progress.CompletedPixel();
    }
    ++fitOff;
  }
}

} // end namespace otb

#endif
