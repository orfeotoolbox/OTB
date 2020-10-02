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

#ifndef otbNumberOfLinesStrippedStreamingManager_hxx
#define otbNumberOfLinesStrippedStreamingManager_hxx

#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
NumberOfLinesStrippedStreamingManager<TImage>::NumberOfLinesStrippedStreamingManager() : m_NumberOfLinesPerStrip(0)
{
}

template <class TImage>
NumberOfLinesStrippedStreamingManager<TImage>::~NumberOfLinesStrippedStreamingManager()
{
}

template <class TImage>
void NumberOfLinesStrippedStreamingManager<TImage>::PrepareStreaming(itk::DataObject* /*input*/, const RegionType& region)
{
  otbMsgDevMacro(<< "Activating STRIPPED_SET_NUMBEROFLINES streaming mode") if (m_NumberOfLinesPerStrip < 1)
  {
    itkWarningMacro(<< "DesiredNumberOfLines set to 0 : streaming disabled")
  }

  // Calculate number of splits
  unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
  unsigned long nbSplit;
  if (numberLinesOfRegion > m_NumberOfLinesPerStrip && m_NumberOfLinesPerStrip > 0)
  {
    nbSplit = static_cast<unsigned long>(std::ceil(static_cast<double>(numberLinesOfRegion) / static_cast<double>(m_NumberOfLinesPerStrip)));
  }
  else
  {
    // Don't stream
    nbSplit = 1;
  }

  this->m_Splitter               = itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbSplit);
  otbMsgDevMacro(<< "Computed number of split : " << this->m_ComputedNumberOfSplits)

      // Save the region to generate the splits later
      this->m_Region = region;
}

} // End namespace otb

#endif
