/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarDeburstImageFilter_hxx
#define otbSarDeburstImageFilter_hxx

#include "otbSarDeburstImageFilter.h"

#include "otbSarSensorModelAdapter.h"
#include "otbImageKeywordlist.h"
#include "itkImageScanlineIterator.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{
// Constructor
template <class TImage>
SarDeburstImageFilter<TImage>::SarDeburstImageFilter() : m_LinesRecord(), m_SamplesRecord(), m_OnlyValidSample(false)
{
}

// Needs to be re-implemented since size of output is modified
template <class TImage>
void SarDeburstImageFilter<TImage>::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve the input image pointer
  const ImageType* inputPtr  = this->GetInput();
  ImageType*       outputPtr = this->GetOutput();

  // Check that azimuth spacing has not been modified
  if (std::abs(inputPtr->GetSignedSpacing()[1] - 1.) >= std::numeric_limits<double>::epsilon())
    itkExceptionMacro("Can not perform deburst if input image azimuth spacing is not 1.");

  // Check that the azimuth sampling grid has not been modified
  if (std::abs(inputPtr->GetOrigin()[1] - static_cast<long>(inputPtr->GetOrigin()[1]) - 0.5) >= std::numeric_limits<double>::epsilon())
    itkExceptionMacro("Can not perform deburst if input image azimuth origin is not N.5");

  // Retrieve input image keywordlist
  ImageKeywordlist inputKwl = inputPtr->GetImageKeywordlist();

  // Try to create a SarSensorModelAdapter
  SarSensorModelAdapter::Pointer sarSensorModel = SarSensorModelAdapter::New();
  bool                           loadOk         = sarSensorModel->LoadState(inputKwl);

  if (!loadOk || !sarSensorModel->IsValidSensorModel())
    itkExceptionMacro(<< "Input image does not contain a valid SAR sensor model.");

  // Try to call the deburst function
  bool deburstOk = sarSensorModel->Deburst(m_LinesRecord, m_SamplesRecord, m_OnlyValidSample);

  if (!deburstOk || m_LinesRecord.empty())
    itkExceptionMacro(<< "Could not deburst SAR sensor model from input image");

  // Compute the actual lines to remove
  typename ImageType::RegionType largestPossibleRegion = this->GetInput()->GetLargestPossibleRegion();
  typename ImageType::PointType  origin                = this->GetInput()->GetOrigin();

  // Export the new keywordlist
  ImageKeywordlist newKwl;

  bool saveOk = sarSensorModel->SaveState(newKwl);

  if (!saveOk)
    itkExceptionMacro(<< "Could not export deburst SAR sensor model to keyword list");

  // Now, filter the LinesRecord so as to account for possible
  // extracts on input image
  long firstInputLine = static_cast<long>(origin[1] - 0.5);

  // We know that spacing[1]=1.
  long lastInputLine = static_cast<long>(origin[1] - 0.5 + largestPossibleRegion.GetSize()[1] - 1);

  // Move origin
  unsigned long outputOriginLine = 0;
  SarSensorModelAdapter::ImageLineToDeburstLine(m_LinesRecord, firstInputLine, outputOriginLine);

  long          originOffset_samples = static_cast<long>(this->GetInput()->GetOrigin()[0] - 0.5);
  unsigned long outputOriginSample   = 0;
  if (m_OnlyValidSample)
  {
    if (static_cast<int>(m_SamplesRecord.first) > originOffset_samples)
    {
      outputOriginSample = 0;
    }
    else
    {
      outputOriginSample = originOffset_samples - static_cast<int>(m_SamplesRecord.first);
    }
    origin[0] = 0.5 + outputOriginSample;
  }

  origin[1] = 0.5 + outputOriginLine;
  outputPtr->SetOrigin(origin);

  // Update line records to accommodate actual input image region
  LinesRecordVectorType filteredRecords;

  for (LinesRecordVectorType::const_iterator it = m_LinesRecord.begin(); it != m_LinesRecord.end(); ++it)
  {
    // If record is inside input image region
    if ((long)it->first <= lastInputLine && (long)it->second >= firstInputLine)
    {
      RecordType filteredRecord = *it;
      filteredRecord.first      = std::max(static_cast<long>(filteredRecord.first), firstInputLine);
      filteredRecord.second     = std::min(static_cast<long>(filteredRecord.second), lastInputLine);
      filteredRecords.push_back(filteredRecord);
    }
  }

  // Use filtered records
  // m_LinesRecord.swap(filteredRecords);

  // TODO: Ensure that records are sorted ?


  // Compute deburst azimuth size
  typename ImageType::SizeType deburstSize = largestPossibleRegion.GetSize();
  deburstSize[1]                           = 0;


  for (LinesRecordVectorType::const_iterator it = filteredRecords.begin(); it != filteredRecords.end(); ++it)
  {
    deburstSize[1] += it->second - it->first + 1;
  }

  if (m_OnlyValidSample)
  {
    long minEnd = static_cast<long>(
        std::min(static_cast<long>(m_SamplesRecord.second), static_cast<long>(largestPossibleRegion.GetSize()[0] + originOffset_samples - 1)));
    long maxStart  = static_cast<long>(std::max(static_cast<long>(m_SamplesRecord.first), originOffset_samples));
    deburstSize[0] = minEnd - maxStart + 1;
  }

  // Set largest possible region
  typename ImageType::RegionType outputLargestPossibleRegion = largestPossibleRegion;
  largestPossibleRegion.SetSize(deburstSize);
  outputPtr->SetLargestPossibleRegion(largestPossibleRegion);

  newKwl.AddKey("support_data.number_samples", std::to_string(deburstSize[0]));
  newKwl.AddKey("support_data.number_lines", std::to_string(deburstSize[1]));

  newKwl.AddKey("number_samples", std::to_string(deburstSize[0]));
  newKwl.AddKey("number_lines", std::to_string(deburstSize[1]));

  // Set new keyword list to output image
  outputPtr->SetImageKeywordList(newKwl);
}

