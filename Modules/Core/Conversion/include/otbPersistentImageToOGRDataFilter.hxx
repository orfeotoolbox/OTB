/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentImageToOGRDataFilter_hxx
#define otbPersistentImageToOGRDataFilter_hxx

#include "otbPersistentImageToOGRDataFilter.h"
#include "otbStopwatch.h"
#include <stdio.h>
#include "otbMacro.h"
#include "otbOGRHelpers.h"

namespace otb
{

template <class TImage>
PersistentImageToOGRDataFilter<TImage>::PersistentImageToOGRDataFilter()
  : m_FieldName("DN"), m_LayerName("Layer"), m_GeometryType(wkbMultiPolygon), m_FieldType(OFTInteger)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
  m_StreamSize.Fill(0);
}

template <class TImage>
PersistentImageToOGRDataFilter<TImage>::~PersistentImageToOGRDataFilter()
{
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::SetOGRDataSource(OGRDataSourcePointerType ogrDS)
{
  this->itk::ProcessObject::SetNthInput(1, ogrDS);
}

template <class TImage>
typename PersistentImageToOGRDataFilter<TImage>::OGRDataSourceType* PersistentImageToOGRDataFilter<TImage>::GetOGRDataSource(void)
{
  return static_cast<OGRDataSourceType*>(this->itk::ProcessObject::GetInput(1));
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::AddOGRLayerCreationOption(const std::string& option)
{
  m_OGRLayerCreationOptions.push_back(option);
  this->Modified();
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::ClearOGRLayerCreationOptions()
{
  m_OGRLayerCreationOptions.clear();
  this->Modified();
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::SetOGRLayerCreationOptions(const std::vector<std::string>& options)
{
  m_OGRLayerCreationOptions = options;
  this->Modified();
}

template <class TImage>
const std::vector<std::string>& PersistentImageToOGRDataFilter<TImage>::GetOGRLayerCreationOptions(void)
{
  return m_OGRLayerCreationOptions;
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::Reset()
{
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::Synthetize()
{
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::Initialize()
{
  std::string          projectionRefWkt               = this->GetInput()->GetProjectionRef();
  bool                 projectionInformationAvailable = !projectionRefWkt.empty();
  OGRSpatialReference* oSRS                           = NULL;
  if (projectionInformationAvailable)
  {
    oSRS = static_cast<OGRSpatialReference*>(OSRNewSpatialReference(projectionRefWkt.c_str()));
  }

  OGRDataSourcePointerType ogrDS    = this->GetOGRDataSource();
  OGRLayerType             outLayer = ogrDS->CreateLayer(m_LayerName, oSRS, m_GeometryType, m_OGRLayerCreationOptions);
  OGRFieldDefn             field(m_FieldName.c_str(), m_FieldType);
  outLayer.CreateField(field, true);
}


template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::GenerateData()
{

  if (this->GetStreamSize()[0] == 0 && this->GetStreamSize()[1] == 0)
  {
    this->m_StreamSize = this->GetInput()->GetRequestedRegion().GetSize();
  }

  // call the processing function for this tile
  OGRDataSourcePointerType currentTileVD = this->ProcessTile();
  OGRLayerType             srcLayer      = currentTileVD->GetLayerChecked(0);

  OGRDataSourcePointerType ogrDS    = this->GetOGRDataSource();
  OGRLayerType             dstLayer = ogrDS->GetLayersCount() == 1 ? ogrDS->GetLayer(0) : ogrDS->GetLayer(m_LayerName);


  // Copy features contained in the memory layer (srcLayer) in the output layer
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();

  OGRErr err = dstLayer.ogr().StartTransaction();
  if (err != OGRERR_NONE)
  {
    itkExceptionMacro(<< "Unable to start transaction for OGR layer " << dstLayer.ogr().GetName() << ".");
  }

  OGRLayerType::const_iterator featIt = srcLayer.begin();
  for (; featIt != srcLayer.end(); ++featIt)
  {
    OGRFeatureType dstFeature(dstLayer.GetLayerDefn());
    dstFeature.SetFrom(*featIt, TRUE);
    dstLayer.CreateFeature(dstFeature);
  }

  err = dstLayer.ogr().CommitTransaction();

  if (err != OGRERR_NONE)
  {
    itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << dstLayer.ogr().GetName() << ".");
  }

  chrono.Stop();
  otbMsgDebugMacro(<< "Writing OGR tile took " << chrono.GetElapsedMilliseconds() << " ms");
}

template <class TImage>
void PersistentImageToOGRDataFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
