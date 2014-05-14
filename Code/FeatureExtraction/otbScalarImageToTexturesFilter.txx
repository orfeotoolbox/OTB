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

#ifndef __otbScalarImageToTexturesFilter_txx
#define __otbScalarImageToTexturesFilter_txx

#include "otbScalarImageToTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"
#include <vector>
#include <cmath>

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToTexturesFilter()
: m_Radius()
, m_Offset()
, m_NeighborhoodRadius()
, m_NumberOfBinsPerAxis(8)
, m_InputImageMinimum(0)
, m_InputImageMaximum(255)
{
  // There are 8 outputs corresponding to the 8 textures indices
  this->SetNumberOfRequiredOutputs(8);

  // Create the 8 outputs
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());
  this->SetNthOutput(4, OutputImageType::New());
  this->SetNthOutput(5, OutputImageType::New());
  this->SetNthOutput(6, OutputImageType::New());
  this->SetNthOutput(7, OutputImageType::New());
}

template <class TInputImage, class TOutputImage>
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::~ScalarImageToTexturesFilter()
{}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetEnergyOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetCorrelationOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(2));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetInverseDifferenceMomentOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(3));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetInertiaOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(4));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetClusterShadeOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(5));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetClusterProminenceOutput()
{
  if (this->GetNumberOfOutputs() < 7)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(6));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::GetHaralickCorrelationOutput()
{
  if (this->GetNumberOfOutputs() < 8)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(7));
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
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

  typename OutputRegionType::IndexType outputIndex = outputRequestedRegion.GetIndex();
  typename OutputRegionType::SizeType  outputSize   = outputRequestedRegion.GetSize();
  typename InputRegionType::IndexType  inputIndex;
  typename InputRegionType::SizeType   inputSize;

  // First, apply offset
  for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
    {
    inputIndex[dim] = std::min(outputIndex[dim], outputIndex[dim] + m_Offset[dim]);
    inputSize[dim] =
      std::max(outputIndex[dim] + outputSize[dim], outputIndex[dim] + outputSize[dim] +
               m_Offset[dim]) - inputIndex[dim];
    }

  // Build the input requested region
  InputRegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(inputIndex);
  inputRequestedRegion.SetSize(inputSize);

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
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  unsigned int minRadius = 0;
  for ( unsigned int i = 0; i < m_Offset.GetOffsetDimension(); i++ )
    {
    unsigned int distance = vcl_abs(m_Offset[i]);
    if ( distance > minRadius )
      {
      minRadius = distance;
      }
    }
  m_NeighborhoodRadius.Fill(minRadius);
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr             =      const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType energyPtr            =      this->GetEnergyOutput();
  OutputImagePointerType entropyPtr           =      this->GetEntropyOutput();
  OutputImagePointerType correlationPtr       =      this->GetCorrelationOutput();
  OutputImagePointerType invDiffMomentPtr     =      this->GetInverseDifferenceMomentOutput();
  OutputImagePointerType inertiaPtr           =      this->GetInertiaOutput();
  OutputImagePointerType clusterShadePtr      =      this->GetClusterShadeOutput();
  OutputImagePointerType clusterProminencePtr =      this->GetClusterProminenceOutput();
  OutputImagePointerType haralickCorPtr       =      this->GetHaralickCorrelationOutput();

  // Build output iterators
  itk::ImageRegionIteratorWithIndex<OutputImageType> energyIt(energyPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          entropyIt(entropyPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          correlationIt(correlationPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          invDiffMomentIt(invDiffMomentPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          inertiaIt(inertiaPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          clusterShadeIt(clusterShadePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          clusterProminenceIt(clusterProminencePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          haralickCorIt(haralickCorPtr, outputRegionForThread);

  // Go to begin
  energyIt.GoToBegin();
  entropyIt.GoToBegin();
  correlationIt.GoToBegin();
  invDiffMomentIt.GoToBegin();
  inertiaIt.GoToBegin();
  clusterShadeIt.GoToBegin();
  clusterProminenceIt.GoToBegin();
  haralickCorIt.GoToBegin();

  const double log2 = vcl_log(2.0);

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!energyIt.IsAtEnd()
         && !entropyIt.IsAtEnd()
         && !correlationIt.IsAtEnd()
         && !invDiffMomentIt.IsAtEnd()
         && !inertiaIt.IsAtEnd()
         && !clusterShadeIt.IsAtEnd()
         && !clusterProminenceIt.IsAtEnd()
         && !haralickCorIt.IsAtEnd())
    {
    // Compute the region on which co-occurence will be estimated
    typename InputRegionType::IndexType inputIndex;
    typename InputRegionType::SizeType inputSize;

    // First, create an window for neighborhood iterator based on m_Radius
    // For example, if xradius and yradius is 2. window size is 5x5 (2 * radius + 1).
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      inputIndex[dim] = energyIt.GetIndex()[dim] - m_Radius[dim];
      inputSize[dim] = 2 * m_Radius[dim] + 1;
      }

    // Build the input  region
    InputRegionType inputRegion;
    inputRegion.SetIndex(inputIndex);
    inputRegion.SetSize(inputSize);
    inputRegion.Crop(inputPtr->GetRequestedRegion());

    CooccurrenceIndexedListPointerType GLCIList = CooccurrenceIndexedListType::New();
    GLCIList->Initialize(m_NumberOfBinsPerAxis, m_InputImageMinimum, m_InputImageMaximum);

    typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
    NeighborhoodIteratorType neighborIt;
    neighborIt = NeighborhoodIteratorType(m_NeighborhoodRadius, inputPtr, inputRegion);
    for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
      {
      const InputPixelType centerPixelIntensity = neighborIt.GetCenterPixel();
      bool pixelInBounds;
      const InputPixelType pixelIntensity =  neighborIt.GetPixel(m_Offset, pixelInBounds);
      if ( !pixelInBounds )
        {
        continue; // don't put a pixel in the histogram if it's out-of-bounds.
        }
      GLCIList->AddPixelPair(centerPixelIntensity, pixelIntensity);
      }

    double pixelMean = 0.;
    double marginalMean;
    double marginalDevSquared = 0.;
    double pixelVariance = 0.;

    //Create and Intialize marginalSums
    std::vector<double> marginalSums(m_NumberOfBinsPerAxis, 0);

    //get co-occurrence vector and totalfrequency
    VectorType glcVector = GLCIList->GetVector();
    double totalFrequency = static_cast<double> (GLCIList->GetTotalFrequency());

    //Normalize the co-occurrence indexed list and compute mean, marginalSum
    typename VectorType::iterator it = glcVector.begin();
    while( it != glcVector.end())
      {
      double frequency = (*it).second / totalFrequency;
      CooccurrenceIndexType index = (*it).first;
      pixelMean += index[0] * frequency;
      marginalSums[index[0]] += frequency;
      ++it;
      }

    /* Now get the mean and deviaton of the marginal sums.
       Compute incremental mean and SD, a la Knuth, "The  Art of Computer
       Programming, Volume 2: Seminumerical Algorithms",  section 4.2.2.
       Compute mean and standard deviation using the recurrence relation:
       M(1) = x(1), M(k) = M(k-1) + (x(k) - M(k-1) ) / k
       S(1) = 0, S(k) = S(k-1) + (x(k) - M(k-1)) * (x(k) - M(k))
       for 2 <= k <= n, then
       sigma = vcl_sqrt(S(n) / n) (or divide by n-1 for sample SD instead of
       population SD).
     */
    std::vector<double>::const_iterator msIt = marginalSums.begin();
    marginalMean = *msIt;
    //Increment iterator to start with index 1
    ++msIt;
    for(int k= 2; msIt != marginalSums.end(); ++k, ++msIt)
      {
      double M_k_minus_1 = marginalMean;
      double S_k_minus_1 = marginalDevSquared;
      double x_k = *msIt;
      double M_k = M_k_minus_1 + ( x_k - M_k_minus_1 ) / k;
      double S_k = S_k_minus_1 + ( x_k - M_k_minus_1 ) * ( x_k - M_k );
      marginalMean = M_k;
      marginalDevSquared = S_k;
      }
    marginalDevSquared = marginalDevSquared / m_NumberOfBinsPerAxis;

    VectorConstIteratorType constVectorIt;
    constVectorIt = glcVector.begin();
    while( constVectorIt != glcVector.end())
    {
    RelativeFrequencyType frequency = (*constVectorIt).second / totalFrequency;
    CooccurrenceIndexType        index = (*constVectorIt).first;
    pixelVariance += ( index[0] - pixelMean ) * ( index[0] - pixelMean ) * frequency;
    ++constVectorIt;
    }

    double pixelVarianceSquared = pixelVariance * pixelVariance;
    // Variance is only used in correlation. If variance is 0, then (index[0] - pixelMean) * (index[1] - pixelMean)
    // should be zero as well. In this case, set the variance to 1. in order to
    // avoid NaN correlation.
    if(pixelVarianceSquared < m_PixelValueTolerance)
      {
      pixelVarianceSquared = 1.;
      }

    //Initalize texture variables;
    PixelValueType energy      = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType entropy     = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType correlation = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType inverseDifferenceMoment      = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType inertia             = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType clusterShade        = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType clusterProminence   = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType haralickCorrelation = itk::NumericTraits< PixelValueType >::Zero;

    //Compute textures
    constVectorIt = glcVector.begin();
    while( constVectorIt != glcVector.end())
      {
      CooccurrenceIndexType index = (*constVectorIt).first;
      RelativeFrequencyType frequency = (*constVectorIt).second / totalFrequency;
      energy += frequency * frequency;
      entropy -= ( frequency > m_PixelValueTolerance ) ? frequency *vcl_log(frequency) / log2 : 0;
      correlation += ( ( index[0] - pixelMean ) * ( index[1] - pixelMean ) * frequency ) / pixelVarianceSquared;
      inverseDifferenceMoment += frequency / ( 1.0 + ( index[0] - index[1] ) * ( index[0] - index[1] ) );
      inertia += ( index[0] - index[1] ) * ( index[0] - index[1] ) * frequency;
      clusterShade += vcl_pow( ( index[0] - pixelMean ) + ( index[1] - pixelMean ), 3 ) * frequency;
      clusterProminence += vcl_pow( ( index[0] - pixelMean ) + ( index[1] - pixelMean ), 4 ) * frequency;
      haralickCorrelation += index[0] * index[1] * frequency;
      ++constVectorIt;
      }

    haralickCorrelation = (fabs(marginalDevSquared) > 1E-8) ?
      ( haralickCorrelation - marginalMean * marginalMean )  / marginalDevSquared : 0;

    // Fill outputs
    energyIt.Set(energy);
    entropyIt.Set(entropy);
    correlationIt.Set(correlation);
    invDiffMomentIt.Set(inverseDifferenceMoment);
    inertiaIt.Set(inertia);
    clusterShadeIt.Set(clusterShade);
    clusterProminenceIt.Set(clusterProminence);
    haralickCorIt.Set(haralickCorrelation);

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    ++energyIt;
    ++entropyIt;
    ++correlationIt;
    ++invDiffMomentIt;
    ++inertiaIt;
    ++clusterShadeIt;
    ++clusterProminenceIt;
    ++haralickCorIt;
    }
}

} // End namespace otb

#endif
