/*
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

#ifndef otbScalarImageToAdvancedTexturesFilter_hxx
#define otbScalarImageToAdvancedTexturesFilter_hxx

#include "otbScalarImageToAdvancedTexturesFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"
#include <algorithm>

namespace otb
{
template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ScalarImageToAdvancedTexturesFilter()
: m_Radius()
, m_Offset()
, m_NeighborhoodRadius()
, m_NumberOfBinsPerAxis(8)
, m_InputImageMinimum(0)
, m_InputImageMaximum(255)
, m_SubsampleFactor()
, m_SubsampleOffset()
{
  // There are 10 outputs corresponding to the 9 textures indices
  this->SetNumberOfRequiredOutputs(10);

  // Create the 10 outputs
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

  this->m_SubsampleFactor.Fill(1);
  this->m_SubsampleOffset.Fill(0);
}

template <class TInputImage, class TOutputImage>
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::~ScalarImageToAdvancedTexturesFilter()
{}
template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetMeanOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDissimilarityOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(2));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumAverageOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(3));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(4));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetSumEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(5));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 7)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(6));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetDifferenceVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 8)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(7));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC1Output()
{
  if (this->GetNumberOfOutputs() < 9)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(8));
}

template <class TInputImage, class TOutputImage>
typename ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::OutputImageType *
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GetIC2Output()
{
  if (this->GetNumberOfOutputs() < 10)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->GetOutput(9));
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // First, call superclass implementation
  Superclass::GenerateOutputInformation();

  // Compute output size, origin & spacing
  InputRegionType inputRegion = this->GetInput()->GetLargestPossibleRegion();
  OutputRegionType outputRegion;
  outputRegion.SetIndex(0,0);
  outputRegion.SetIndex(1,0);
  outputRegion.SetSize(0, 1 + (inputRegion.GetSize(0) - 1 - m_SubsampleOffset[0]) / m_SubsampleFactor[0]);
  outputRegion.SetSize(1, 1 + (inputRegion.GetSize(1) - 1 - m_SubsampleOffset[1]) / m_SubsampleFactor[1]);

  typename OutputImageType::SpacingType outSpacing = this->GetInput()->GetSignedSpacing();
  outSpacing[0] *= m_SubsampleFactor[0];
  outSpacing[1] *= m_SubsampleFactor[1];

  typename OutputImageType::PointType outOrigin;
  this->GetInput()->TransformIndexToPhysicalPoint(inputRegion.GetIndex()+m_SubsampleOffset,outOrigin);

  for (unsigned int i=0 ; i<this->GetNumberOfOutputs() ; i++)
    {
    OutputImagePointerType outputPtr = this->GetOutput(i);
    outputPtr->SetLargestPossibleRegion(outputRegion);
    outputPtr->SetOrigin(outOrigin);
    outputPtr->SetSignedSpacing(outSpacing);
    }
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
  InputRegionType inputLargest = inputPtr->GetLargestPossibleRegion();

  typename OutputRegionType::IndexType outputIndex = outputRequestedRegion.GetIndex();
  typename OutputRegionType::SizeType  outputSize   = outputRequestedRegion.GetSize();
  typename InputRegionType::IndexType  inputIndex;
  typename InputRegionType::SizeType   inputSize;

  // Convert index and size to full grid
  outputIndex[0] = outputIndex[0] * m_SubsampleFactor[0] + m_SubsampleOffset[0] + inputLargest.GetIndex(0);
  outputIndex[1] = outputIndex[1] * m_SubsampleFactor[1] + m_SubsampleOffset[1] + inputLargest.GetIndex(1);
  outputSize[0] = 1 + (outputSize[0] - 1) * m_SubsampleFactor[0];
  outputSize[1] = 1 + (outputSize[1] - 1) * m_SubsampleFactor[1];

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
::BeforeThreadedGenerateData()
{
  unsigned int minRadius = 0;
  for ( unsigned int i = 0; i < m_Offset.GetOffsetDimension(); i++ )
    {
    unsigned int distance = std::abs(m_Offset[i]);
    if ( distance > minRadius )
      {
      minRadius = distance;
      }
    }
  m_NeighborhoodRadius.Fill(minRadius);
}

template <class TInputImage, class TOutputImage>
void
ScalarImageToAdvancedTexturesFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr              =      const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType meanPtr               =      this->GetMeanOutput();
  OutputImagePointerType variancePtr           =      this->GetVarianceOutput();
  OutputImagePointerType dissimilarityPtr      =      this->GetDissimilarityOutput();
  OutputImagePointerType sumAveragePtr         =      this->GetSumAverageOutput();
  OutputImagePointerType sumVariancePtr        =      this->GetSumVarianceOutput();
  OutputImagePointerType sumEntropytPtr        =      this->GetSumEntropyOutput();
  OutputImagePointerType differenceEntropyPtr  =      this->GetDifferenceEntropyOutput();
  OutputImagePointerType differenceVariancePtr =      this->GetDifferenceVarianceOutput();
  OutputImagePointerType ic1Ptr                =      this->GetIC1Output();
  OutputImagePointerType ic2Ptr                =      this->GetIC2Output();

  // Build output iterators
  itk::ImageRegionIteratorWithIndex<OutputImageType> varianceIt(variancePtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          meanIt(meanPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>          dissimilarityIt(dissimilarityPtr, outputRegionForThread);
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
  dissimilarityIt.GoToBegin();
  sumAverageIt.GoToBegin();
  sumVarianceIt.GoToBegin();
  sumEntropytIt.GoToBegin();
  differenceEntropyIt.GoToBegin();
  differenceVarianceIt.GoToBegin();
  ic1It.GoToBegin();
  ic2It.GoToBegin();

  const double log2 = std::log(2.0);
  const unsigned int histSize = m_NumberOfBinsPerAxis;
  const long unsigned int twiceHistSize = 2 * m_NumberOfBinsPerAxis;

  InputRegionType inputLargest = inputPtr->GetLargestPossibleRegion();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while (!varianceIt.IsAtEnd()
         && !meanIt.IsAtEnd()
         && !dissimilarityIt.IsAtEnd()
         && !sumAverageIt.IsAtEnd()
         && !sumVarianceIt.IsAtEnd()
         && !sumEntropytIt.IsAtEnd()
         && !differenceEntropyIt.IsAtEnd()
         && !differenceVarianceIt.IsAtEnd()
         && !ic1It.IsAtEnd()
         && !ic2It.IsAtEnd())
    {
    // Compute the region on which co-occurence will be estimated
    typename InputRegionType::IndexType inputIndex;
    typename InputRegionType::SizeType inputSize;

    // Convert index to full grid
    typename OutputImageType::IndexType outIndex;

    // First, create an window for neighborhood iterator based on m_Radius
    // For example, if xradius and yradius is 2. window size is 5x5 (2 * radius + 1).
    for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
      {
      outIndex[dim] = varianceIt.GetIndex()[dim] * m_SubsampleFactor[dim]
        + m_SubsampleOffset[dim] + inputLargest.GetIndex(dim);
      inputIndex[dim] = outIndex[dim] - m_Radius[dim];
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
      continue; // don't put a pixel in the co-occurrence list if the value is
               // out of bounds
      }
    GLCIList->AddPixelPair(centerPixelIntensity, pixelIntensity);
    }

    PixelValueType m_Mean                    = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_Variance                = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_Dissimilarity           = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_SumAverage              = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_SumEntropy              = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_SumVariance             = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_DifferenceEntropy       = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_DifferenceVariance      = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_IC1                     = itk::NumericTraits< PixelValueType >::Zero;
    PixelValueType m_IC2                     = itk::NumericTraits< PixelValueType >::Zero;

    double Entropy = 0;

    typedef itk::Array<double> DoubleArrayType;
    DoubleArrayType hx(histSize);
    DoubleArrayType hy(histSize);
    DoubleArrayType pdxy(twiceHistSize);

    for(long unsigned int i = 0; i < histSize; i++)
      {
      hx[i] = 0.0;
      hy[i] = 0.0;
      pdxy[i] = 0.0;
      }
    for(long unsigned int i = histSize; i < twiceHistSize; i++)
      {
      pdxy[i] = 0.0;
      }

    /*   hx.Fill(0.0);    hy.Fill(0.0);    pdxy.Fill(0.0);   */
    double hxy1 = 0;

    //get co-occurrence vector and totalfrequency
    VectorType glcVector = GLCIList->GetVector();
    double totalFrequency = static_cast<double> (GLCIList->GetTotalFrequency());

    VectorConstIteratorType constVectorIt;
    //Normalize the GreyLevelCooccurrenceListType
    //Compute Mean, Entropy (f12), hx, hy, pdxy
    constVectorIt = glcVector.begin();
    while( constVectorIt != glcVector.end())
      {
      CooccurrenceIndexType index = (*constVectorIt).first;
      double frequency = (*constVectorIt).second / totalFrequency;
      m_Mean += static_cast<double>(index[0]) * frequency;
      Entropy -= (frequency > 0.0001) ? frequency * std::log(frequency) / log2 : 0.;
      unsigned int i = index[1];
      unsigned int j = index[0];
      hx[j] += frequency;
      hy[i] += frequency;

      if( i+j > histSize-1)
        {
        pdxy[i+j] += frequency;
        }
      if( i <= j )
        {
        pdxy[j-i] += frequency;
        }
      ++constVectorIt;
      }

    //second pass over normalized co-occurrence list to find variance and pipj.
    //pipj is needed to calculate f11
    constVectorIt = glcVector.begin();
    while( constVectorIt != glcVector.end())
      {
      double frequency = (*constVectorIt).second / totalFrequency;
      CooccurrenceIndexType index = (*constVectorIt).first;
      unsigned int i = index[1];
      unsigned int j = index[0];
      double index0 = static_cast<double>(index[0]);
      m_Variance += ((index0 - m_Mean) * (index0 - m_Mean)) * frequency;
      double pipj = hx[j] * hy[i];
      hxy1 -= (pipj > 0.0001) ? frequency * std::log(pipj) : 0.;
      ++constVectorIt;
      }

    //iterate histSize to compute sumEntropy
    double PSSquareCumul = 0;
    for(long unsigned int k = histSize; k < twiceHistSize; k++)
      {
      m_SumAverage += k * pdxy[k];
      m_SumEntropy -= (pdxy[k] > 0.0001) ? pdxy[k] * std::log(pdxy[k]) / log2 : 0;
      PSSquareCumul += k * k * pdxy[k];
      }
    m_SumVariance = PSSquareCumul - m_SumAverage * m_SumAverage;

    double PDSquareCumul = 0;
    double PDCumul = 0;
    double hxCumul = 0;
    double hyCumul = 0;

    for (long unsigned int i = 0; i < histSize; ++i)
      {
      double pdTmp = pdxy[i];
      PDCumul += i * pdTmp;
      m_DifferenceEntropy -= (pdTmp > 0.0001) ? pdTmp * std::log(pdTmp) / log2 : 0;
      PDSquareCumul += i * i * pdTmp;

      //comput hxCumul and hyCumul
      double marginalfreq = hx[i];
      hxCumul += (marginalfreq > 0.0001) ? std::log (marginalfreq) * marginalfreq : 0;

      marginalfreq = hy[i];
      hyCumul += (marginalfreq > 0.0001) ? std::log (marginalfreq) * marginalfreq : 0;
      }
    m_DifferenceVariance = PDSquareCumul - PDCumul * PDCumul;

    /* pipj computed below is totally different from earlier one which was used
     * to compute hxy1. */
    double hxy2 = 0;
    for(unsigned int i = 0; i < histSize; ++i)
      {
      for(unsigned int j = 0; j < histSize; ++j)
        {
        double pipj = hx[j] * hy[i];
        hxy2 -= (pipj > 0.0001) ? pipj * std::log(pipj) : 0.;
        double frequency = GLCIList->GetFrequency(i,j, glcVector) / totalFrequency;
        m_Dissimilarity+= ( static_cast<double>(j) - static_cast<double>(i) ) * (frequency * frequency);
        }
      }

    //Information measures of correlation 1 & 2
    m_IC1 = (std::abs(std::max (hxCumul, hyCumul)) > 0.0001) ? (Entropy - hxy1) / (std::max (hxCumul, hyCumul)) : 0;
    m_IC2 = 1 - std::exp (-2. * std::abs (hxy2 - Entropy));
    m_IC2 = (m_IC2 >= 0) ? std::sqrt (m_IC2) : 0;

    // Fill outputs
    meanIt.Set(m_Mean);
    varianceIt.Set(m_Variance);
    dissimilarityIt.Set(m_Dissimilarity);
    sumAverageIt.Set(m_SumAverage);
    sumVarianceIt.Set(m_SumVariance);
    sumEntropytIt.Set(m_SumEntropy);
    differenceEntropyIt.Set(m_DifferenceEntropy);
    differenceVarianceIt.Set(m_DifferenceVariance);
    ic1It.Set(m_IC1);
    ic2It.Set(m_IC2);

    // Update progress
    progress.CompletedPixel();

    // Increment iterators
    ++varianceIt;
    ++meanIt;
    ++dissimilarityIt;
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
