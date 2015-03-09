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

#ifndef __otbLocalRxDetectorFilter_txx
#define __otbLocalRxDetectorFilter_txx

#include "otbLocalRxDetectorFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
LocalRxDetectorFilter<TInputImage, TOutputImage>
::LocalRxDetectorFilter()
  : m_InternalRadius(1), m_ExternalRadius(2)
{
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
LocalRxDetectorFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Internal Radius: " << m_InternalRadius << std::endl;
  os << indent << "External Radius: " << m_ExternalRadius << std::endl;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
LocalRxDetectorFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // Get the input and output pointers
  OutputPointerType       outputPtr = this->GetOutput();

  // Fill the buffer with black pixels
  outputPtr->FillBuffer(0);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
LocalRxDetectorFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  // Get the input and output pointers
  InputConstPointerType       inputPtr = this->GetInput();
  OutputPointerType       outputPtr = this->GetOutput();

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Compute input region for thread
  typename TInputImage::RegionType inputRegionForThread;
  inputRegionForThread = outputRegionForThread;
  inputRegionForThread.PadByRadius(m_ExternalRadius);
  inputRegionForThread.Crop(inputPtr->GetLargestPossibleRegion());

  // Iterator on input region
  typename ConstShapedNeighborhoodIteratorType::RadiusType radius;
  radius.Fill(m_ExternalRadius);

  VectorFaceCalculatorType                                         vectorFaceCalculator;
  typename VectorFaceCalculatorType::FaceListType           vectorFaceList;
  typename VectorFaceCalculatorType::FaceListType::iterator vectorFit;

  vectorFaceList = vectorFaceCalculator(inputPtr, inputRegionForThread, radius);

  vectorFit = vectorFaceList.begin();       // Only the first face is used

  ConstShapedNeighborhoodIteratorType inputIt(radius, inputPtr, *vectorFit);

  // Neighborhood Configuration
  typename ConstShapedNeighborhoodIteratorType::OffsetType off;

  for (int y = -m_ExternalRadius; y <= m_ExternalRadius; y++)
    {
    off[1] = y;
    for (int x = -m_ExternalRadius; x <= m_ExternalRadius; x++)
      {
      off[0] = x;
      if ((abs(x) > m_InternalRadius) || (abs(y) > m_InternalRadius))
        {
        inputIt.ActivateOffset(off);
        }
      }
    }


  // iterator on output region
  FaceCalculatorType                         faceCalculator;
  typename FaceCalculatorType::FaceListType           faceList;
  typename FaceCalculatorType::FaceListType::iterator fit;

  faceList = faceCalculator(outputPtr, inputRegionForThread, radius);
  fit = faceList.begin();       // Only the first face is used

  ImageRegionIteratorType outputIt(outputPtr, *fit);

  // Run Input Image
  for (inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt, ++outputIt)
    {
    // Create ListSample
    typename ListSampleType::Pointer listSample = ListSampleType::New();
    listSample->SetMeasurementVectorSize(inputPtr->GetNumberOfComponentsPerPixel());

    // Run neighborhood
    typename ConstShapedNeighborhoodIteratorType::ConstIterator ci;
    for (ci = inputIt.Begin(); !ci.IsAtEnd(); ++ci)
      {
      // Pushback element in listSample
      listSample->PushBack(ci.Get());
      }

    // Compute mean & covariance matrix
    typename CovarianceCalculatorType::Pointer covarianceCalculator = CovarianceCalculatorType::New();
    covarianceCalculator->SetInput(listSample);
    covarianceCalculator->Update();

    MeasurementVectorRealType meanVector = covarianceCalculator->GetMean();
    MatrixType covarianceMatrix = covarianceCalculator->GetCovarianceMatrix();

    // Compute RX value
    typename MatrixType::InternalMatrixType invCovMat = covarianceMatrix.GetInverse();

    VectorMeasurementType testPixVec;
    testPixVec = inputPtr->GetPixel(inputIt.GetIndex());

    VectorMeasurementType meanVec(meanVector.GetNumberOfElements());
    for(unsigned int i = 0; i < meanVector.GetNumberOfElements(); ++i)
      {
      meanVec.SetElement(i, meanVector.GetElement(i));
      }

    typename MatrixType::InternalMatrixType centeredTestPixMat(meanVector.GetNumberOfElements(), 1);
    for (unsigned int i = 0; i < centeredTestPixMat.rows(); ++i)
      {
      centeredTestPixMat.put(i, 0, (testPixVec.GetElement(i) - meanVector.GetElement(i)));
      }

    typename MatrixType::InternalMatrixType rxValue = centeredTestPixMat.transpose() * invCovMat * centeredTestPixMat;

    outputIt.Set(rxValue.get(0, 0));
    }
}

/**
*
*/
template <class TInputImage, class TOutputImage>
void
LocalRxDetectorFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputPointerType  inputPtr =
    const_cast< InputImageType * >( this->GetInput());
  OutputPointerType outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_ExternalRadius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

} // end namespace otb

#endif

