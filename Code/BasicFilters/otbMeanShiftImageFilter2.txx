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

#ifndef __otbMeanShiftImageFilter2_txx
#define __otbMeanShiftImageFilter2_txx

#include "otbMeanShiftImageFilter2.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h"

#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::MeanShiftImageFilter2()
{
  m_MaxIterationNumber = 4;
  m_SpatialBandwidth = 3;
  m_RangeBandwidth=16.;
  m_Threshold=1e-3;

  this->SetNumberOfOutputs(4);
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputMetricImageType::New());
  this->SetNthOutput(3, OutputIterationImageType::New());
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::~ MeanShiftImageFilter2()
{

}

template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
const typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetSpatialOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetSpatialOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
const typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetRangeOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetRangeOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
const typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputMetricImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetMetricOutput() const
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<const OutputMetricImageType *>(this->itk::ProcessObject::GetOutput(2));
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputMetricImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetMetricOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputMetricImageType *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputIterationImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetIterationOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
      return 0;
    }
  return static_cast<OutputIterationImageType *>(this->itk::ProcessObject::GetOutput(3));
}

template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
const typename MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>::OutputIterationImageType *
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GetIterationOutput() const
{
  if (this->GetNumberOfOutputs() < 4)
    {
      return 0;
    }
  return static_cast<OutputIterationImageType *>(this->itk::ProcessObject::GetOutput(3));
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::AllocateOutputs()
{

  typename OutputImageType::Pointer   spatialOutputPtr = this->GetSpatialOutput();
  typename OutputImageType::Pointer   rangeOutputPtr = this->GetRangeOutput();
  typename OutputImageType::Pointer   metricOutputPtr = this->GetMetricOutput();
  typename OutputIterationImageType::Pointer iterationOutputPtr = this->GetIterationOutput();

  metricOutputPtr->SetBufferedRegion(metricOutputPtr->GetRequestedRegion());
  metricOutputPtr->Allocate();

  spatialOutputPtr->SetBufferedRegion(spatialOutputPtr->GetRequestedRegion());
  spatialOutputPtr->Allocate();

  rangeOutputPtr->SetBufferedRegion(rangeOutputPtr->GetRequestedRegion());
  rangeOutputPtr->Allocate();

  iterationOutputPtr->SetBufferedRegion(iterationOutputPtr->GetRequestedRegion());
  iterationOutputPtr->Allocate();

 }


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  m_NumberOfComponentsPerPixel = this->GetInput()->GetNumberOfComponentsPerPixel();

  if (this->GetSpatialOutput())
    {
    this->GetSpatialOutput()->SetNumberOfComponentsPerPixel(ImageDimension); // image lattice
    }
  if (this->GetSpatialOutput())
    {
    this->GetRangeOutput()->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
    }
  if (this->GetMetricOutput())
    {
    this->GetMetricOutput()->SetNumberOfComponentsPerPixel(ImageDimension + m_NumberOfComponentsPerPixel); // Spectral Part + lattice
    }
}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::GenerateInputRequestedRegion()
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input pointers
  TInputImage * inPtr  = const_cast<TInputImage *>(this->GetInput());

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputImage * outSpatialPtr = this->GetSpatialOutput();
  TOutputImage * outRangePtr = this->GetRangeOutput();
  OutputIterationImageType * outIterationPtr = this->GetIterationOutput();

  // Check pointers before using them
  if(!inPtr || !outMetricPtr || !outSpatialPtr || !outRangePtr || !outIterationPtr)
    {
    return;
    }


  // Retrieve requested region (TODO: check if we need to handle
  // region for outHDispPtr)
  RegionType outputRequestedRegion = outMetricPtr->GetRequestedRegion();

  // spatial and range radius may differ, padding must be done with the largest.
  //unsigned int largestRadius= this->GetLargestRadius();
  // SHE: commented out, only the spatial radius has an effect on the input region size
  //InputSizeType largestRadius= this->GetLargestRadius();
  // Pad by the appropriate radius
  RegionType inputRequestedRegion  = outputRequestedRegion;

  // Initializes the kernel bandwidth to calculate its radius
  m_SpatialKernel.SetBandwidth(m_SpatialBandwidth);
  m_SpatialRadius.Fill(m_SpatialKernel.GetRadius());

  inputRequestedRegion.PadByRadius(m_SpatialRadius);

  // Crop the input requested region at the input's largest possible region
   if ( inputRequestedRegion.Crop(inPtr->GetLargestPossibleRegion()) )
     {
     inPtr->SetRequestedRegion( inputRequestedRegion );
     return;
     }
   else
     {
     // Couldn't crop the region (requested region is outside the largest
     // possible region).  Throw an exception.

     // store what we tried to request (prior to trying to crop)
     inPtr->SetRequestedRegion( inputRequestedRegion );

     // build an exception
     itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
     e.SetLocation(ITK_LOCATION);
     e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
     e.SetDataObject(inPtr);
     throw e;
     }

}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::BeforeThreadedGenerateData()
{

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputImage * outSpatialPtr   = this->GetSpatialOutput();
  TOutputImage * outRangePtr   = this->GetRangeOutput();

  m_SpatialKernel.SetBandwidth(m_SpatialBandwidth);
  m_RangeKernel.SetBandwidth(m_RangeBandwidth);

  m_SpatialRadius.Fill(m_SpatialKernel.GetRadius());

  m_NumberOfComponentsPerPixel = this->GetInput()->GetNumberOfComponentsPerPixel();
}


