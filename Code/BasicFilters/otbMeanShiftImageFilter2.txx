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
#include "otbUnaryFunctorWithIndexWithOutputSizeImageFilter.h"
#include "otbMacro.h"

#include "itkProgressReporter.h"

#define MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION

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
  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorWithIndexType;
  typedef itk::ImageRegionIterator<RealVectorImageType> JointImageIteratorType;

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputImage * outSpatialPtr   = this->GetSpatialOutput();
  TOutputImage * outRangePtr   = this->GetRangeOutput();
  typename InputImageType::ConstPointer inputPtr = this->GetInput();

  InputIndexType index;

  typename InputImageType::PixelType inputPixel;
  RealVector jointPixel;

  m_SpatialKernel.SetBandwidth(m_SpatialBandwidth);
  m_RangeKernel.SetBandwidth(m_RangeBandwidth);

  m_SpatialRadius.Fill(m_SpatialKernel.GetRadius());

  m_NumberOfComponentsPerPixel = this->GetInput()->GetNumberOfComponentsPerPixel();


  // m_JointImage is the input data expressed in the joint spatial-range
  // domain, i.e. spatial coordinates are concatenated to the range values.
  // Moreover, pixel components in this image are normalized by their respective
  // (spatial or range) bandwith.
  typedef SpatialRangeJointDomainTransform<InputImageType, RealVectorImageType> FunctionType;
  typedef otb::UnaryFunctorWithIndexWithOutputSizeImageFilter<InputImageType, RealVectorImageType, FunctionType> JointImageFunctorType;

  typename JointImageFunctorType::Pointer jointImageFunctor = JointImageFunctorType::New();

  jointImageFunctor->SetInput(inputPtr);
  jointImageFunctor->GetFunctor().Initialize(ImageDimension, m_NumberOfComponentsPerPixel, m_SpatialBandwidth, m_RangeBandwidth);
  jointImageFunctor->Update();
  m_JointImage = jointImageFunctor->GetOutput();

/*
  // Allocate the joint domain image
  m_JointImage = RealVectorImageType::New();
  m_JointImage->SetNumberOfComponentsPerPixel(ImageDimension + m_NumberOfComponentsPerPixel);
  m_JointImage->SetRegions(inputPtr->GetRequestedRegion());
  m_JointImage->Allocate();

  InputIteratorWithIndexType inputIt(inputPtr, inputPtr->GetRequestedRegion());
  JointImageIteratorType jointIt(m_JointImage, inputPtr->GetRequestedRegion());

  // Initialize the joint image with scaled values
  inputIt.GoToBegin();
  jointIt.GoToBegin();

  while (!inputIt.IsAtEnd())
    {
    inputPixel = inputIt.Get();
    index = inputIt.GetIndex();

    jointPixel = jointIt.Get();
    for(unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      jointPixel[comp] = index[comp] / m_SpatialBandwidth;
      }
    for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      jointPixel[ImageDimension + comp] = inputPixel[comp] / m_RangeBandwidth;
      }
    jointIt.Set(jointPixel);

    ++inputIt;
    ++jointIt;
    }
*/

#ifdef MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION
  // Image to store the status at each pixel:
  // 0 : no mode has been found yet
  // 1 : a mode has been assigned to this pixel
  // 2 : a mode will be assigned to this pixel
  m_modeTable = ModeTableImageType::New();
  m_modeTable->SetRegions(inputPtr->GetRequestedRegion());
  m_modeTable->Allocate();
  m_modeTable->FillBuffer(0);
#endif

}