template <class TImage>
typename SarDeburstImageFilter<TImage>::RegionType SarDeburstImageFilter<TImage>::OutputRegionToInputRegion(const RegionType& outputRegion) const
{
  PointType outputUperLeft, outputLowerLeft;

  typename RegionType::IndexType outputUpperLeftIndex = outputRegion.GetIndex();
  typename RegionType::IndexType outputLowerLeftIndex = outputUpperLeftIndex;
  outputLowerLeftIndex[1] += outputRegion.GetSize()[1] - 1;

  this->GetOutput()->TransformIndexToPhysicalPoint(outputUpperLeftIndex, outputUperLeft);
  this->GetOutput()->TransformIndexToPhysicalPoint(outputLowerLeftIndex, outputLowerLeft);

  // TODO: Watch for <0
  unsigned long upperLeftLine = static_cast<unsigned long>(outputUperLeft[1] - 0.5);
  unsigned long lowerLeftLine = static_cast<unsigned long>(outputLowerLeft[1] - 0.5);

  unsigned long inputUpperLeftLine, inputLowerLeftLine;

  SarSensorModelAdapter::DeburstLineToImageLine(m_LinesRecord, upperLeftLine, inputUpperLeftLine);
  SarSensorModelAdapter::DeburstLineToImageLine(m_LinesRecord, lowerLeftLine, inputLowerLeftLine);

  long originOffset         = static_cast<long>(this->GetInput()->GetOrigin()[1] - 0.5);
  long originOffset_samples = static_cast<long>(this->GetInput()->GetOrigin()[0] - 0.5);

  inputUpperLeftLine -= originOffset;
  inputLowerLeftLine -= originOffset;

  RegionType inputRegion = outputRegion;

  typename RegionType::SizeType  size  = inputRegion.GetSize();
  typename RegionType::IndexType index = inputRegion.GetIndex();

  if (m_OnlyValidSample)
  {
    if (static_cast<int>(m_SamplesRecord.first) > originOffset_samples)
    {
      index[0] += m_SamplesRecord.first - originOffset_samples;
    }
  }

  index[1] = inputUpperLeftLine;
  size[1]  = inputLowerLeftLine - inputUpperLeftLine + 1;

  inputRegion.SetIndex(index);
  inputRegion.SetSize(size);

  return inputRegion;
}


