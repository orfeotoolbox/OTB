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

#ifndef __otbLocalRxDetectorNonThreadFilter_txx
#define __otbLocalRxDetectorNonThreadFilter_txx

#include "otbLocalRxDetectorNonThreadFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
LocalRxDetectorNonThreadFilter<TInputImage, TOutputImage>
::LocalRxDetectorNonThreadFilter()
{
  this->m_ExternalRadius = 0;
  this->m_InternalRadius = 0;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
LocalRxDetectorNonThreadFilter<TInputImage, TOutputImage>
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
LocalRxDetectorNonThreadFilter<TInputImage, TOutputImage>
::GenerateData()
{

  this->AllocateOutputs();

  // Get the input and output pointers
  InputPointerType  inputPtr =
    const_cast< InputImageType * >( this->GetInput());
  OutputPointerType              outputPtr = this->GetOutput();

  inputPtr->Update();

  // Fill the output buffer with black pixels
  outputPtr->FillBuffer(0);

  // Iterator on input region
  typename ConstShapedNeighborhoodIteratorType::RadiusType radius;
  radius.Fill(m_ExternalRadius);

  VectorFaceCalculatorType                                         vectorFaceCalculator;
  typename VectorFaceCalculatorType::FaceListType           vectorFaceList;
  typename VectorFaceCalculatorType::FaceListType::iterator vectorFit;

  vectorFaceList = vectorFaceCalculator(inputPtr, inputPtr->GetRequestedRegion(), radius);
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

  // Iterator on output region
  FaceCalculatorType                                                 faceCalculator;
  typename FaceCalculatorType::FaceListType           faceList;
  typename FaceCalculatorType::FaceListType::iterator fit;

  faceList = faceCalculator(outputPtr, inputPtr->GetRequestedRegion(), radius);
  fit = faceList.begin();       // Only the first face is used


  ImageRegionIteratorType outputIt(outputPtr, *fit);

  //std::cout << "Region de la face list : " << *vectorFit << std::endl;
  //std::cout << "Region dans le buffer : " << inputPtr->GetBufferedRegion() << std::endl;

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
      //std::cout << "pixel of shaped iteror : " << ci.Get() << std::endl;
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
LocalRxDetectorNonThreadFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // Call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Get pointers to the input and output
  InputPointerType  inputPtr =
    const_cast< InputImageType * >( this->GetInput());
  OutputPointerType outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // Get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // Pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_ExternalRadius );

  // Crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // Store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

} // end namespace otb

#endif
