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
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToTexturesFilter() : m_Radius(),
  m_Offset(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(256)
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


  // Build the co-occurence matrix generator
  CoocurrenceMatrixGeneratorPointerType coOccurenceMatrixGenerator = CoocurrenceMatrixGeneratorType::New();
  coOccurenceMatrixGenerator->SetOffset(m_Offset);
  coOccurenceMatrixGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
  coOccurenceMatrixGenerator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);

  // Build the texture calculator
  TextureCoefficientsCalculatorPointerType texturesCalculator = TextureCoefficientsCalculatorType::New();

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
    typename InputRegionType::IndexType inputIndex, inputIndexWithTwiceOffset;
    typename InputRegionType::SizeType inputSize, inputSizeWithTwiceOffset;

    // First, apply offset
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      inputIndex[dim] = std::min(
                                static_cast<int>(energyIt.GetIndex()[dim] - m_Radius[dim]),
                                static_cast<int>(energyIt.GetIndex()[dim] - m_Radius[dim] + m_Offset[dim])
                                );
      inputSize[dim] = 2 * m_Radius[dim] + 1 + std::abs(m_Offset[dim]);

      inputIndexWithTwiceOffset[dim] = static_cast<int>(energyIt.GetIndex()[dim] - m_Radius[dim] - std::abs(m_Offset[dim]));
      inputSizeWithTwiceOffset[dim] = inputSize[dim] + std::abs(m_Offset[dim]);
      }

    // Build the input  region
    InputRegionType inputRegion;
    inputRegion.SetIndex(inputIndex);
    inputRegion.SetSize(inputSize);
    inputRegion.Crop(inputPtr->GetRequestedRegion());

    InputRegionType inputRegionWithTwiceOffset;
    inputRegionWithTwiceOffset.SetIndex(inputIndexWithTwiceOffset);
    inputRegionWithTwiceOffset.SetSize(inputSizeWithTwiceOffset);
    inputRegionWithTwiceOffset.Crop(inputPtr->GetRequestedRegion());

    /*********************************************************************************/
    //Local copy of the input image around the processed pixel *outputIt
    InputImagePointerType localInputImage = InputImageType::New();
    localInputImage->SetRegions(inputRegionWithTwiceOffset);
    localInputImage->Allocate();
    typedef itk::ImageRegionIteratorWithIndex<InputImageType> ImageRegionIteratorType;
    ImageRegionIteratorType itInputPtr(inputPtr, inputRegionWithTwiceOffset);
    ImageRegionIteratorType itLocalInputImage(localInputImage, inputRegionWithTwiceOffset);
    for (itInputPtr.GoToBegin(), itLocalInputImage.GoToBegin(); !itInputPtr.IsAtEnd(); ++itInputPtr, ++itLocalInputImage)
      {
      itLocalInputImage.Set(itInputPtr.Get());
      }
    /*********************************************************************************/

    InputImagePointerType maskImage = InputImageType::New();
    maskImage->SetRegions(inputRegionWithTwiceOffset);
    maskImage->Allocate();
    maskImage->FillBuffer(0);

    ImageRegionIteratorType itMask(maskImage, inputRegion);
    for (itMask.GoToBegin(); !itMask.IsAtEnd(); ++itMask)
      {
      itMask.Set(1);
      }

    // Compute the co-occurence matrix
    coOccurenceMatrixGenerator->SetInput(localInputImage);
    coOccurenceMatrixGenerator->SetMaskImage(maskImage);
    coOccurenceMatrixGenerator->SetInsidePixelValue(1);
    coOccurenceMatrixGenerator->Update();

    // Compute textures indices
    texturesCalculator->SetInput(coOccurenceMatrixGenerator->GetOutput());
    texturesCalculator->Update();

    // Fill outputs
    energyIt.Set(texturesCalculator->GetEnergy());
    entropyIt.Set(texturesCalculator->GetEntropy());
    correlationIt.Set(texturesCalculator->GetCorrelation());
    invDiffMomentIt.Set(texturesCalculator->GetInverseDifferenceMoment());
    inertiaIt.Set(texturesCalculator->GetInertia());
    clusterShadeIt.Set(texturesCalculator->GetClusterShade());
    clusterProminenceIt.Set(texturesCalculator->GetClusterProminence());
    haralickCorIt.Set(texturesCalculator->GetHaralickCorrelation());

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
