/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBoxAndWhiskerImageFilter_txx
#define __otbBoxAndWhiskerImageFilter_txx

#include <vector>
#include <algorithm>

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

#include "otbBoxAndWhiskerImageFilter.h"
#include "otbEuclideanDistanceWithMissingValue.h"

namespace otb {

template <class TInputImage>
BoxAndWhiskerImageFilter <TInputImage>
::BoxAndWhiskerImageFilter ()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->InPlaceOn();
  m_Beta = 1.5;
  m_Radius.Fill(1);
  m_NumberFound = 0L;
}

template <class TInputImage>
void
BoxAndWhiskerImageFilter <TInputImage>
::ThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  itk::ThreadIdType threadId)
{
  const TInputImage * inputPtr = this->GetInput();
  OutputImageType *   outputPtr = this->GetOutput();

  // data-set boundary faces
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>
  ImageBoundaryFacesCalculatorType;
  typename ImageBoundaryFacesCalculatorType::FaceListType faceList;
  ImageBoundaryFacesCalculatorType                        boundaryCalculator;
  faceList = boundaryCalculator(inputPtr, outputRegionForThread, this->GetRadius());

  // face iterator
  typename ImageBoundaryFacesCalculatorType::FaceListType::iterator faceIterator;

  // local iterators
  itk::ImageRegionConstIterator<InputImageType> inputIter;
  itk::ImageRegionIterator<OutputImageType>     outputIter;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  /**
   *  Process each of the boundary faces.
   *  These are N-d regions which border the edge of the buffer.
   */
  for (faceIterator = faceList.begin();
       faceIterator != faceList.end();
       ++faceIterator)
    {
    inputIter = itk::ImageRegionConstIterator<InputImageType> (inputPtr, *faceIterator);
    inputIter.GoToBegin();

    outputIter = itk::ImageRegionIterator<OutputImageType> (outputPtr, *faceIterator);
    outputIter.GoToBegin();

    while (!outputIter.IsAtEnd())
      {
      outputIter.Set(this->PerformBoxAndWhiskerDetection(inputIter.Get()));

      ++inputIter;
      ++outputIter;

      progress.CompletedPixel();
      }
    }
}

template <class TInputImage>
typename BoxAndWhiskerImageFilter <TInputImage>::PixelType
BoxAndWhiskerImageFilter <TInputImage>
::PerformBoxAndWhiskerDetection(const PixelType& pixel)
{
  typedef std::vector<ValueType>                                        VectorType;
  typedef otb::Statistics::EuclideanDistanceWithMissingValue<PixelType> OutlierType;

  unsigned int       i;
  const unsigned int vectorSize = pixel.Size();
  const unsigned int medianPosition = vectorSize / 2;
  const unsigned int firstQuartilePosition = vectorSize / 4;
  const unsigned int thirdQuartilePosition = (3 * vectorSize) / 4;

  VectorType data(vectorSize);
  for (i = 0; i < vectorSize; ++i)
    data[i] = pixel[i];

  std::sort(data.begin(), data.end());

  double box = m_Beta * static_cast<double>(data[thirdQuartilePosition] - data[firstQuartilePosition]);
  double median = ::fabs(static_cast<double>(data[medianPosition]));

  PixelType outputPixel(pixel);
  for (i = 0; i < vectorSize; ++i)
    {
    if (::fabs(static_cast<double>(outputPixel[i]) - box) > median)
      {
      OutlierType::SetToMissingValue(outputPixel[i]);
      m_NumberFound++;
      }
    }

  return outputPixel;
}

template <class TInputImage>
void
BoxAndWhiskerImageFilter <TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(
    this->GetInput()->GetNumberOfComponentsPerPixel());

  this->GetOutput()->CopyInformation(this->GetInput());

}

template <class TInputImage>
void
BoxAndWhiskerImageFilter <TInputImage>
::AllocateOutputs()
{
  OutputImageType * output = this->GetOutput();
  output->SetNumberOfComponentsPerPixel(
    this->GetInput()->GetNumberOfComponentsPerPixel());
  output->SetBufferedRegion(output->GetRequestedRegion());
  output->Allocate();
}

} // end of namespace otb

#endif
