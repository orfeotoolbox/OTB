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


// template <class TInputImage, class TOutputImage, class TFunction  >
// void
// UnaryFunctorNeighborhoodWithOffsetImageFilter<TInputImage,TOutputImage,TFunction>
// ::BeforeThreadedGenerateData()
// {
//   Superclass::BeforeThreadedGenerateData();

//   for (int i =0; i<this->GetNumberOfThreads(); i++)
//   {
//     m_FunctorList.push_back(m_Functor);
//   }
// }


// template <class TInputImage, class TOutputImage, class TFunction  >
// void
// UnaryFunctorNeighborhoodWithOffsetImageFilter<TInputImage,TOutputImage,TFunction>
// ::GenerateInputRequestedRegion()
// {
//   // call the superclass' implementation of this method
//   Superclass::GenerateInputRequestedRegion();

//   // get pointers to the input and output
//   typename Superclass::InputImagePointer  inputPtr =
//     const_cast< TInputImage * >( this->GetInput());
//   typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

//   if ( !inputPtr || !outputPtr )
//   {
//     return;
//   }
//   // get a copy of the input requested region (should equal the output
//   // requested region)
//   typename TInputImage::RegionType inputRequestedRegion;
//   inputRequestedRegion = inputPtr->GetRequestedRegion();

//   // pad the input requested region by the operator radius
//   InputImageSizeType maxRad;
//   maxRad[0] = m_Radius + vcl_abs(m_Offset[0]);
//   maxRad[1] = m_Radius + vcl_abs(m_Offset[1]);;
//   inputRequestedRegion.PadByRadius( maxRad );

//   // crop the input requested region at the input's largest possible region
//   if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
//   {
//     inputPtr->SetRequestedRegion( inputRequestedRegion );
//     return;
//   }
//   else
//   {
//     // Couldn't crop the region (requested region is outside the largest
//     // possible region).  Throw an exception.

//     // store what we tried to request (prior to trying to crop)
//     inputPtr->SetRequestedRegion( inputRequestedRegion );

//     // build an exception
//     itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
//     itk::OStringStream msg;
//     msg << this->GetNameOfClass()
//     << "::GenerateInputRequestedRegion()";
//     e.SetLocation(msg.str().c_str());
//     e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
//     e.SetDataObject(inputPtr);
//     throw e;
//   }
// }



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
  rOff[0] = this->GetRadius() + vcl_abs(this->GetOffset()[0]);
  rOff[1] = this->GetRadius() + vcl_abs(this->GetOffset()[1]);
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

      outputIt.Set( this->m_FunctorList[threadId]( neighInputOffIt ) );

      ++neighInputOffIt;
      ++outputIt;
      progress.CompletedPixel();
    }
    ++fitOff;
  }
}

} // end namespace otb

#endif
