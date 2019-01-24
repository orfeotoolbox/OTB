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

#ifndef otbTileDimensionTiledStreamingManager_hxx
#define otbTileDimensionTiledStreamingManager_hxx

#include "otbTileDimensionTiledStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <class TImage>
TileDimensionTiledStreamingManager<TImage>::TileDimensionTiledStreamingManager()
  : m_TileDimension(0)
{
}

template <class TImage>
TileDimensionTiledStreamingManager<TImage>::~TileDimensionTiledStreamingManager()
{
}

template <class TImage>
void
TileDimensionTiledStreamingManager<TImage>::PrepareStreaming( itk::DataObject * /*input*/, const RegionType &region )
{
  if (m_TileDimension < 16)
    {
    itkWarningMacro(<< "TileDimension inferior to 16 : using 16 as tile dimension")
    m_TileDimension = 16;
    }

  // Calculate number of split
  this->m_Splitter = otb::ImageRegionSquareTileSplitter<itkGetStaticConstMacro(ImageDimension)>::New();
  unsigned int nbDesiredTiles =
    itk::Math::Ceil<unsigned int>( double(region.GetNumberOfPixels() ) / (m_TileDimension * m_TileDimension) );
  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDesiredTiles);
  this->m_Region = region;
}

} // End namespace otb

#endif

