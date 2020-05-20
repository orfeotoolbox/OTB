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

#include "otbGDALDriverManagerWrapper.h"

#include <vector>

#include "otb_boost_string_header.h"
#include "otbSystem.h"

namespace otb
{

GDALDatasetWrapper::GDALDatasetWrapper() : m_Dataset(nullptr)
{
}

GDALDatasetWrapper::~GDALDatasetWrapper()
{
  if (m_Dataset)
  {
    GDALClose(m_Dataset);

    // GDALDataset * is statically cast from GDALDatasetH in
    // GDALDriverManagerWrapper::Open(). So, it should be destroyed by
    // GDALClose() (see
    // http://gdal.org/classGDALDataset.html#a4d110533d799bac7dcfad3c41d30c0e7).
    m_Dataset = nullptr;
  }
}

// GetDataSet
const GDALDataset* GDALDatasetWrapper::GetDataSet() const
{
  return m_Dataset;
}


GDALDataset* GDALDatasetWrapper::GetDataSet()
{
  return m_Dataset;
}


// IsJPEG2000
bool GDALDatasetWrapper::IsJPEG2000() const
{
  if (m_Dataset == nullptr)
  {
    return false;
  }
  std::string driverName(m_Dataset->GetDriver()->GetDescription());
  if (driverName.compare("JP2OpenJPEG") == 0 || driverName.compare("JP2KAK") == 0 || driverName.compare("JP2ECW") == 0)
  {
    return true;
  }
  return false;
}


int GDALDatasetWrapper::GetOverviewsCount() const
{
  assert(m_Dataset != NULL);
  assert(m_Dataset->GetRasterCount() > 0);
  assert(m_Dataset->GetRasterBand(1) != NULL);

  return m_Dataset->GetRasterBand(1)->GetOverviewCount();
}


unsigned int GDALDatasetWrapper::GetWidth() const
{
  assert(m_Dataset != NULL);

  return m_Dataset->GetRasterXSize();
}


unsigned int GDALDatasetWrapper::GetHeight() const
{
  assert(m_Dataset != NULL);

  return m_Dataset->GetRasterYSize();
}


size_t GDALDatasetWrapper::GetPixelBytes() const
{
  assert(m_Dataset != NULL);

  size_t size = 0;

  int count = m_Dataset->GetRasterCount();

  for (int i = 1; i <= count; ++i)
  {
    assert(m_Dataset->GetRasterBand(i) != NULL);

    switch (m_Dataset->GetRasterBand(i)->GetRasterDataType())
    {
    case GDT_Unknown:
      assert(false && "Unexpected GDALDataType GDT_Unknown value.");
      break;

    case GDT_Byte:
      size += 1;
      break;

    case GDT_UInt16:
    case GDT_Int16:
    case GDT_CInt16:
      size += 2;
      break;

    case GDT_UInt32:
    case GDT_Int32:
    case GDT_Float32:
    case GDT_CInt32:
    case GDT_CFloat32:
      size += 4;
      break;

    case GDT_Float64:
    case GDT_CFloat64:
      size += 8;
      break;

    default:
      assert(false && "Unhandled GDALDataType enum value.");
      break;
    }
  }

  return size;
}

Projection::GCPParam GDALDatasetWrapper::GetGCPParam() const
{
  Projection::GCPParam gcpParam;
  gcpParam.GCPProjection = std::string(m_Dataset->GetGCPProjection());
  for ( const GDAL_GCP *gcps = m_Dataset->GetGCPs() ; gcps != gcps + m_Dataset->GetGCPCount() ; ++gcps)
  {
    gcpParam.GCPs.push_back(OTB_GCP(std::string(gcps->pszId),
       		                        std::string(gcps->pszInfo),
  									gcps->dfGCPPixel,
   									gcps->dfGCPLine,
   									gcps->dfGCPX,
   									gcps->dfGCPY,
   									gcps->dfGCPZ));
  }
  return gcpParam;
}

void GDALDatasetWrapper::SetGCPParam(Projection::GCPParam gcpParam)
{
  int nGCPCount = gcpParam.GCPs.size();
  GDAL_GCP gcps[nGCPCount];
  GDAL_GCP *gcpIt = gcps;
  for (auto otbGcpIt = gcpParam.GCPs.cbegin() ; otbGcpIt != gcpParam.GCPs.cend() ; ++otbGcpIt, gcpIt++)
  {
    GDAL_GCP gdalGcp;
    gdalGcp.pszId      = const_cast<char*>(otbGcpIt->m_Id.c_str());
    gdalGcp.pszInfo    = const_cast<char*>(otbGcpIt->m_Info.c_str());
    gdalGcp.dfGCPPixel = otbGcpIt->m_GCPCol;
    gdalGcp.dfGCPLine  = otbGcpIt->m_GCPRow;
    gdalGcp.dfGCPX     = otbGcpIt->m_GCPX;
    gdalGcp.dfGCPY     = otbGcpIt->m_GCPY;
    gdalGcp.dfGCPZ     = otbGcpIt->m_GCPZ;
    *gcpIt = gdalGcp;
  }
  m_Dataset->SetGCPs(nGCPCount, gcps, gcpParam.GCPProjection.c_str());
}

} // end namespace otb
