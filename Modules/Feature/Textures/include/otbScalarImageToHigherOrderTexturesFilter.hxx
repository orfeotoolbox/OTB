/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbScalarImageToHigherOrderTexturesFilter_hxx
#define otbScalarImageToHigherOrderTexturesFilter_hxx

#include "otbScalarImageToHigherOrderTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::ScalarImageToHigherOrderTexturesFilter()
  : m_Radius(), m_NumberOfBinsPerAxis(8), m_InputImageMinimum(0), m_InputImageMaximum(255), m_FastCalculations(false), m_SubsampleFactor(), m_SubsampleOffset()
{
  // There are 10 outputs corresponding to the 8 textures indices
  this->SetNumberOfRequiredOutputs(10);

  // Create the 11 outputs
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());
  this->SetNthOutput(4, OutputImageType::New());
  this->SetNthOutput(5, OutputImageType::New());
  this->SetNthOutput(6, OutputImageType::New());
  this->SetNthOutput(7, OutputImageType::New());
  this->SetNthOutput(8, OutputImageType::New());
  this->SetNthOutput(9, OutputImageType::New());

  m_Radius.Fill(10);

  // Set the offset directions to their defaults: half of all the possible
  // directions 1 pixel away. (The other half is included by symmetry.)
  // We use a neighborhood iterator to calculate the appropriate offsets.
  typedef itk::Neighborhood<typename InputImageType::PixelType, InputImageType::ImageDimension> NeighborhoodType;
  NeighborhoodType hood;
  hood.SetRadius(1);

  // select all "previous" neighbors that are face+edge+vertex
  // connected to the current pixel. do not include the center pixel.
  unsigned int        centerIndex = hood.GetCenterNeighborhoodIndex();
  OffsetVectorPointer offsets     = OffsetVector::New();
  for (unsigned int d = 0; d < centerIndex; d++)
  {
    OffsetType offset = hood.GetOffset(d);
    offsets->push_back(offset);
  }
  this->SetOffsets(offsets);

  this->m_SubsampleFactor.Fill(1);
  this->m_SubsampleOffset.Fill(0);
}