// Calculates the mean shift vector at the position given by jointPixel
template <class TInputImage, class TOutputImage, class TKernel, class TNorm, class TOutputMetricImage, class TOutputIterationImage>
void
MeanShiftImageFilter2<TInputImage, TOutputImage, TKernel, TNorm, TOutputMetricImage, TOutputIterationImage>
::CalculateMeanShiftVector(typename RealVectorImageType::Pointer jointImage, RealVector jointPixel, const OutputRegionType& outputRegion, RealVector & meanShiftVector)
 {
  unsigned int jointDimension = ImageDimension + m_NumberOfComponentsPerPixel;
  RealVector jointNeighbor;

  RealType weightSum = 0;
  InputIndexType inputIndex;
  InputIndexType regionIndex;
  InputSizeType  regionSize;
  RegionType neighborhoodRegion;

  meanShiftVector.Fill(0.);
  jointNeighbor.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);

  // Calculates current pixel neighborhood region, restricted to the output image region
  for(unsigned int comp = 0; comp < ImageDimension; ++comp)
    {
    long int indexRight;
    inputIndex[comp] = jointPixel[comp] * m_SpatialBandwidth;

    regionIndex[comp] = vcl_max(static_cast<long int>(outputRegion.GetIndex().GetElement(comp)), static_cast<long int>(inputIndex[comp] - m_SpatialRadius[comp]));
    indexRight = vcl_min(static_cast<long int>(outputRegion.GetIndex().GetElement(comp) + outputRegion.GetSize().GetElement(comp) - 1), static_cast<long int>(inputIndex[comp] + m_SpatialRadius[comp]));

    regionSize[comp] = vcl_max(0l, indexRight - static_cast<long int>(regionIndex[comp]) + 1);
    }

  neighborhoodRegion.SetIndex(regionIndex); // TODO Handle region borders
  neighborhoodRegion.SetSize(regionSize); //TODO Add +1 for each dimension

  // An iterator on the neighborhood of the current pixel (in joint
  // spatial-range domain)
  itk::ImageRegionConstIteratorWithIndex<RealVectorImageType> it(jointImage, neighborhoodRegion);

  it.GoToBegin();
  while(!it.IsAtEnd())
    {
    RealType norm2;
    RealType weight;

    jointNeighbor = it.Get();

    // Compute the squared norm of the difference
    // This is the L2 norm, TODO: replace by the templated norm
    norm2 = 0;
    for(unsigned int comp = 0; comp < jointDimension; comp++)
      {
      RealType d;
      d = jointNeighbor[comp] - jointPixel[comp];
      norm2 += d*d;
      }

    // Compute pixel weight from kernel
    // TODO : replace by the templated kernel
    weight = (norm2 <= 1.0)? 1.0 : 0.0;


/*
    // The following code is an alternative way to compute norm2 and weight
    // It separates the norms of spatial and range elements
    RealType spatialNorm2;
    RealType rangeNorm2;
    spatialNorm2 = 0;
    for (unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      RealType d;
      d = jointNeighbor[comp] - jointPixel[comp];
      spatialNorm2 += d*d;
      }

    if(spatialNorm2 >= 1.0)
      {
      weight = 0;
      }
    else
      {
      rangeNorm2 = 0;
      for (unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
        {
        RealType d;
        d = jointNeighbor[ImageDimension + comp] - jointPixel[ImageDimension + comp];
        rangeNorm2 += d*d;
        }

      weight = (rangeNorm2 <= 1.0)? 1.0 : 0.0;
      }
*/

    // Update sum of weights
    weightSum += weight;

    // Update mean shift vector
    for(unsigned int comp = 0; comp < jointDimension; comp++)
      {
      meanShiftVector[comp] += weight * jointNeighbor[comp];
      }


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


  typename OutputImageType::PixelType rangePixel;
  typename OutputImageType::PixelType spatialPixel;
  typename OutputMetricImageType::PixelType metricPixel;
  typename OutputIterationImageType::PixelType iterationPixel;

  InputIndexType index;

  // Pixel in the joint spatial-range domain
  RealVector jointPixel;

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  RegionType requestedRegion;
  requestedRegion = input->GetRequestedRegion();


  typedef itk::ImageRegionIterator<RealVectorImageType> JointImageIteratorType;
  JointImageIteratorType jointIt(m_JointImage, outputRegionForThread);

  OutputIteratorType rangeIt(rangeOutput, outputRegionForThread);
  OutputIteratorType spatialIt(spatialOutput, outputRegionForThread);
  OutputMetricIteratorType metricIt(metricOutput, outputRegionForThread);
  OutputIterationIteratorType iterationIt(iterationOutput, outputRegionForThread);

  jointIt.GoToBegin();
  rangeIt.GoToBegin();
  spatialIt.GoToBegin();
  metricIt.GoToBegin();
  iterationIt.GoToBegin();

  unsigned int iteration = 0;

  // Mean shift vector, updating the joint pixel at each iteration
  RealVector meanShiftVector;

  meanShiftVector.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);

#ifdef MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION
  // List of indices where the current pixel passes through
  std::vector<InputIndexType> pointList(m_MaxIterationNumber);
  // Number of points currently in the pointList
  unsigned int pointCount;
  // Number of times an already processed candidate pixel is encountered, resulting in no
  // further computation (Used for statistics only)
  unsigned int numBreaks = 0;
