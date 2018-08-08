/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbBoxAndWhiskerImageFilter_hxx
#define otbBoxAndWhiskerImageFilter_hxx

#include <vector>
#include <algorithm>
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

#include "otbBoxAndWhiskerImageFilter.h"
#include "otbEuclideanDistanceMetricWithMissingValue.h"

namespace otb {

template <class TInputImage>
BoxAndWhiskerImageFilter <TInputImage>
::BoxAndWhiskerImageFilter ()
{
  OTB_DISABLE_DYNAMIC_MT;
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
  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValue<PixelType> OutlierType;

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
