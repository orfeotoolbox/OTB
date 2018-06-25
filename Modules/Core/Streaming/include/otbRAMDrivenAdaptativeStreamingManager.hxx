/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRAMDrivenAdaptativeStreamingManager_hxx
#define otbRAMDrivenAdaptativeStreamingManager_hxx

#include "otbRAMDrivenAdaptativeStreamingManager.h"
#include "otbMacro.h"
#include "otbImageRegionAdaptativeSplitter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{

template <class TImage>
RAMDrivenAdaptativeStreamingManager<TImage>::RAMDrivenAdaptativeStreamingManager()
  : m_AvailableRAMInMB(0),
    m_Bias(1.0)
{
}

template <class TImage>
RAMDrivenAdaptativeStreamingManager<TImage>::~RAMDrivenAdaptativeStreamingManager()
{
}

template <class TImage>
void
RAMDrivenAdaptativeStreamingManager<TImage>::PrepareStreaming( itk::DataObject * input, const RegionType &region )
{
  unsigned long nbDivisions =
      this->EstimateOptimalNumberOfDivisions(input, region, m_AvailableRAMInMB, m_Bias);

  typename otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::SizeType tileHint;

  unsigned int tileHintX(0), tileHintY(0);

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintX,
                                    tileHintX);

  itk::ExposeMetaData<unsigned int>(input->GetMetaDataDictionary(),
                                    MetaDataKey::TileHintY,
                                    tileHintY);

  tileHint[0] = tileHintX;
  tileHint[1] = tileHintY;

  typename otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::Pointer splitter =
      otb::ImageRegionAdaptativeSplitter<itkGetStaticConstMacro(ImageDimension)>::New();

  splitter->SetTileHint(tileHint);

  this->m_Splitter = splitter;

  this->m_ComputedNumberOfSplits = this->m_Splitter->GetNumberOfSplits(region, nbDivisions);

  this->m_Region = region;
}

} // End namespace otb

#endif