template <class TInputImage, class TOutputImage>
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::~ScalarImageToHigherOrderTexturesFilter()
{
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetShortRunEmphasisOutput()
{
  return this->GetOutput(0);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetLongRunEmphasisOutput()
{
  return this->GetOutput(1);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetGreyLevelNonuniformityOutput()
{
  return this->GetOutput(2);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetRunLengthNonuniformityOutput()
{
  return this->GetOutput(3);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetLowGreyLevelRunEmphasisOutput()
{
  return this->GetOutput(4);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetHighGreyLevelRunEmphasisOutput()
{
  return this->GetOutput(5);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetShortRunLowGreyLevelEmphasisOutput()
{
  return this->GetOutput(6);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetShortRunHighGreyLevelEmphasisOutput()
{
  return this->GetOutput(7);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetLongRunLowGreyLevelEmphasisOutput()
{
  return this->GetOutput(8);
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::OutputImageType*
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GetLongRunHighGreyLevelEmphasisOutput()
{
  return this->GetOutput(9);
}

template <class TInputImage, class TOutputImage>
void ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::SetOffset(const OffsetType offset)
{
  OffsetVectorPointer offsetVector = OffsetVector::New();
  offsetVector->push_back(offset);
  this->SetOffsets(offsetVector);
}

template <class TInputImage, class TOutputImage>
void ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  // Compute output size, origin & spacing
  const InputImageType* inputPtr    = this->GetInput();
  InputRegionType       inputRegion = inputPtr->GetLargestPossibleRegion();
  OutputRegionType      outputRegion;
  outputRegion.SetIndex(0, 0);
  outputRegion.SetIndex(1, 0);
  outputRegion.SetSize(0, 1 + (inputRegion.GetSize(0) - 1 - m_SubsampleOffset[0]) / m_SubsampleFactor[0]);
  outputRegion.SetSize(1, 1 + (inputRegion.GetSize(1) - 1 - m_SubsampleOffset[1]) / m_SubsampleFactor[1]);

  typename OutputImageType::SpacingType outSpacing = inputPtr->GetSignedSpacing();
  outSpacing[0] *= m_SubsampleFactor[0];
  outSpacing[1] *= m_SubsampleFactor[1];

  typename OutputImageType::PointType outOrigin;
  inputPtr->TransformIndexToPhysicalPoint(inputRegion.GetIndex() + m_SubsampleOffset, outOrigin);

  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); i++)
  {
    OutputImagePointerType outputPtr = this->GetOutput(i);
    outputPtr->CopyInformation(inputPtr);
    outputPtr->SetLargestPossibleRegion(outputRegion);
    outputPtr->SetOrigin(outOrigin);
    outputPtr->SetSignedSpacing(outSpacing);
  }
}


template <class TInputImage, class TOutputImage>
void ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr  = const_cast<InputImageType*>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
  {
    return;
  }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType                     outputRequestedRegion = outputPtr->GetRequestedRegion();
  typename OutputRegionType::IndexType outputIndex           = outputRequestedRegion.GetIndex();
  typename OutputRegionType::SizeType  outputSize            = outputRequestedRegion.GetSize();
  InputRegionType                      inputLargest          = inputPtr->GetLargestPossibleRegion();

  // Convert index and size to full grid
  outputIndex[0] = outputIndex[0] * m_SubsampleFactor[0] + m_SubsampleOffset[0] + inputLargest.GetIndex(0);
  outputIndex[1] = outputIndex[1] * m_SubsampleFactor[1] + m_SubsampleOffset[1] + inputLargest.GetIndex(1);
  outputSize[0]  = 1 + (outputSize[0] - 1) * m_SubsampleFactor[0];
  outputSize[1]  = 1 + (outputSize[1] - 1) * m_SubsampleFactor[1];

  InputRegionType inputRequestedRegion(outputIndex, outputSize);

  // Apply the radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // Try to apply the requested region to the input image
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
  {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
  }
  else
  {
    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

template <class TInputImage, class TOutputImage>
void ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputRegionType& outputRegionForThread,
                                                                                             itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  const InputImageType* inputPtr = this->GetInput();

  typedef typename itk::ImageRegionIterator<OutputImageType> IteratorType;
  std::vector<IteratorType>                                  outputImagesIterators;

  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    outputImagesIterators.push_back(IteratorType(this->GetOutput(i), outputRegionForThread));
    outputImagesIterators[i].GoToBegin();
  }

  // Compute the max possible run length (in physical unit)
  typename InputImageType::PointType topLeftPoint;
  typename InputImageType::PointType bottomRightPoint;
  inputPtr->TransformIndexToPhysicalPoint(outputImagesIterators[0].GetIndex() - m_Radius, topLeftPoint);
  inputPtr->TransformIndexToPhysicalPoint(outputImagesIterators[0].GetIndex() + m_Radius, bottomRightPoint);
  double maxDistance = topLeftPoint.EuclideanDistanceTo(bottomRightPoint);

  InputRegionType inputLargest = inputPtr->GetLargestPossibleRegion();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!outputImagesIterators[0].IsAtEnd())
  {
    // Compute the region on which run-length matrix will be estimated
    typename InputRegionType::IndexType inputIndex;
    typename InputRegionType::SizeType  inputSize;

    // Convert index to full grid
    typename OutputImageType::IndexType outIndex;

    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
    {
      outIndex[dim]   = outputImagesIterators[0].GetIndex()[dim] * m_SubsampleFactor[dim] + m_SubsampleOffset[dim] + inputLargest.GetIndex(dim);
      inputIndex[dim] = outIndex[dim] - m_Radius[dim];
      inputSize[dim]  = 2 * m_Radius[dim] + 1;
    }

    // Build the input  region
    InputRegionType inputRegion;
    inputRegion.SetIndex(inputIndex);
    inputRegion.SetSize(inputSize);

    inputRegion.Crop(inputPtr->GetBufferedRegion());

    // Create a local image corresponding to the input region
    InputImagePointerType localInputImage = InputImageType::New();
    localInputImage->SetRegions(inputRegion);
    localInputImage->Allocate();
    typedef itk::ImageRegionIteratorWithIndex<InputImageType>      ImageRegionIteratorType;
    typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> ImageRegionConstIteratorType;
    ImageRegionConstIteratorType                                   itInputPtr(inputPtr, inputRegion);
    ImageRegionIteratorType                                        itLocalInputImage(localInputImage, inputRegion);
    for (itInputPtr.GoToBegin(), itLocalInputImage.GoToBegin(); !itInputPtr.IsAtEnd(); ++itInputPtr, ++itLocalInputImage)
    {
      itLocalInputImage.Set(itInputPtr.Get());
    }

    typename ScalarImageToRunLengthFeaturesFilterType::Pointer runLengthFeatureCalculator = ScalarImageToRunLengthFeaturesFilterType::New();
    runLengthFeatureCalculator->SetInput(localInputImage);
    runLengthFeatureCalculator->SetOffsets(m_Offsets);
    runLengthFeatureCalculator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
    runLengthFeatureCalculator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);
    runLengthFeatureCalculator->SetDistanceValueMinMax(0, maxDistance);

    runLengthFeatureCalculator->Update();

    typename ScalarImageToRunLengthFeaturesFilterType::FeatureValueVector& featuresMeans = *(runLengthFeatureCalculator->GetFeatureMeans().GetPointer());

    // Fill output
    for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
      // Fill output
      outputImagesIterators[i].Set(featuresMeans[i]);
      // Increment iterators
      ++outputImagesIterators[i];
    }
    // Update progress
    progress.CompletedPixel();
  }
}

} // End namespace otb

#endif
