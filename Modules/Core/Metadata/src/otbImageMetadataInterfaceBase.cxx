/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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


#include "otbImageMetadataInterfaceBase.h"

#include "otbGeometryMetadata.h"
#include "itkMetaDataObject.h"
#include "itksys/SystemTools.hxx"


namespace otb
{

void ImageMetadataInterfaceBase::SetImage(ImageType* image)
{
  this->SetMetaDataDictionary(image->GetMetaDataDictionary());
}


void ImageMetadataInterfaceBase::SetMetaDataDictionary(const MetaDataDictionaryType& dict)
{
  m_MetaDataDictionary = dict;
}


const ImageMetadataInterfaceBase::MetaDataDictionaryType&
ImageMetadataInterfaceBase::GetMetaDataDictionary() const
{
  return m_MetaDataDictionary;
}

void ImageMetadataInterfaceBase::SetMetadataSupplierInterface(const MetadataSupplierInterface& mds)
{
  m_MetadataSupplierInterface = &mds;
}

std::string ImageMetadataInterfaceBase::GetProjectionRef() const
{
  std::string                   metadata;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::ProjectionRefKey))
  {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::ProjectionRefKey), metadata);
    return (metadata);
  }
  else
    return ("");
}

std::string ImageMetadataInterfaceBase::GetGCPProjection() const
{
  std::string                   metadata;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GCPProjectionKey))
  {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::GCPProjectionKey), metadata);
    return (metadata);
  }
  else
    return ("");
}

unsigned int ImageMetadataInterfaceBase::GetGCPCount() const
{
  unsigned int                  GCPCount = 0;
  const MetaDataDictionaryType& dict     = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GCPCountKey))
  {
    itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, GCPCount);
  }

  return (GCPCount);
}

GCP& ImageMetadataInterfaceBase::GetGCPs(unsigned int GCPnum)
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {

    itk::ExposeMetaData<GCP>(dict, key, m_GCP);
  }
  return (m_GCP);
}

std::string ImageMetadataInterfaceBase::GetGCPId(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_Id);
  }
  else
    return ("");
}

std::string ImageMetadataInterfaceBase::GetGCPInfo(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_Info);
  }
  else
    return ("");
}

double ImageMetadataInterfaceBase::GetGCPRow(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_GCPRow);
  }
  else
    return (0);
}

double ImageMetadataInterfaceBase::GetGCPCol(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_GCPCol);
  }
  else
    return (0);
}

double ImageMetadataInterfaceBase::GetGCPX(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_GCPX);
  }
  else
    return (0);
}

double ImageMetadataInterfaceBase::GetGCPY(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_GCPY);
  }
  else
    return (0);
}

double ImageMetadataInterfaceBase::GetGCPZ(unsigned int GCPnum) const
{
  std::string                   key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  std::ostringstream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    GCP gcp;
    itk::ExposeMetaData<GCP>(dict, key, gcp);
    return (gcp.m_GCPZ);
  }
  else
    return (0);
}

ImageMetadataInterfaceBase::VectorType ImageMetadataInterfaceBase::GetGeoTransform() const
{
  VectorType                    adfGeoTransform;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GeoTransformKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::GeoTransformKey, adfGeoTransform);
  }
  return (adfGeoTransform);
}

ImageMetadataInterfaceBase::VectorType ImageMetadataInterfaceBase::GetUpperLeftCorner() const
{
  VectorType                    UpperLeftCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::UpperLeftCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperLeftCornerKey, UpperLeftCorner);
  }
  return (UpperLeftCorner);
}

ImageMetadataInterfaceBase::VectorType ImageMetadataInterfaceBase::GetUpperRightCorner() const
{
  VectorType                    UpperRightCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::UpperRightCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperRightCornerKey, UpperRightCorner);
  }
  return (UpperRightCorner);
}

ImageMetadataInterfaceBase::VectorType ImageMetadataInterfaceBase::GetLowerLeftCorner() const
{
  VectorType                    LowerLeftCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::LowerLeftCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerLeftCornerKey, LowerLeftCorner);
  }
  return (LowerLeftCorner);
}

ImageMetadataInterfaceBase::VectorType ImageMetadataInterfaceBase::GetLowerRightCorner() const
{
  VectorType                    LowerRightCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::LowerRightCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerRightCornerKey, LowerRightCorner);
  }
  return (LowerRightCorner);
}

void ImageMetadataInterfaceBase::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

const std::string&
ImageMetadataInterfaceBase::Fetch(
  MDStr key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (band >= 0)
    {
    assert( (size_t)(band) < imd.Bands.size());
    imd.Bands[band].Add(key, m_MetadataSupplierInterface->GetAs<std::string>(path, band));
    return imd.Bands[band][key];
    }
  imd.Add(key, m_MetadataSupplierInterface->GetAs<std::string>(path) );
  return imd[key];
}

bool
ImageMetadataInterfaceBase::CheckFetch(
  MDStr key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (m_MetadataSupplierInterface->HasValue(path, band))
  {
    Fetch(key, imd, path, band);
    return true;
  }
  return false;
}

