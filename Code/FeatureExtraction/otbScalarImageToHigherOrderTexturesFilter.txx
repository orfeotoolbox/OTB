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
::ScalarImageToHigherOrderTexturesFilter() : m_Radius(),
  m_Offset(),
  m_NumberOfBinsPerAxis(8),
  m_InputImageMinimum(0),
  m_InputImageMaximum(255)
{
  // There are 11 outputs corresponding to the 8 textures indices
  this->SetNumberOfOutputs(11);

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
  this->SetNthOutput(11, OutputImageType::New());
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
ScalarImageToHigherOrderTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, int threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr  = const_cast<InputImageType *>(this->GetInput());

  typedef typename itk::ImageRegionIterator<OutputImageType> IteratorType;
  std::vector<IteratorType> outputImagesIterators;

  for (unsigned int i = 0; i < 11; ++i)
    {
    outputImagesIterators.push_back( IteratorType(this->GetOutput(i), outputRegionForThread) );
    outputImagesIterators[i].GoToBegin();
    }

  // Build the run length matrix generator
  RunLengthMatrixGeneratorPointerType runlengthMatrixGenerator = RunLengthMatrixGeneratorType::New();
  runlengthMatrixGenerator->SetInput(inputPtr);
  runlengthMatrixGenerator->SetOffset(m_Offset);
  runlengthMatrixGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
  runlengthMatrixGenerator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);

  // Compute the max possible run length (in physical unit)
  typename InputImageType::PointType  topLeftPoint;
  typename InputImageType::PointType  bottomRightPoint;
  inputPtr->TransformIndexToPhysicalPoint( outputImagesIterators[0].GetIndex() - m_Radius, topLeftPoint );
  inputPtr->TransformIndexToPhysicalPoint( outputImagesIterators[0].GetIndex() + m_Radius, bottomRightPoint );
  runlengthMatrixGenerator->SetDistanceValueMinMax(0, topLeftPoint.EuclideanDistanceTo(bottomRightPoint));

  // Build the texture calculator
  TextureCoefficientsCalculatorPointerType texturesCalculator = TextureCoefficientsCalculatorType::New();

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

    // Compute the run-length matrix
    runlengthMatrixGenerator->SetRegion(inputRegion);
    //runlengthMatrixGenerator->SetNormalize(true);
    runlengthMatrixGenerator->Compute();

    // Compute textures indices
    texturesCalculator->SetHistogram(runlengthMatrixGenerator->GetOutput());
    texturesCalculator->Compute();

    // Fill outputs
    outputImagesIterators[0].Set(texturesCalculator->GetShortRunEmphasis());
    outputImagesIterators[1].Set(texturesCalculator->GetLongRunEmphasis());
    outputImagesIterators[2].Set(texturesCalculator->GetGreyLevelNonuniformity());
    outputImagesIterators[3].Set(texturesCalculator->GetRunLengthNonuniformity());
    outputImagesIterators[4].Set(static_cast<double>(texturesCalculator->GetTotalNumberOfRuns()) / inputRegion.GetNumberOfPixels());
    outputImagesIterators[5].Set(texturesCalculator->GetLowGreyLevelRunEmphasis());
    outputImagesIterators[6].Set(texturesCalculator->GetHighGreyLevelRunEmphasis());
    outputImagesIterators[7].Set(texturesCalculator->GetShortRunLowGreyLevelEmphasis());
    outputImagesIterators[8].Set(texturesCalculator->GetShortRunHighGreyLevelEmphasis());
    outputImagesIterators[9].Set(texturesCalculator->GetLongRunLowGreyLevelEmphasis());
    outputImagesIterators[10].Set(texturesCalculator->GetLongRunHighGreyLevelEmphasis());

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    for (unsigned int i = 0; i < 11; ++i)
      {
      ++outputImagesIterators[i];
      }
    }

}

} // End namespace otb

#endif
