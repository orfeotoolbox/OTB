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


#include "otbImageMetadataInterfaceBase.h"

#include "otbNoDataHelper.h"
#include "otbGeometryMetadata.h"
#include "itkMetaDataObject.h"
#include "itksys/SystemTools.hxx"


namespace otb
{

ImageMetadataInterfaceBase::ImageMetadataInterfaceBase()
{
}


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

bool ImageMetadataInterfaceBase::GetNoDataFlags(std::vector<bool>& flags, std::vector<double>& values) const
{
  return ReadNoDataFlags(this->GetMetaDataDictionary(), flags, values);
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

ImageMetadataInterfaceBase::ImageKeywordlistType ImageMetadataInterfaceBase::GetImageKeywordlist()
{
  ImageKeywordlistType          imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  return (imageKeywordlist);
}

const ImageMetadataInterfaceBase::ImageKeywordlistType ImageMetadataInterfaceBase::GetImageKeywordlist() const
{
  ImageKeywordlistType          imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  return (imageKeywordlist);
}

std::string const ImageMetadataInterfaceBase::GetSensorID() const
{
  std::string s;
  GetSensorID(s);
  return s;
}

bool ImageMetadataInterfaceBase::GetSensorID(std::string& sensorId) const
{
  ImageKeywordlistType          imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (!imageKeywordlist.HasKey("sensor"))
  {
    return false;
  }

  sensorId = imageKeywordlist.GetMetadataByKey("sensor");

  return true;
}

unsigned int ImageMetadataInterfaceBase::GetNumberOfBands() const
{
  ImageKeywordlistType          imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (!imageKeywordlist.HasKey("support_data.number_bands"))
  {
    return 0;
  }

  std::string  valueString = imageKeywordlist.GetMetadataByKey("support_data.number_bands");
  unsigned int value       = atoi(valueString.c_str());
  return value;
}

void ImageMetadataInterfaceBase::PrintMetadata(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType& dict)
{

  std::vector<std::string> keys = dict.GetKeys();

  // an extra dependency just for printing is a bad idea.
  //  VectorDataKeywordlist    vectorDataKeywordlistValue;
  unsigned int i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;

  for (unsigned int itkey = 0; itkey < keys.size(); ++itkey)
  {
    switch (MetaDataKey::GetKeyType(keys[itkey]))
    {
    case MetaDataKey::TSTRING:
    {
      std::string svalue;
      itk::ExposeMetaData<std::string>(dict2, keys[itkey], svalue);
      os << indent << "---> " << keys[itkey] << " = " << svalue << std::endl;
      break;
    }
    case MetaDataKey::TENTIER:
    {
      unsigned int ivalue(0);
      itk::ExposeMetaData<unsigned int>(dict2, keys[itkey], ivalue);
      os << indent << "---> " << keys[itkey] << " = " << ivalue << std::endl;
      break;
    }
    case MetaDataKey::TVECTOR:
    {
      VectorType vvalue;
      itk::ExposeMetaData<VectorType>(dict2, keys[itkey], vvalue);

      for (i = 0; i < vvalue.size(); ++i)
      {
        os << indent << "---> " << keys[itkey] << "[" << i << "] = " << vvalue[i] << std::endl;
      }
      vvalue.clear();

      break;
    }
    case MetaDataKey::TDOUBLE:
    {
      double dvalue(0.0);
      itk::ExposeMetaData<double>(dict2, keys[itkey], dvalue);
      os << indent << "---> " << keys[itkey] << " = " << dvalue << std::endl;
      break;
    }

    case MetaDataKey::TGCP:
    {
      GCP gcpvalue;
      itk::ExposeMetaData<GCP>(dict2, keys[itkey], gcpvalue);

      os << indent << "---> " << keys[itkey] << std::endl;
      gcpvalue.Print(os);
      break;
    }
    case MetaDataKey::TOSSIMKEYWORDLIST:
    {
      ImageKeywordlist kwl;
      itk::ExposeMetaData<ImageKeywordlist>(dict2, keys[itkey], kwl);

      os << indent << "---> " << keys[itkey] << std::endl;
      kwl.Print(os);
      break;
    }
    //      case MetaDataKey::TVECTORDATAKEYWORDLIST:
    //        itk::ExposeMetaData<VectorDataKeywordlist>(dict2, keys[itkey], vectorDataKeywordlistValue);
    //
    //        os << indent << "---> " << keys[itkey] << std::endl;
    //        vectorDataKeywordlistValue.Print(os);
    //        break;
    default:
      break;
    }
  }
}


void ImageMetadataInterfaceBase::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);

  bool canRead = this->CanRead();

  os << indent << "Initialized: " << (canRead ? true : false) << std::endl;
  if (canRead)
  {
    os << indent << "ProjectionRef:   " << this->GetProjectionRef() << std::endl;
    os << indent << "GCPProjection:   " << this->GetGCPProjection() << std::endl;
    os << indent << "GCPCount:        " << this->GetGCPCount() << std::endl;
    for (unsigned int gcpIdx = 0; gcpIdx < this->GetGCPCount(); ++gcpIdx)
    {
      // os << indent << "GCPs:            " << this->GetGCPs(gcpIdx) << std::endl;
      os << indent << "GCPId:           " << this->GetGCPId(gcpIdx) << std::endl;
      os << indent << "GCPInfo:         " << this->GetGCPInfo(gcpIdx) << std::endl;
    }
    // os << indent << "GeoTransform:    " << this->GetGeoTransform( ) << std::endl;
    // os << indent << "UpperLeftCorner: " << this->GetUpperLeftCorner( ) << std::endl;
    // os << indent << "UpperRightCorner:" << this->GetUpperRightCorner( ) << std::endl;
    // os << indent << "LowerLeftCorner: " << this->GetLowerLeftCorner( ) << std::endl;
    // os << indent << "LowerRightCorner:" << this->GetLowerRightCorner( ) << std::endl;
    // os << indent << "ImageKeywordlist:" << this->GetImageKeywordlist( ) << std::endl;
    std::string sensorId;
    this->GetSensorID(sensorId);
    os << indent << "SensorID:        " << sensorId << std::endl;
    os << indent << "NumberOfBands:   " << this->GetNumberOfBands() << std::endl;
  }
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

const MetaData::Time&
ImageMetadataInterfaceBase::Fetch(
  MDTime key,
  ImageMetadata& imd,
  const char *path,
  int band)
{
  if (band >= 0)
    {
    assert( (size_t)(band) < imd.Bands.size());
    imd.Bands[band].Add(key, m_MetadataSupplierInterface->GetAs<MetaData::Time>(path, band));
    return imd.Bands[band][key];
    }

  imd.Add(key, m_MetadataSupplierInterface->GetAs<MetaData::Time>(path));
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