const double&
ImageMetadataInterfaceBase::Fetch(
  MDNum key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (band >= 0)
    {
    assert( (size_t)(band) < imd.Bands.size());
    imd.Bands[band].Add(key, m_MetadataSupplierInterface->GetAs<double>(path, band));
    return imd.Bands[band][key];
    }
  imd.Add(key, m_MetadataSupplierInterface->GetAs<double>(path));
  return imd[key];
}

bool
ImageMetadataInterfaceBase::CheckFetch(
  MDNum key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (m_MetadataSupplierInterface->HasValue(path, band))
  {
    Fetch(key, imd, path, band);
    return true;
  }
  return false;
}

const MetaData::TimePoint&
ImageMetadataInterfaceBase::Fetch(
  MDTime key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (band >= 0)
    {
    assert( (size_t)(band) < imd.Bands.size());
    imd.Bands[band].Add(key, MetaData::ReadFormattedDate(m_MetadataSupplierInterface->GetAs<std::string>(path, band)));
    return imd.Bands[band][key];
    }

  imd.Add(key, MetaData::ReadFormattedDate(m_MetadataSupplierInterface->GetAs<std::string>(path)));
  return imd[key];
}

bool
ImageMetadataInterfaceBase::CheckFetch(
  MDTime key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (m_MetadataSupplierInterface->HasValue(path, band))
  {
    Fetch(key, imd, path, band);
    return true;
  }
  return false;
}

const std::string&
ImageMetadataInterfaceBase::Fetch(
  std::string key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (band >= 0)
    {
    assert( (size_t)(band) < imd.Bands.size());
    imd.Bands[band].Add(key, m_MetadataSupplierInterface->GetAs<std::string>(path, band));
    return imd.Bands[band][key];
    }
  imd.Add(key, m_MetadataSupplierInterface->GetAs<std::string>(path) );
  return imd[key];
}

bool
ImageMetadataInterfaceBase::CheckFetch(
  std::string key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (m_MetadataSupplierInterface->HasValue(path, band))
  {
    Fetch(key, imd, path, band);
    return true;
  }
  return false;
}

const boost::any& ImageMetadataInterfaceBase::FetchRPC(
        ImageMetadata& imd,
        const double lineOffset, const double sampleOffset)
{
  Projection::RPCParam rpcStruct;

  // In some products, RPC metadata read by GDAL have an unit attached, and the
  // fetched string cannot be converted to double directly, e.g.
  // LINE_OFF=+002320.00 pixels (from an Ikonos product)
  // This lambda removes the unit suffix from the metadata.
  auto GetMetadataWithoutUnit = [this](const std::string & path)
  {
    auto metadataAsString = this->m_MetadataSupplierInterface->GetAs<std::string>(path);

    for (const auto & name : {"meters", "degrees", "pixels"})
    {
      auto i = metadataAsString.find(name);
      if (i != std::string::npos)
      {
        metadataAsString.erase(i, strlen(name));
        break;
      }
    }
    
    try
    {
      return std::stod(metadataAsString);
    }
    catch (const std::invalid_argument&)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<path<<"', got: "<<metadataAsString)
    }
  };

  rpcStruct.LineOffset    = GetMetadataWithoutUnit("RPC/LINE_OFF") + lineOffset;
  rpcStruct.SampleOffset  = GetMetadataWithoutUnit("RPC/SAMP_OFF") + sampleOffset;
  rpcStruct.LatOffset     = GetMetadataWithoutUnit("RPC/LAT_OFF");
  rpcStruct.LonOffset     = GetMetadataWithoutUnit("RPC/LONG_OFF");
  rpcStruct.HeightOffset  = GetMetadataWithoutUnit("RPC/HEIGHT_OFF");

  rpcStruct.LineScale    = GetMetadataWithoutUnit("RPC/LINE_SCALE");
  rpcStruct.SampleScale  = GetMetadataWithoutUnit("RPC/SAMP_SCALE");
  rpcStruct.LatScale     = GetMetadataWithoutUnit("RPC/LAT_SCALE");
  rpcStruct.LonScale     = GetMetadataWithoutUnit("RPC/LONG_SCALE");
  rpcStruct.HeightScale  = GetMetadataWithoutUnit("RPC/HEIGHT_SCALE");

  std::vector<double> coeffs(20);

  coeffs = m_MetadataSupplierInterface->GetAsVector<double>("RPC/LINE_NUM_COEFF",' ',20);
  std::copy(coeffs.begin(), coeffs.end(), rpcStruct.LineNum);

  coeffs = m_MetadataSupplierInterface->GetAsVector<double>("RPC/LINE_DEN_COEFF",' ',20);
  std::copy(coeffs.begin(), coeffs.end(), rpcStruct.LineDen);

  coeffs = m_MetadataSupplierInterface->GetAsVector<double>("RPC/SAMP_NUM_COEFF",' ',20);
  std::copy(coeffs.begin(), coeffs.end(), rpcStruct.SampleNum);

  coeffs = m_MetadataSupplierInterface->GetAsVector<double>("RPC/SAMP_DEN_COEFF",' ',20);
  std::copy(coeffs.begin(), coeffs.end(), rpcStruct.SampleDen);

  imd.Add(MDGeom::RPC, rpcStruct);
  assert(imd.Has(MDGeom::RPC));
  assert(rpcStruct == boost::any_cast<Projection::RPCParam>(imd[MDGeom::RPC]));
  return imd[MDGeom::RPC];
}

} // end namespace otb
