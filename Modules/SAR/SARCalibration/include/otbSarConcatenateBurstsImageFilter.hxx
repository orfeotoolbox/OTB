/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
#include "otbSarSensorModel.h"

namespace otb
{
template <class TImage>
void SarConcatenateBurstsImageFilter<TImage>::SetSLCImageMetadata(ImageMetadata sarImd)
{
  m_SLCImd = sarImd;
}

template <class TImage>
bool SarConcatenateBurstsImageFilter<TImage>::getDeburstLinesAndSamples(LinesRecordVectorType& linesRecord, LinesRecordVectorType& samplesRecord,
                                                                        unsigned int first_burstInd, bool inputWithInvalidPixels)
{
  // Try to create a SarSensorModel
  SarSensorModel sarSensorModel(m_SLCImd);

  LinesRecordVectorType lines;

  // Try to call the deburstAndConcatenate function
  bool deburstAndConcatenateOk = sarSensorModel.DeburstAndConcatenate(linesRecord, samplesRecord, m_Offset_OriginL, first_burstInd, inputWithInvalidPixels);

  if (!deburstAndConcatenateOk)
    itkExceptionMacro(<< "Could not deburst or concatenate from input bursts");

  sarSensorModel.UpdateImageMetadata(m_SLCImd);

  return true;
}

template <class TImage>
void SarConcatenateBurstsImageFilter<TImage>::GenerateOutputInformation()
{
  // First, call superclass implementation
  Superclass::GenerateOutputInformation();

  auto outputPtr = this->GetOutput();

  // Origin to (0.5, 0.5) : Metadata are already adjusted
  PointType origin;
  origin[0] = 0.5;
  origin[1] = 0.5 + m_Offset_OriginL;

  outputPtr->SetOrigin(origin);

  m_SLCImd.Add(MDNum::NumberOfLines, this->GetOutput()->GetLargestPossibleRegion().GetSize()[0]);
  m_SLCImd.Add(MDNum::NumberOfColumns, this->GetOutput()->GetLargestPossibleRegion().GetSize()[1]);

  outputPtr->SetImageMetadata(m_SLCImd);
}


} // end namespace otb

#endif
