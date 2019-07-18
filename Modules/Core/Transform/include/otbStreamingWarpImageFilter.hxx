/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingWarpImageFilter_hxx
#define otbStreamingWarpImageFilter_hxx

#include "otbStreamingWarpImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{

template<class TInputImage, class TOutputImage, class TDisplacementField>
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::StreamingWarpImageFilter()
 {
  // Fill the default maximum displacement
  m_MaximumDisplacement.Fill(1);
  m_OutputSignedSpacing = this->Superclass::GetOutputSpacing();
 }


template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::SetOutputSpacing( const SpacingType outputSpacing )
{
  m_OutputSignedSpacing = outputSpacing;
  SpacingType spacing = outputSpacing;
  typename TInputImage::DirectionType direction = 
      this->GetOutput()->GetDirection();
  for( unsigned int i = 0 ; i < TInputImage::ImageDimension ; ++i )
    {
    if ( spacing[i] < 0 )
      {
      if ( direction[i][i] > 0 )
        {
        for( unsigned int j = 0 ; j < TInputImage::ImageDimension ; ++j )
          {
          direction[j][i] = - direction[j][i];
          }
        }
      spacing[i] = - spacing[i];
      }
    }
  this->Superclass::SetOutputSpacing( spacing );
  this->Superclass::SetOutputDirection( direction );
  this->Modified();
}

template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::SetOutputSpacing( const double *values )
{
  SpacingType s;
  for(unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
    {
    s[i] = static_cast< typename SpacingType::ValueType >(values[i]);
    }
  this->SetOutputSpacing(s);
}

template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::GenerateInputRequestedRegion()
 {
  Superclass::GenerateInputRequestedRegion();

  // Get the input and displacement field pointers
  InputImageType         *       inputPtr
  = const_cast<InputImageType *>(this->GetInput());
  DisplacementFieldType   * displacementPtr
  = const_cast<DisplacementFieldType*>(this->GetDisplacementField());
  OutputImageType * outputPtr
  = this->GetOutput();
  // Check if the input and the displacement field exist
  if (!inputPtr || !displacementPtr || !outputPtr)
    {
    return;
    }

  // Here we are breaking traditional pipeline steps because we need to access the displacement field data
  // so as to compute the input image requested region

  // 1) First, evaluate the displacement field requested region corresponding to the output requested region
  // (Here we suppose that the displacement field and the output image are in the same geometry/map projection)
  typename OutputImageType::RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();
  typename OutputImageType::IndexType outIndexStart = outputRequestedRegion.GetIndex();
  typename OutputImageType::IndexType outIndexEnd;
  for(unsigned int dim = 0; dim<OutputImageType::ImageDimension; ++dim)
    outIndexEnd[dim]= outIndexStart[dim] + outputRequestedRegion.GetSize()[dim]-1;
  typename OutputImageType::PointType outPointStart, outPointEnd;
  outputPtr->TransformIndexToPhysicalPoint(outIndexStart, outPointStart);
  outputPtr->TransformIndexToPhysicalPoint(outIndexEnd, outPointEnd);

  typename DisplacementFieldType::IndexType defIndexStart, defIndexEnd;
  displacementPtr->TransformPhysicalPointToIndex(outPointStart, defIndexStart);
  displacementPtr->TransformPhysicalPointToIndex(outPointEnd, defIndexEnd);

  typename DisplacementFieldType::SizeType defRequestedSize;
  typename DisplacementFieldType::IndexType defRequestedIndex;

  for(unsigned int dim = 0; dim<OutputImageType::ImageDimension; ++dim)
    {
    defRequestedIndex[dim] = std::min(defIndexStart[dim], defIndexEnd[dim]);
    defRequestedSize[dim] = std::max(defIndexStart[dim], defIndexEnd[dim]) - defRequestedIndex[dim] + 1;
    }

  // Finally, build the displacement field requested region
  typename DisplacementFieldType::RegionType displacementRequestedRegion;
  displacementRequestedRegion.SetIndex(defRequestedIndex);
  displacementRequestedRegion.SetSize(defRequestedSize);

  // Avoid extrapolation
  displacementRequestedRegion.PadByRadius(1);

  // crop the input requested region at the input's largest possible region
  if (displacementRequestedRegion.Crop(displacementPtr->GetLargestPossibleRegion()))
    {
    displacementPtr->SetRequestedRegion(displacementRequestedRegion);
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    displacementPtr->SetRequestedRegion(displacementRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of the displacement field.");
    e.SetDataObject(inputPtr);
    throw e;
    }

  // 2) If we are still there, we have a correct displacement field requested region.
  // This next step breaks pipeline rule but we need to do it to compute the input requested region,
  // since it depends on displacement value.

  // Trigger pipeline update on the displacement field

  displacementPtr->PropagateRequestedRegion();
  displacementPtr->UpdateOutputData();

  // Walk the loaded displacement field to derive maximum and minimum
  itk::ImageRegionIteratorWithIndex<DisplacementFieldType> defIt(displacementPtr, displacementRequestedRegion);
  defIt.GoToBegin();

  typename InputImageType::PointType currentPoint;
  typename InputImageType::PointType inputStartPoint, inputEndPoint;

  // Initialise start and end points
  displacementPtr->TransformIndexToPhysicalPoint(defIt.GetIndex(), currentPoint);
  for(unsigned int dim = 0; dim<DisplacementFieldType::ImageDimension; ++dim)
    {
    currentPoint[dim]+=defIt.Get()[dim];
    }
  inputStartPoint = currentPoint;
  inputEndPoint = currentPoint;

  ++defIt;

  // 3) Now Walk the field and compute the physical bounding box
  while(!defIt.IsAtEnd())
    {
    displacementPtr->TransformIndexToPhysicalPoint(defIt.GetIndex(), currentPoint);
    for(unsigned int dim = 0; dim<DisplacementFieldType::ImageDimension; ++dim)
      {
      currentPoint[dim]+=defIt.Get()[dim];
      if(inputStartPoint[dim] > currentPoint[dim])
        inputStartPoint[dim] = currentPoint[dim];
      if(inputEndPoint[dim] < currentPoint[dim])
        inputEndPoint[dim] = currentPoint[dim];
      }
    ++defIt;
    }

  // Convert physical bounding box to requested region
  typename InputImageType::IndexType inputStartIndex, inputEndIndex;
  inputPtr->TransformPhysicalPointToIndex(inputStartPoint, inputStartIndex);
  inputPtr->TransformPhysicalPointToIndex(inputEndPoint, inputEndIndex);

  typename InputImageType::SizeType inputFinalSize;
  typename InputImageType::IndexType inputFinalIndex;

  for(unsigned int dim = 0; dim<DisplacementFieldType::ImageDimension; ++dim)
    {
    inputFinalIndex[dim] = std::min(inputStartIndex[dim], inputEndIndex[dim]);
    inputFinalSize[dim] = std::max(inputStartIndex[dim], inputEndIndex[dim])-inputFinalIndex[dim]+1;
    }

  typename InputImageType::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(inputFinalIndex);
  inputRequestedRegion.SetSize(inputFinalSize);

  // Compute the padding due to the interpolator
  unsigned int interpolatorRadius =
      StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(interpolatorRadius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  // Here we do not throw an exception, we just do nothing since
  // resampling filter can legitimately ask for a region outside of
  // input's largest possible region.
 }


template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Set the NoData flag to the edge padding value
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  std::vector<bool> noDataValueAvailable;
  bool ret = itk::ExposeMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,noDataValueAvailable);
  if (!ret)
    {
    noDataValueAvailable.resize(this->GetOutput()->GetNumberOfComponentsPerPixel(),false);
    }
  std::vector<double> noDataValue;
  ret = itk::ExposeMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,noDataValue);
  if (!ret)
    {
    noDataValue.resize(this->GetOutput()->GetNumberOfComponentsPerPixel(),0.0);
    }
  PixelType edgePadding = this->GetEdgePaddingValue();
  if (itk::NumericTraits<PixelType>::GetLength(edgePadding) != this->GetOutput()->GetNumberOfComponentsPerPixel())
    {
    itk::NumericTraits<PixelType>::SetLength(edgePadding,this->GetOutput()->GetNumberOfComponentsPerPixel());
    this->SetEdgePaddingValue(edgePadding);
    }
  for (unsigned int i=0; i<noDataValueAvailable.size() ; ++i)
    {
    if (!noDataValueAvailable[i])
      {
      noDataValueAvailable[i] = true;
      noDataValue[i] = itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,edgePadding);
      }
    }
  itk::EncapsulateMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,noDataValueAvailable);
  itk::EncapsulateMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,noDataValue);
}

