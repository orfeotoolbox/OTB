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
#ifndef __otbRadiometricMomentsImageFilter_txx
#define __otbRadiometricMomentsImageFilter_txx

#include "otbRadiometricMomentsImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkNeighborhoodAlgorithm.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
RadiometricMomentsImageFilter<TInputImage, TOutputImage>
::RadiometricMomentsImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Radius.Fill(1);
}

template <class TInputImage, class TOutputImage>
void
RadiometricMomentsImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr =
    const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

/**
 * Generate the output information
 */
template <class TInputImage, class TOutputImage>
void
RadiometricMomentsImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(4);
}
/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage>
void
RadiometricMomentsImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
{
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;

// We use dynamic_cast since inputs are stored as DataObjects.  The
// ImageToImageFilter::GetInput(int) always returns a pointer to a
// TInputImage so it cannot be used for the second input.
  InputImagePointer inputPtr
    = dynamic_cast<const TInputImage*>(ProcessObjectType::GetInput(0));
  OutputImagePointer outputPtr = this->GetOutput(0);

  RadiusType r;

  r[0] = m_Radius[0];
  r[1] = m_Radius[1];

  NeighborhoodIteratorType neighInputIt;

  itk::ImageRegionIterator<TOutputImage> outputIt;

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>               bC;
  faceList = bC(inputPtr, outputRegionForThread, r);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fit;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    neighInputIt = itk::ConstNeighborhoodIterator<TInputImage>(r, inputPtr, *fit);

    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fit);
    neighInputIt.OverrideBoundaryCondition(&nbc);
    neighInputIt.GoToBegin();

    while (!outputIt.IsAtEnd())
      {
      outputIt.Set(m_Functor(neighInputIt));

      ++neighInputIt;
      ++outputIt;
      progress.CompletedPixel();
      }
    }
}

} // end namespace otb

#endif
