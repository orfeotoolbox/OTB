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

template<class TInputImage, class TOutputImage, class TDeformationField>
StreamingWarpImageFilter<TInputImage, TOutputImage, TDeformationField>
::StreamingWarpImageFilter()
 {
  // Fill the default maximum deformation
  m_MaximumDeformation.Fill(1);
 }

template<class TInputImage, class TOutputImage, class TDeformationField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateInputRequestedRegion()
 {
  // Get the input and deformation field pointers
  InputImageType         *       inputPtr
  = const_cast<InputImageType *>(this->GetInput());
  DeformationFieldType   * deformationPtr
  = const_cast<DeformationFieldType*>(this->GetDeformationField());
  OutputImageType * outputPtr
  = this->GetOutput();
  // Check if the input and the deformation field exist
  if (!inputPtr || !deformationPtr || !outputPtr)
    {
    return;
    }

  // Here we are breaking traditional pipeline steps because we need to access the deformation field data
  // so as to compute the input image requested region

  // 1) First, evaluate the deformation field requested region corresponding to the output requested region
  // (Here we suppose that the deformation field and the output image are in the same geometry/map projection)
  typename OutputImageType::RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();
  typename OutputImageType::IndexType outIndexStart = outputRequestedRegion.GetIndex();
  typename OutputImageType::IndexType outIndexEnd;
  for(unsigned int dim = 0; dim<OutputImageType::ImageDimension; ++dim)
    outIndexEnd[dim]= outIndexStart[dim] + outputRequestedRegion.GetSize()[dim]-1;
  typename OutputImageType::PointType outPointStart, outPointEnd;
  outputPtr->TransformIndexToPhysicalPoint(outIndexStart, outPointStart);
  outputPtr->TransformIndexToPhysicalPoint(outIndexEnd, outPointEnd);

  typename DeformationFieldType::IndexType defIndexStart, defIndexEnd;
  deformationPtr->TransformPhysicalPointToIndex(outPointStart, defIndexStart);
  deformationPtr->TransformPhysicalPointToIndex(outPointEnd, defIndexEnd);

  typename DeformationFieldType::SizeType defRequestedSize;
  typename DeformationFieldType::IndexType defRequestedIndex;

  for(unsigned int dim = 0; dim<OutputImageType::ImageDimension; ++dim)
    {
    defRequestedIndex[dim] = std::min(defIndexStart[dim], defIndexEnd[dim]);
    defRequestedSize[dim] = std::max(defIndexStart[dim], defIndexEnd[dim]) - defRequestedIndex[dim] + 1;
    }

  // Finally, build the deformation field requested region
  typename DeformationFieldType::RegionType deformationRequestedRegion;
  deformationRequestedRegion.SetIndex(defRequestedIndex);
  deformationRequestedRegion.SetSize(defRequestedSize);

  // Avoid extrapolation
  deformationRequestedRegion.PadByRadius(1);

  // crop the input requested region at the input's largest possible region
  if (deformationRequestedRegion.Crop(deformationPtr->GetLargestPossibleRegion()))
    {
    deformationPtr->SetRequestedRegion(deformationRequestedRegion);
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    deformationPtr->SetRequestedRegion(deformationRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of the deformation field.");
    e.SetDataObject(inputPtr);
    throw e;
    }

  // 2) If we are still there, we have a correct deformation field requested region.
  // This next step breaks pipeline rule but we need to do it to compute the input requested region,
  // since it depends on deformation value.

  // Trigger pipeline update on the deformation field

  deformationPtr->PropagateRequestedRegion();
  deformationPtr->UpdateOutputData();

  // Walk the loaded deformation field to derive maximum and minimum
  itk::ImageRegionIteratorWithIndex<DeformationFieldType> defIt(deformationPtr, deformationRequestedRegion);
  defIt.GoToBegin();

  typename InputImageType::PointType currentPoint;
  typename InputImageType::PointType inputStartPoint, inputEndPoint;

  // Initialise start and end points
  deformationPtr->TransformIndexToPhysicalPoint(defIt.GetIndex(), currentPoint);
  for(unsigned int dim = 0; dim<DeformationFieldType::ImageDimension; ++dim)
    {
    currentPoint[dim]+=defIt.Get()[dim];
    }
  inputStartPoint = currentPoint;
  inputEndPoint = currentPoint;

  ++defIt;

  // 3) Now Walk the field and compute the physical bounding box
  while(!defIt.IsAtEnd())
    {
    deformationPtr->TransformIndexToPhysicalPoint(defIt.GetIndex(), currentPoint);
    for(unsigned int dim = 0; dim<DeformationFieldType::ImageDimension; ++dim)
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

  for(unsigned int dim = 0; dim<DeformationFieldType::ImageDimension; ++dim)
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


template<class TInputImage, class TOutputImage, class TDeformationField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDeformationField>
::ThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  int threadId )
  {
  // the superclass itk::WarpImageFilter is doing the actual warping
  Superclass::ThreadedGenerateData(outputRegionForThread,threadId);
  
  // second pass on the thread region to mask pixels outside the deformation grid
  const PixelType paddingValue = this->GetEdgePaddingValue();
  OutputImagePointerType outputPtr = this->GetOutput();
  DeformationFieldPointerType fieldPtr = this->GetDeformationField();
  
  DeformationFieldRegionType defRegion = fieldPtr->GetLargestPossibleRegion();
  
  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(
    outputPtr, outputRegionForThread );
  IndexType currentIndex;
  PointType currentPoint;
  itk::ContinuousIndex<double,DeformationFieldType::ImageDimension> contiIndex;
  
  while(!outputIt.IsAtEnd())
    {
    // get the output image index
    currentIndex = outputIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint(currentIndex,currentPoint);
    fieldPtr->TransformPhysicalPointToContinuousIndex(currentPoint,contiIndex);
    
    for (unsigned int dim = 0; dim<DeformationFieldType::ImageDimension; ++dim)
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


template<class TInputImage, class TOutputImage, class TDeformationField>
void
StreamingWarpImageFilter<TInputImage, TOutputImage, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
 {
  Superclass::PrintSelf(os, indent);
  os << indent << "Maximum deformation: " << m_MaximumDeformation << std::endl;
 }

} // end namespace otb
#endif
