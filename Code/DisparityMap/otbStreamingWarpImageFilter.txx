/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingWarpImageFilter_txx
#define __otbStreamingWarpImageFilter_txx

#include "otbStreamingWarpImageFilter.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{

template<class TInputImage, class TOutputImage, class TDisplacementField>
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::StreamingWarpImageFilter()
 {
  // Fill the default maximum displacement
  m_MaximumDisplacement.Fill(1);
 }

template<class TInputImage, class TOutputImage, class TDisplacementField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
::GenerateInputRequestedRegion()
 {
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

  // Convert physical bouding box to requested region
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
  else
    {

    inputFinalSize.Fill(0);
    inputRequestedRegion.SetSize(inputFinalSize);
    inputFinalIndex.Fill(0);
    inputRequestedRegion.SetIndex(inputFinalIndex);

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

//    // build an exception
//    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
//    e.SetLocation(ITK_LOCATION);
//    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
//    e.SetDataObject(inputPtr);
//    throw e;
    }
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
  DisplacementFieldPointerType fieldPtr = this->GetDisplacementField();
  
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