#endif

  while (!jointIt.IsAtEnd())
    {
    bool hasConverged = false;


    rangePixel = rangeIt.Get();
    spatialPixel = spatialIt.Get();
    metricPixel = metricIt.Get();

    jointPixel = jointIt.Get();

#ifdef MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION
    // index of the currently processed output pixel
    InputIndexType currentIndex;
    // index of the current pixel updated during the mean shift loop
    InputIndexType modeCandidate;
    for (unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      currentIndex[comp] = jointPixel[comp] * m_SpatialBandwidth + 0.5;
      }
    pointCount = 0;

#endif

    iteration = 0;
    while ((iteration < m_MaxIterationNumber) && (!hasConverged))
      {

#ifdef MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION
      // Find index of the pixel closest to the current jointPixel (not normalized by bandwidth)
      for (unsigned int comp = 0; comp < ImageDimension; comp++)
        {
        modeCandidate[comp] = jointPixel[comp] * m_SpatialBandwidth + 0.5;
        }
      // Check status of candidate mode

      // If pixel candidate has status 0 (no mode assigned) or 1 (mode assigned)
      // but not 2 (pixel in current search path), and pixel has actually moved
      // from its initial position, and pixel candidate is inside the output
      // region, then perform optimization tasks
      if (m_modeTable->GetPixel(modeCandidate) != 2 && modeCandidate != currentIndex && outputRegionForThread.IsInside(modeCandidate))
        {
        // Obtain the data point to see if it close to jointPixel
        RealVector candidatePixel;
        RealType diff = 0;
        candidatePixel = m_JointImage->GetPixel(modeCandidate);
        for (unsigned int comp = ImageDimension; comp < ImageDimension + m_NumberOfComponentsPerPixel; comp++)
          {
          RealType d;
          d = candidatePixel[comp] - jointPixel[comp];
          diff += d*d;
          }

        if (diff < 0.5) // Spectral value is close enough
          {
          // If no mode has been associated to the candidate pixel then
          // associate it to the upcoming mode
          if( m_modeTable->GetPixel(modeCandidate) == 0)
            {
            // Add the candidate to the list of pixels that will be assigned the
            // finally calculated mode value
            pointList[pointCount++] = modeCandidate;
            m_modeTable->SetPixel(modeCandidate, 2);
            } else // == 1
            {
            // The candidate pixel has already been assigned to a mode
            // Assign the same value
            rangePixel = rangeOutput->GetPixel(modeCandidate);
            for (unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
              {
              jointPixel[ImageDimension + comp] = rangePixel[comp] / m_RangeBandwidth;
              }
            // Update the mode table because pixel will be assigned just now
            m_modeTable->SetPixel(currentIndex, 2);
            // bypass further calculation
            numBreaks++;
            break;
            }
          }
        }
#endif
      //Calculate meanShiftVector
      this->CalculateMeanShiftVector(m_JointImage, jointPixel, requestedRegion, meanShiftVector);

      // Compute mean shift vector squared norm (not normalized by bandwidth)
      double meanShiftVectorSqNorm;
      meanShiftVectorSqNorm = 0;
      for(unsigned int comp = 0; comp < ImageDimension; comp++)
        {
        meanShiftVectorSqNorm += meanShiftVector[comp] * meanShiftVector[comp] * m_SpatialBandwidth*m_SpatialBandwidth;
        }
      for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
        {
        meanShiftVectorSqNorm += meanShiftVector[ImageDimension + comp] * meanShiftVector[ImageDimension + comp] * m_RangeBandwidth*m_RangeBandwidth;
        }

      jointPixel += meanShiftVector;

      //TODO replace SSD Test with templated metric
      hasConverged = meanShiftVectorSqNorm < m_Threshold;
      iteration++;
      }

    for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      rangePixel[comp] = jointPixel[ImageDimension + comp] * m_RangeBandwidth;
      }

    for(unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      spatialPixel[comp] = jointPixel[comp] * m_SpatialBandwidth;
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

#ifdef MEAN_SHIFT_MODE_SEARCH_OPTIMIZATION
    // Update the mode table now that the current pixel has been assigned
    m_modeTable->SetPixel(currentIndex, 1);

    // Also assign all points in the list to the same mode
    for(unsigned int i = 0; i < pointCount; i++)
      {
      rangeOutput->SetPixel(pointList[i], rangePixel);
      m_modeTable->SetPixel(pointList[i], 1);
      }
#endif

    ++jointIt;
    ++rangeIt;
    ++spatialIt;
    ++metricIt;
    ++iterationIt;

    progress.CompletedPixel();

    }
  // std::cout << "numBreaks: " << numBreaks << " Break ratio: " << numBreaks / (RealType)outputRegionForThread.GetNumberOfPixels() << std::endl;
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
