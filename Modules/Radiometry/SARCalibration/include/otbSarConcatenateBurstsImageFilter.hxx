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

#ifndef otbSarConcatenateBurstsImageFilter_hxx
#define otbSarConcatenateBurstsImageFilter_hxx

#include "otbSarConcatenateBurstsImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbSarSensorModelAdapter.h"

namespace otb
{
template <class TImage>
void SarConcatenateBurstsImageFilter<TImage>::SetSLCImageKeyWorList(ImageKeywordlist sarImageKWL)
{
  m_SLCImageKWL = sarImageKWL;
}

template <class TImage>
bool SarConcatenateBurstsImageFilter<TImage>::getDeburstLinesAndSamples(LinesRecordVectorType& linesRecord, LinesRecordVectorType& samplesRecord,
                                                                        unsigned int first_burstInd, bool inputWithInvalidPixels)
{
  // Try to create a SarSensorModelAdapter
  SarSensorModelAdapter::Pointer sarSensorModel = SarSensorModelAdapter::New();

  bool loadOk = sarSensorModel->LoadState(m_SLCImageKWL);

  if (!loadOk || !sarSensorModel->IsValidSensorModel())
    itkExceptionMacro(<< "Input image does not contain a valid SAR sensor model.");

  LinesRecordVectorType lines;

  // Try to call the deburstAndConcatenate function
  bool deburstAndConcatenateOk = sarSensorModel->DeburstAndConcatenate(linesRecord, samplesRecord, m_Offset_OriginL, first_burstInd, inputWithInvalidPixels);

  if (!deburstAndConcatenateOk)
    itkExceptionMacro(<< "Could not deburst or concatenate from input bursts");

  // Export the new keywordlist
  bool saveOk = sarSensorModel->SaveState(m_DeburstSLCImageKWL);

  if (!saveOk)
    itkExceptionMacro(<< "Could not export deburst SAR sensor model to keyword list");

  return true;
}

template <class TImage>
void SarConcatenateBurstsImageFilter<TImage>::GenerateOutputInformation()
{
  // First, call superclass implementation
  Superclass::GenerateOutputInformation();

  ImageType* outputPtr = this->GetOutput();

  // Origin to (0.5, 0.5) : Metadata are already adjusted
  PointType origin;
  origin[0] = 0.5;
  origin[1] = 0.5 + m_Offset_OriginL;

  outputPtr->SetOrigin(origin);

  // Output KeywordList
  m_DeburstSLCImageKWL.AddKey("support_data.number_samples", std::to_string(this->GetOutput()->GetLargestPossibleRegion().GetSize()[0]));
  m_DeburstSLCImageKWL.AddKey("support_data.number_lines", std::to_string(this->GetOutput()->GetLargestPossibleRegion().GetSize()[1]));

  m_DeburstSLCImageKWL.AddKey("number_samples", std::to_string(this->GetOutput()->GetLargestPossibleRegion().GetSize()[0]));
  m_DeburstSLCImageKWL.AddKey("number_lines", std::to_string(this->GetOutput()->GetLargestPossibleRegion().GetSize()[1]));

  // Set new keyword list to output image
  outputPtr->SetImageKeywordList(m_DeburstSLCImageKWL);
}


} // end namepsace otb

#endif
