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
#ifndef __otbScalarImageToHigherOrderTexturesFilter_txx
#define __otbScalarImageToHigherOrderTexturesFilter_txx

#include "otbScalarImageToHigherOrderTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToHigherOrderTexturesFilter() :
  m_Radius(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(255),
  m_FastCalculations(false)
{
  // There are 11 outputs corresponding to the 8 textures indices
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
  this->SetNthOutput(10, OutputImageType::New());

  m_Radius.Fill(10);

  // Set the offset directions to their defaults: half of all the possible
  // directions 1 pixel away. (The other half is included by symmetry.)
  // We use a neighborhood iterator to calculate the appropriate offsets.
  typedef itk::Neighborhood<typename InputImageType::PixelType,
    InputImageType::ImageDimension> NeighborhoodType;
  NeighborhoodType hood;
  hood.SetRadius( 1 );

  // select all "previous" neighbors that are face+edge+vertex
  // connected to the current pixel. do not include the center pixel.
  unsigned int centerIndex = hood.GetCenterNeighborhoodIndex();
  OffsetVectorPointer offsets = OffsetVector::New();
  for( unsigned int d = 0; d < centerIndex; d++ )
    {
    OffsetType offset = hood.GetOffset( d );
    offsets->push_back( offset );
    }
  this->SetOffsets( offsets );

}

template <class TInputImage, class TOutputImage>
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::~ScalarImageToHigherOrderTexturesFilter()
{}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetShortRunEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetLongRunEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetGreyLevelNonuniformityOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(2));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetRunLengthNonuniformityOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(3));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetRunPercentageOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(4));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetLowGreyLevelRunEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(5));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetHighGreyLevelRunEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 7)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(6));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetShortRunLowGreyLevelEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 8)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(7));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetShortRunHighGreyLevelEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 9)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(8));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetLongRunLowGreyLevelEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 10)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(9));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GetLongRunHighGreyLevelEmphasisOutput()
{
  if (this->GetNumberOfOutputs() < 11)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(10));
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::SetOffset( const OffsetType offset )
{
  OffsetVectorPointer offsetVector = OffsetVector::New();
  offsetVector->push_back( offset );
  this->SetOffsets( offsetVector );
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType outputRequestedRegion = outputPtr->GetRequestedRegion();
  InputRegionType inputRequestedRegion = outputRequestedRegion;

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
void
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr  = const_cast<InputImageType *>(this->GetInput());

  typedef typename itk::ImageRegionIterator<OutputImageType> IteratorType;
  std::vector<IteratorType> outputImagesIterators;

  for (unsigned int i = 0; i < 10; ++i)
    {
    outputImagesIterators.push_back( IteratorType(this->GetOutput(i), outputRegionForThread) );
    outputImagesIterators[i].GoToBegin();
    }

  // Compute the max possible run length (in physical unit)
  typename InputImageType::PointType  topLeftPoint;
  typename InputImageType::PointType  bottomRightPoint;
  inputPtr->TransformIndexToPhysicalPoint( outputImagesIterators[0].GetIndex() - m_Radius, topLeftPoint );
  inputPtr->TransformIndexToPhysicalPoint( outputImagesIterators[0].GetIndex() + m_Radius, bottomRightPoint );
  double maxDistance = topLeftPoint.EuclideanDistanceTo(bottomRightPoint);

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while ( !outputImagesIterators[0].IsAtEnd() )
    {
    // Compute the region on which run-length matrix will be estimated
    typename InputRegionType::IndexType inputIndex = outputImagesIterators[0].GetIndex() - m_Radius;
    typename InputRegionType::SizeType  inputSize;

    // First, apply offset
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      inputSize[dim] = 2 * m_Radius[dim] + 1;
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
    typedef itk::ImageRegionIteratorWithIndex<InputImageType> ImageRegionIteratorType;
    ImageRegionIteratorType itInputPtr(inputPtr, inputRegion);
    ImageRegionIteratorType itLocalInputImage(localInputImage, inputRegion);
    for (itInputPtr.GoToBegin(), itLocalInputImage.GoToBegin();
        !itInputPtr.IsAtEnd();
        ++itInputPtr, ++itLocalInputImage)
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

    typename ScalarImageToRunLengthFeaturesFilterType::FeatureValueVector&
      featuresMeans = *(runLengthFeatureCalculator->GetFeatureMeans().GetPointer());

    // Fill outputs
    outputImagesIterators[0].Set(featuresMeans[0]);
    outputImagesIterators[1].Set(featuresMeans[1]);
    outputImagesIterators[2].Set(featuresMeans[2]);
    outputImagesIterators[3].Set(featuresMeans[3]);
    outputImagesIterators[4].Set(featuresMeans[4]);
    outputImagesIterators[5].Set(featuresMeans[5]);
    outputImagesIterators[6].Set(featuresMeans[6]);
    outputImagesIterators[7].Set(featuresMeans[7]);
    outputImagesIterators[8].Set(featuresMeans[8]);
    outputImagesIterators[9].Set(featuresMeans[9]);

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    for (unsigned int i = 0; i < 10; ++i)
      {
      ++outputImagesIterators[i];
      }
    }

}

} // End namespace otb

#endif
