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
#ifndef __otbScalarImageToAdvancedTexturesFilter_txx
#define __otbScalarImageToAdvancedTexturesFilter_txx

#include "otbScalarImageToAdvancedTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToAdvancedTexturesFilter() : m_Radius(),
  m_Offset(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(256)
{
  // There are 9 outputs corresponding to the 8 textures indices
  this->SetNumberOfRequiredOutputs(9);

  // Create the 9 outputs
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());
  this->SetNthOutput(4, OutputImageType::New());
  this->SetNthOutput(5, OutputImageType::New());
  this->SetNthOutput(6, OutputImageType::New());
  this->SetNthOutput(7, OutputImageType::New());
  this->SetNthOutput(8, OutputImageType::New());
}

template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::~ScalarImageToAdvancedTexturesFilter()
{}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumAverageOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(2));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(3));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(4));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(5));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC1Output()
{
  if (this->GetNumberOfOutputs() < 7)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(6));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC2Output()
{
  if (this->GetNumberOfOutputs() < 8)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(7));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetMeanOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->GetOutput(8));
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
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
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr             =      const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType meanPtr              =      this->GetMeanOutput();
  OutputImagePointerType variancePtr          =      this->GetVarianceOutput();
  OutputImagePointerType sumAveragePtr        =      this->GetSumAverageOutput();
  OutputImagePointerType sumVariancePtr       =      this->GetSumVarianceOutput();
  OutputImagePointerType sumEntropytPtr       =      this->GetSumEntropyOutput();
  OutputImagePointerType differenceEntropyPtr =      this->GetDifferenceEntropyOutput();
  OutputImagePointerType differenceVariancePtr =      this->GetDifferenceVarianceOutput();
  OutputImagePointerType ic1Ptr               =      this->GetIC1Output();
  OutputImagePointerType ic2Ptr               =      this->GetIC2Output();

  // Build output iterators
  itk::ImageRegionIteratorWithIndex<OutputImageType> varianceIt(variancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          meanIt(meanPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumAverageIt(sumAveragePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumVarianceIt(sumVariancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          sumEntropytIt(sumEntropytPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          differenceEntropyIt(differenceEntropyPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          differenceVarianceIt(differenceVariancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          ic1It(ic1Ptr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          ic2It(ic2Ptr, outputRegionForThread);

  // Go to begin
  varianceIt.GoToBegin();
  meanIt.GoToBegin();
  sumAverageIt.GoToBegin();
  sumVarianceIt.GoToBegin();
  sumEntropytIt.GoToBegin();
  differenceEntropyIt.GoToBegin();
  differenceVarianceIt.GoToBegin();
  ic1It.GoToBegin();
  ic2It.GoToBegin();


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
  while (!varianceIt.IsAtEnd()
         && !meanIt.IsAtEnd()
         && !sumAverageIt.IsAtEnd()
         && !sumVarianceIt.IsAtEnd()
         && !sumEntropytIt.IsAtEnd()
         && !differenceEntropyIt.IsAtEnd()
         && !differenceVarianceIt.IsAtEnd()
         && !ic1It.IsAtEnd()
         && !ic2It.IsAtEnd())
    {
    // Compute the region on which co-occurence will be estimated
    typename InputRegionType::IndexType inputIndex, inputIndexWithTwiceOffset;
    typename InputRegionType::SizeType inputSize, inputSizeWithTwiceOffset;

    // First, apply offset
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      inputIndex[dim] = std::min(
                                static_cast<int>(varianceIt.GetIndex()[dim] - m_Radius[dim]),
                                static_cast<int>(varianceIt.GetIndex()[dim] - m_Radius[dim] + m_Offset[dim])
                                );
      inputSize[dim] = 2 * m_Radius[dim] + 1 + std::abs(m_Offset[dim]);

      inputIndexWithTwiceOffset[dim] = static_cast<int>(varianceIt.GetIndex()[dim] - m_Radius[dim] - std::abs(m_Offset[dim]));
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
    texturesCalculator->SetHistogram(const_cast<HistogramType*>(coOccurenceMatrixGenerator->GetOutput())); //FIXME const correctness
    texturesCalculator->Compute();

    // Fill outputs
    varianceIt.Set(texturesCalculator->GetVariance());
    meanIt.Set(texturesCalculator->GetMean());
    sumAverageIt.Set(texturesCalculator->GetSumAverage());
    sumVarianceIt.Set(texturesCalculator->GetSumVariance());
    sumEntropytIt.Set(texturesCalculator->GetSumEntropy());
    differenceEntropyIt.Set(texturesCalculator->GetDifferenceEntropy());
    differenceVarianceIt.Set(texturesCalculator->GetDifferenceVariance());
    ic1It.Set(texturesCalculator->GetIC1());
    ic2It.Set(texturesCalculator->GetIC2());

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    ++varianceIt;
    ++meanIt;
    ++sumAverageIt;
    ++sumVarianceIt;
    ++sumEntropytIt;
    ++differenceEntropyIt;
    ++differenceVarianceIt;
    ++ic1It;
    ++ic2It;
    }

}

} // End namespace otb

#endif
