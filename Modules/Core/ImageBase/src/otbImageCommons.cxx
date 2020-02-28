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


std::string ImageCommons::GetProjectionRef(void) const
{
  return m_Imd.ProjectionRef;
}


void ImageCommons::SetProjectionRef(const std::string& proj)
{
  m_Imd.ProjectionRef = proj;
}


std::string ImageCommons::GetGCPProjection(void) const
{
  return m_Imd.GCPProjection;
}


unsigned int ImageCommons::GetGCPCount(void) const
{
  return m_Imd.GCPs.size();
}


OTB_GCP& ImageCommons::GetGCPs(unsigned int GCPnum)
{
  return m_Imd.GCPs[GCPnum];
}


const OTB_GCP& ImageCommons::GetGCPs(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum];
}


std::string ImageCommons::GetGCPId(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_Id;
}


std::string ImageCommons::GetGCPInfo(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_Info;
}


double ImageCommons::GetGCPRow(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_GCPRow;
}


double ImageCommons::GetGCPCol(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_GCPCol;
}


double ImageCommons::GetGCPX(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_GCPX;
}


double ImageCommons::GetGCPY(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_GCPY;
}


double ImageCommons::GetGCPZ(unsigned int GCPnum) const
{
  return m_Imd.GCPs[GCPnum].m_GCPZ;
}


ImageMetadataInterfaceBase::VectorType ImageCommons::GetGeoTransform(void) const
{
  //~ return {m_Imd.GeoTransform[0], m_Imd.GeoTransform[1],
          //~ m_Imd.GeoTransform[2], m_Imd.GeoTransform[3],
          //~ m_Imd.GeoTransform[4], m_Imd.GeoTransform[5]};
  return {};
}


ImageMetadataInterfaceBase::VectorType ImageCommons::GetUpperLeftCorner(void) const
{
  //~ return {m_Imd.ULX, m_Imd.ULY};
  return {};
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
  return !m_Imd.SensorGeometry.empty();
}

} // end namespace otb