// Calculates the mean shift vector at the position given by jointPixel
template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::CalculateMeanShiftVector(typename InputImageType::ConstPointer inputImage, RealVector jointPixel, const OutputRegionType& outputRegion, RealVector & meanShiftVector)
 {
  RealVector jointNeighbor;

  jointNeighbor.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);

  RealType weightSum = 0;
  InputPixelType inputPixel;
  InputIndexType inputIndex;
  InputIndexType regionIndex;
  InputSizeType  regionSize;
  RegionType neighborhoodRegion;

  meanShiftVector.Fill(0.);

  // Calculates current pixel neighborhood region, restricted to the output image region
  for(unsigned int comp = 0; comp < ImageDimension; ++comp)
    {
    long int indexRight;
    inputIndex[comp] = jointPixel[comp];

    regionIndex[comp] = vcl_max(static_cast<long int>(outputRegion.GetIndex().GetElement(comp)), static_cast<long int>(inputIndex[comp] - m_SpatialRadius[comp]));
    indexRight = vcl_min(static_cast<long int>(outputRegion.GetIndex().GetElement(comp) + outputRegion.GetSize().GetElement(comp) - 1), static_cast<long int>(inputIndex[comp] + m_SpatialRadius[comp]));

    // regionSize[comp] = vcl_max(0l, indexRight - static_cast<long int>(regionIndex[comp] + 1));
    regionSize[comp] = indexRight - regionIndex[comp] + 1;
    }

  neighborhoodRegion.SetIndex(regionIndex); // TODO Handle region borders
  neighborhoodRegion.SetSize(regionSize); //TODO Add +1 for each dimension

  // An iterator on the neighborhood of the current pixel
  itk::ImageRegionConstIteratorWithIndex<InputImageType> it(inputImage, neighborhoodRegion);

  //std::cout << neighborhoodRegion << std::endl;
  it.GoToBegin();
  while(!it.IsAtEnd())
    {
    inputIndex = it.GetIndex();
    inputPixel = it.Get();
    RealVector diff;
    RealType norm2;
    RealType weight;

    // Write the current pixel of the neighborhood in the joint spatial-range domain
    for (unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      jointNeighbor[comp] = inputIndex[comp];
      }
    for (unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      jointNeighbor[ImageDimension + comp] = inputPixel[comp];
      }

    // Calculate the squared norm of the difference
    diff = jointNeighbor - jointPixel;

    // Scale diff vector elements by the bandwidth
    for (unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      diff[comp] /= m_SpatialBandwidth;
      }
    for (unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      diff[ImageDimension + comp] /= m_RangeBandwidth;
      }

    // Compute the squared norm of the difference
    // This is the L_inf norm, TODO: replace by the templated norm
    norm2 = 0;
    for (unsigned int comp = 0; comp < ImageDimension + m_NumberOfComponentsPerPixel; comp++)
      {
      norm2 += vcl_max(norm2, vcl_abs(diff[comp]));
      }
    norm2 *= norm2;

    // Compute pixel weight from kernel
    // TODO : replace by the templated kernel
    weight = (norm2 <= 1.0)? 1.0 : 0.0;

    // Update sum of weights
    weightSum += weight;

    // Update mean shift vector
    meanShiftVector += weight * jointNeighbor;

    ++it;
    }

  if(weightSum > 0)
    {
    meanShiftVector /= weightSum;
    meanShiftVector -= jointPixel;
    }
  else
    meanShiftVector.Fill(0);
 }


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, int threadId)
{
  // at the first iteration

  // Allocate output images
  this->AllocateOutputs();

  RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Retrieve output images pointers
  typename OutputImageType::Pointer spatialOutput = this->GetSpatialOutput();
  typename OutputImageType::Pointer rangeOutput = this->GetRangeOutput();
  typename OutputMetricImageType::Pointer metricOutput = this->GetMetricOutput();
  typename OutputIterationImageType::Pointer iterationOutput = this->GetIterationOutput();

  // Get input image pointer
  typename InputImageType::ConstPointer input = this->GetInput();

  // defines input and output iterators
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typedef itk::ImageRegionIterator<OutputMetricImageType> OutputMetricIteratorType;
  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorWithIndexType;
  typedef itk::ImageRegionIterator<OutputIterationImageType> OutputIterationIteratorType;


  typename InputImageType::PixelType inputPixel;
  typename OutputImageType::PixelType rangePixel;
  typename OutputImageType::PixelType spatialPixel;
  typename OutputMetricImageType::PixelType metricPixel;
  typename OutputIterationImageType::PixelType iterationPixel;


  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());


  InputIteratorWithIndexType inputIt(input, inputRegionForThread);
  OutputIteratorType rangeIt(rangeOutput, outputRegionForThread);
  OutputIteratorType spatialIt(spatialOutput, outputRegionForThread);
  OutputMetricIteratorType metricIt(metricOutput, outputRegionForThread);
  OutputIterationIteratorType iterationIt(iterationOutput, outputRegionForThread);

  inputIt.GoToBegin();
  rangeIt.GoToBegin();
  spatialIt.GoToBegin();
  metricIt.GoToBegin();
  iterationIt.GoToBegin();

  unsigned int iteration = 0;


  // Pixel in the joint spatial-range domain
  RealVector jointPixel;

  // Mean shift vector, updating the joint pixel at each iteration
  RealVector meanShiftVector;


  jointPixel.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);
  meanShiftVector.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);


  while (!inputIt.IsAtEnd())
    {
    bool hasConverged = false;

    InputIndexType index = inputIt.GetIndex();
    inputPixel = inputIt.Get();
    rangePixel = rangeIt.Get();
    spatialPixel = spatialIt.Get();
    metricPixel = metricIt.Get();


    // Initialize pixel in the joint spatial-range domain
    for(unsigned int comp = 0; comp < ImageDimension; ++comp)
      {
      jointPixel.SetElement(comp, index[comp]);
      }

    for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; ++comp)
      {
      jointPixel.SetElement(ImageDimension+comp, inputPixel[comp]);
      }

    iteration = 0;
    while ((iteration < m_MaxIterationNumber) && (!hasConverged))
      {
      double meanShiftVectorSqNorm;

      //Calculate meanShiftVector
      this->CalculateMeanShiftVector(input, jointPixel, outputRegionForThread, meanShiftVector);
      meanShiftVectorSqNorm = meanShiftVector.GetSquaredNorm();
      jointPixel += meanShiftVector;

      //TODO replace SSD Test with templated metric
      hasConverged = meanShiftVectorSqNorm < m_Threshold;
      iteration++;
      }

    for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      rangePixel[comp] = jointPixel[ImageDimension + comp];
      }
    for(unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      spatialPixel[comp] = jointPixel[comp];
      }

    for(unsigned int comp = 0; comp < ImageDimension+m_NumberOfComponentsPerPixel; comp++)
      {
      metricPixel[comp] = meanShiftVector[comp] * meanShiftVector[comp];
      }

    rangeIt.Set(rangePixel);
    spatialIt.Set(spatialPixel);
    metricIt.Set(metricPixel);

    iterationPixel = iteration;
    iterationIt.Set(iterationPixel);

    ++inputIt;
    ++rangeIt;
    ++spatialIt;
    ++metricIt;
    ++iterationIt;

    progress.CompletedPixel();

    }
}


/* after threaded convergence test */
template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::AfterThreadedGenerateData()
{


}


template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Spatial bandwidth: "                << m_SpatialBandwidth               << std::endl;
  os << indent << "Range bandwidth: "                  << m_RangeBandwidth                 << std::endl;
 }

} // end namespace otb

#endif
