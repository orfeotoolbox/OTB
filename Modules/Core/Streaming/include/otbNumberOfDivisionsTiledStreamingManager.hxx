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

#ifndef otbNumberOfDivisionsTiledStreamingManager_hxx
#define otbNumberOfDivisionsTiledStreamingManager_hxx

#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <class TImage>
NumberOfDivisionsTiledStreamingManager<TImage>::NumberOfDivisionsTiledStreamingManager() : m_NumberOfDivisions(0)
{
}

template <class TImage>
NumberOfDivisionsTiledStreamingManager<TImage>::~NumberOfDivisionsTiledStreamingManager()
{
}

template <class TImage>
void NumberOfDivisionsTiledStreamingManager<TImage>::PrepareStreaming(itk::DataObject* /*input*/, const RegionType& region)
{
  otbMsgDevMacro(<< "Activating NumberOfDivisionsTiledStreamingManager streaming mode") if (m_NumberOfDivisions < 1)
  {
    itkWarningMacro(<< "NumberOfDivisions set to 0 : streaming disabled") m_NumberOfDivisions = 1;
  }

  this->m_Splitter               = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, m_NumberOfDivisions);
  otbMsgDevMacro(<< "Computed number of split : " << this->m_ComputedNumberOfSplits)

      // Save the region to generate the splits later
      this->m_Region = region;
}

} // End namespace otb

#endif