// Needs to be re-implemented since size of output is modified
template <class TImage>
void SarDeburstImageFilter<TImage>::GenerateInputRequestedRegion()
{
  RegionType outputRequestedRegion = this->GetOutput()->GetRequestedRegion();
  RegionType inputRequestedRegion  = OutputRegionToInputRegion(outputRequestedRegion);

  ImageType* inputPtr = const_cast<ImageType*>(this->GetInput());

  inputPtr->SetRequestedRegion(inputRequestedRegion);
}

// Actual processing
template <class TImage>
void SarDeburstImageFilter<TImage>::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  if (m_OnlyValidSample)
  {
    this->ThreadedGenerateDataWithOnlyValidSamples(outputRegionForThread, 0);
  }
  else
  {
    this->ThreadedGenerateDataWithAllSamples(outputRegionForThread, 0);
  }
}

// Actual processing with all samples
template <class TImage>
void SarDeburstImageFilter<TImage>::ThreadedGenerateDataWithAllSamples(const RegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  // Compute corresponding input region
  RegionType inputRegionForThread = OutputRegionToInputRegion(outputRegionForThread);

  itk::ImageScanlineConstIterator<ImageType> inputIt(this->GetInput(), inputRegionForThread);
  itk::ImageScanlineIterator<ImageType>      outputIt(this->GetOutput(), outputRegionForThread);

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
  {
    typename ImageType::IndexType currentInputIndex = inputIt.GetIndex();
    PointType                     currentInputPoint;
    this->GetInput()->TransformIndexToPhysicalPoint(currentInputIndex, currentInputPoint);

    bool lineToKeep = false;

    for (auto const& record : m_LinesRecord)
    {
      if (currentInputPoint[1] - 0.5 >= record.first && currentInputPoint[1] - 0.5 <= record.second)
      {
        lineToKeep = true;
        break;
      }
    }
    if (lineToKeep)
    {
      for (inputIt.GoToBeginOfLine(), outputIt.GoToBeginOfLine(); !inputIt.IsAtEndOfLine() && !outputIt.IsAtEndOfLine(); ++inputIt, ++outputIt)
      {
        outputIt.Set(inputIt.Get());
      }
      outputIt.NextLine();
    }


    inputIt.NextLine();
  }
}

// Actual processing with only valid samples
template <class TImage>
void SarDeburstImageFilter<TImage>::ThreadedGenerateDataWithOnlyValidSamples(const RegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  // Compute corresponding input region
  RegionType inputRegionForThread = OutputRegionToInputRegion(outputRegionForThread);

  itk::ImageRegionConstIterator<ImageType> inputIt(this->GetInput(), inputRegionForThread);
  itk::ImageRegionIterator<ImageType>      outputIt(this->GetOutput(), outputRegionForThread);

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
  {
    typename ImageType::IndexType currentInputIndex = inputIt.GetIndex();
    PointType                     currentInputPoint;
    this->GetInput()->TransformIndexToPhysicalPoint(currentInputIndex, currentInputPoint);

    bool lineToKeep   = false;
    bool sampleToKeep = false;

    for (auto const& record : m_LinesRecord)
    {
      if (currentInputPoint[1] - 0.5 >= record.first && currentInputPoint[1] - 0.5 <= record.second)
      {
        lineToKeep = true;
        break;
      }
    }

    if (currentInputPoint[0] - 0.5 >= static_cast<int>(m_SamplesRecord.first) && currentInputPoint[1] - 0.5 <= static_cast<int>(m_SamplesRecord.second))
    {
      sampleToKeep = true;
    }

    if (lineToKeep && sampleToKeep)
    {
      outputIt.Set(inputIt.Get());

      ++outputIt;
    }


    ++inputIt;
  }
}


} // End namespace otb

#endif
