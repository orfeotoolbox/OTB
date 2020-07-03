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

#include "otbImageCommons.h"

namespace otb
{

void ImageCommons::SetImageMetadata(ImageMetadata imd)
{
  m_Imd = std::move(imd);
}

void ImageCommons::SetBandImageMetadata(ImageMetadata::ImageMetadataBandsType bands)
{
  m_Imd.Bands = std::move(bands);
}

const ImageMetadata & ImageCommons::GetImageMetadata() const
{
  return m_Imd;
}

std::string ImageCommons::GetProjectionRef(void) const
{
  // TODO: support EPSG and proj as fallback
  return m_Imd.GetProjectionWKT();
}


void ImageCommons::SetProjectionRef(const std::string& proj)
{
  // TODO: support EPSG and proj as fallback
  m_Imd.Add(MDGeom::ProjectionWKT, proj);
}


std::string ImageCommons::GetGCPProjection(void) const
{
  if (m_Imd.Has(MDGeom::GCP))
  {
    return m_Imd.GetGCPParam().GCPProjection;
  }
  return "";
}


unsigned int ImageCommons::GetGCPCount(void) const
{
  if (m_Imd.Has(MDGeom::GCP))
    {
    return m_Imd.GetGCPParam().GCPs.size();
    }
  return 0;
}


const OTB_GCP& ImageCommons::GetGCPs(unsigned int GCPnum) const
{
  assert(GCPnum < GetGCPCount());
  return m_Imd.GetGCPParam().GCPs[GCPnum];
}


std::string ImageCommons::GetGCPId(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_Id;
}


std::string ImageCommons::GetGCPInfo(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_Info;
}


double ImageCommons::GetGCPRow(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_GCPRow;
}


double ImageCommons::GetGCPCol(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_GCPCol;
}


double ImageCommons::GetGCPX(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_GCPX;
}


double ImageCommons::GetGCPY(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_GCPY;
}


double ImageCommons::GetGCPZ(unsigned int GCPnum) const
{
  return GetGCPs(GCPnum).m_GCPZ;
}


ImageMetadataInterfaceBase::VectorType ImageCommons::GetUpperRightCorner(void) const
{
  //~ return {m_Imd.URX, m_Imd.URY};
  return {};
}


ImageMetadataInterfaceBase::VectorType ImageCommons::GetLowerLeftCorner(void) const
{
  //~ return {m_Imd.LLX, m_Imd.LLY};
  return {};
}


ImageMetadataInterfaceBase::VectorType ImageCommons::GetLowerRightCorner(void) const
{
  //~ return {m_Imd.LRX, m_Imd.LRY};
  return {};
}

bool ImageCommons::HasSensorGeometry() const
{
  return m_Imd.HasSensorGeometry();
}

} // end namespace otb