template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::ThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  itk::ThreadIdType threadId )
  {
  // the superclass itk::WarpImageFilter is doing the actual warping
  Superclass::ThreadedGenerateData(outputRegionForThread,threadId);

  // second pass on the thread region to mask pixels outside the displacement grid
  const PixelType paddingValue = this->GetEdgePaddingValue();
  OutputImagePointerType outputPtr = this->GetOutput();

  // ITK 4.13 fix const correctness of GetDisplacementField. 
  // Related commit in ITK: https://github.com/InsightSoftwareConsortium/ITK/commit/0070848b91baf69f04893bc3ce85bcf110c3c63a
  
  // DisplacementFieldPointerType fieldPtr = this->GetDisplacementField();
  const DisplacementFieldType * fieldPtr = this->GetDisplacementField();


  DisplacementFieldRegionType defRegion = fieldPtr->GetLargestPossibleRegion();

  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(
    outputPtr, outputRegionForThread );
  IndexType currentIndex;
  PointType currentPoint;
  itk::ContinuousIndex<double,DisplacementFieldType::ImageDimension> contiIndex;

  while(!outputIt.IsAtEnd())
    {
    // get the output image index
    currentIndex = outputIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint(currentIndex,currentPoint);
    fieldPtr->TransformPhysicalPointToContinuousIndex(currentPoint,contiIndex);

    for (unsigned int dim = 0; dim<DisplacementFieldType::ImageDimension; ++dim)
      {
      if (contiIndex[dim] < static_cast<double>(defRegion.GetIndex(dim)) ||
          contiIndex[dim] > static_cast<double>(defRegion.GetIndex(dim)+defRegion.GetSize(dim)-1))
        {
        outputIt.Set(paddingValue);
        break;
        }
      }
    ++outputIt;
    }
  }

template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
 {
  Superclass::PrintSelf(os, indent);
  os << indent << "Maximum displacement: " << m_MaximumDisplacement << std::endl;
 }

} // end namespace otb
#endif
