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

#ifndef otbRAMDrivenTiledStreamingManager_hxx
#define otbRAMDrivenTiledStreamingManager_hxx

#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <class TImage>
RAMDrivenTiledStreamingManager<TImage>::RAMDrivenTiledStreamingManager() : m_AvailableRAMInMB(0), m_Bias(1.0)
{
}

template <class TImage>
RAMDrivenTiledStreamingManager<TImage>::~RAMDrivenTiledStreamingManager()
{
}

template <class TImage>
void RAMDrivenTiledStreamingManager<TImage>::PrepareStreaming(itk::DataObject* input, const RegionType& region)
{
  unsigned long nbDivisions = this->EstimateOptimalNumberOfDivisions(input, region, m_AvailableRAMInMB, m_Bias);

  this->m_Splitter               = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);
  otbMsgDevMacro(<< "Number of split : " << this->m_ComputedNumberOfSplits) this->m_Region = region;
}

} // End namespace otb

#endif
