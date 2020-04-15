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

#include "otbImageMetadata.h"
namespace otb
{
// ---------------------- [ImageMetadataBase] ------------------------------


bool ImageMetadataBase::HasSensorGeometry() const
{
  return Has(MDGeom::RPC) || Has(MDGeom::SAR) || Has(MDGeom::SensorGeometry);
}

bool ImageMetadataBase::HasProjectedGeometry() const
{
  return Has(MDGeom::ProjectionWKT) || Has(MDGeom::ProjectionEPSG) || Has(MDGeom::ProjectionProj);
}

size_t ImageMetadataBase::RemoveSensorGeometry()
{
  return Remove(MDGeom::RPC) + Remove(MDGeom::SAR) + Remove(MDGeom::SensorGeometry);
}

size_t ImageMetadataBase::RemoveProjectedGeometry()
{
  return Remove(MDGeom::ProjectionWKT) + Remove(MDGeom::ProjectionEPSG) + Remove(MDGeom::ProjectionProj);
}

const Projection::GCPParam & ImageMetadataBase::GetGCPParam() const
{
  return boost::any_cast<const Projection::GCPParam &>(GeometryKeys.at(MDGeom::GCP));
}

std::string ImageMetadataBase::GetProjectionWKT() const
{
  return boost::any_cast<std::string>(GeometryKeys.at(MDGeom::ProjectionWKT));
}

// -------------------- Geom utility function ----------------------------
const boost::any & ImageMetadataBase::operator[](const MDGeom& key) const
{
  return GeometryKeys.at(key);
}

void ImageMetadataBase::Add(const MDGeom& key, const boost::any &value)
{
  GeometryKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDGeom& key)
{
  return GeometryKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDGeom& key) const
{
  return (GeometryKeys.find(key) != GeometryKeys.end());
}

// -------------------- Double utility function ----------------------------

const double & ImageMetadataBase::operator[](const MDNum& key) const
{
  return NumericKeys.at(key);
}

void ImageMetadataBase::Add(const MDNum& key, const double &value)
{
  NumericKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDNum& key)
{
  return NumericKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDNum& key) const
{
  return (NumericKeys.find(key) != NumericKeys.end());
}

// -------------------- String utility function ----------------------------

const std::string & ImageMetadataBase::operator[](const MDStr& key) const
{
  return StringKeys.at(key);
}

void ImageMetadataBase::Add(const MDStr& key, const std::string &value)
{
  StringKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDStr& key)
{
  return StringKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDStr& key) const
{
  return (StringKeys.find(key) != StringKeys.end());
}

// -------------------- LUT1D utility function ----------------------------

const MetaData::LUT1D & ImageMetadataBase::operator[](const MDL1D& key) const
{
  return LUT1DKeys.at(key);
}

void ImageMetadataBase::Add(const MDL1D& key, const MetaData::LUT1D &value)
{
  LUT1DKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDL1D& key)
{
  return LUT1DKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDL1D& key) const
{
  return (LUT1DKeys.find(key) != LUT1DKeys.end());
}

// -------------------- 2D LUT utility function ----------------------------

const MetaData::LUT2D & ImageMetadataBase::operator[](const MDL2D& key) const
{
  return LUT2DKeys.at(key);
}

void ImageMetadataBase::Add(const MDL2D& key, const MetaData::LUT2D &value)
{
  LUT2DKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDL2D& key)
{
  return LUT2DKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDL2D& key) const
{
  return (LUT2DKeys.find(key) != LUT2DKeys.end());
}

// -------------------- Time utility function ----------------------------

const MetaData::Time & ImageMetadataBase::operator[](const MDTime& key) const
{
  return TimeKeys.at(key);
}

void ImageMetadataBase::Add(const MDTime& key, const MetaData::Time &value)
{
  TimeKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const MDTime& key)
{
  return TimeKeys.erase(key);
}

bool ImageMetadataBase::Has(const MDTime& key) const
{
  return (TimeKeys.find(key) != TimeKeys.end());
}

// -------------------- Extra keys utility function --------------------------

const std::string & ImageMetadataBase::operator[](const std::string & key) const
{
  return ExtraKeys.at(key);
}

void ImageMetadataBase::Add(const std::string& key, const std::string &value)
{
  ExtraKeys[key] = value;
}

size_t ImageMetadataBase::Remove(const std::string& key)
{
  return ExtraKeys.erase(key);
}

bool ImageMetadataBase::Has(const std::string& key) const
{
  return (ExtraKeys.find(key) != ExtraKeys.end());
}

void ImageMetadataBase::ToKeywordlist(Keywordlist& kwl) const
{
  kwl.clear();
  std::string cast_string;

  // Converting the GeomKeys
  for (const auto& kv : GeometryKeys)
  {

    if (kv.first == MDGeom::RPC)
    {
      Projection::RPCParam rpcStruct = boost::any_cast<Projection::RPCParam>(kv.second);
      cast_string = rpcStruct.ToJSON();
    }
    else if (kv.first == MDGeom::ProjectionEPSG)
    {
      cast_string = std::to_string(boost::any_cast<int>(kv.second));
    }
    else if (kv.first == MDGeom::GCP)
    {
      Projection::GCPParam gcpStruct = boost::any_cast<Projection::GCPParam>(kv.second);
      cast_string = gcpStruct.ToJSON();
    }
    // TODO : MDGeom::SensorGeometry (should be exported as "<typeinfo>" where typeinfo is boost::any::type().name()
    // TODO : MDGeom::SAR
    // TODO : MDGeom::Adjustment
    else
    {
      cast_string = boost::any_cast<std::string>(kv.second);
    }
    kwl.emplace(MetaData::MDGeomNames[kv.first], cast_string);

  }
  // Converting the StringKeys
  for (const auto& kv : StringKeys)
  {
    kwl.emplace(MetaData::MDStrNames[kv.first], kv.second);
  }
  // Converting the NumericKeys
  for (const auto& kv : NumericKeys)
  {
    std::ostringstream oss;
    oss << kv.second;
    kwl.emplace(MetaData::MDNumNames[kv.first], oss.str());
  }
  // Converting the LUT1DKeys
  for (const auto& kv : LUT1DKeys)
  {
    kwl.emplace(MetaData::MDL1DNames[kv.first], kv.second.ToJSON());
  }
  // Convereting the LUT2DKeys
  for (const auto& kv : LUT2DKeys)
  {
    kwl.emplace(MetaData::MDL2DNames[kv.first], kv.second.ToJSON());
  }
  // Converting the TimeKeys
  for (const auto& kv : TimeKeys)
  {
    std::ostringstream oss;
    oss << kv.second;
    kwl.emplace(MetaData::MDTimeNames[kv.first], oss.str());
  }
  // Converting the ExtraKeys
  std::string prefix("Extra.");
  for (const auto& kv : ExtraKeys)
  {
    kwl.emplace(prefix + kv.first, kv.second);
  }
}

std::string ImageMetadataBase::ToJSON(bool multiline) const
{
  Keywordlist kwl;
  ToKeywordlist(kwl);
  std::ostringstream oss;
  std::string sep;
  if (multiline)
    {
    sep = "\n";
    }
  oss << "{";
  for (const auto& kv : kwl)
    {
    oss << "\""<< kv.first << "\": \"" << kv.second << "\"," << sep;
    }
  oss << "}";
  return oss.str();
}

bool ImageMetadataBase::FromKeywordlist(const Keywordlist&)
{
  // TODO
  // skip MDGeom::SensorGeometry (they will be decoded by future SensorModelFactory)
  return false;
}

// ----------------------- [ImageMetadata] ------------------------------


ImageMetadata ImageMetadata::slice(int start, int end) const
{
  assert(start <= end);

  ImageMetadata imd;
  // Copy the keys
  imd.GeometryKeys = this->GeometryKeys;
  imd.NumericKeys = this->NumericKeys;
  imd.StringKeys = this->StringKeys;
  imd.LUT1DKeys = this->LUT1DKeys;
  imd.LUT2DKeys = this->LUT2DKeys;
  imd.TimeKeys = this->TimeKeys;
  imd.ExtraKeys = this->ExtraKeys;
  // Copy the bands
  auto first = this->Bands.cbegin() + start;
  auto last = this->Bands.cbegin() + end + 1;
  imd.Bands = ImageMetadataBandsType(first, last);
  return imd;
}

/** concatenate with an other ImageMetadata */
void ImageMetadata::append(const ImageMetadata& imd)
{
  // Copy the keys
  this->GeometryKeys.insert(imd.GeometryKeys.begin(), imd.GeometryKeys.end());
  this->NumericKeys.insert(imd.NumericKeys.begin(), imd.NumericKeys.end());
  this->StringKeys.insert(imd.StringKeys.begin(), imd.StringKeys.end());
  this->LUT1DKeys.insert(imd.LUT1DKeys.begin(), imd.LUT1DKeys.end());
  this->LUT2DKeys.insert(imd.LUT2DKeys.begin(), imd.LUT2DKeys.end());
  this->TimeKeys.insert(imd.TimeKeys.begin(), imd.TimeKeys.end());
  this->ExtraKeys.insert(imd.ExtraKeys.begin(), imd.ExtraKeys.end());
  // Copy the bands
  this->Bands.insert(this->Bands.end(), imd.Bands.begin(), imd.Bands.end());
}

/** if all bands share the same value of a key, put it at top level */
void ImageMetadata::compact()
{
  // TODO
}

// printing
std::ostream& operator<<(std::ostream& os, const otb::ImageMetadataBase& imd)
{
  os << imd.ToJSON(true);
  return os;
}

std::ostream& operator<<(std::ostream& os, const otb::ImageMetadata& imd)
{
  os << "{";
  os << imd.ToJSON(true);
  os << "Bands[";
  for (auto &band : imd.Bands)
    {
    os << band.ToJSON(true) << ",";
    }
  os << "]\n";
  os << "}";

  return os;
}

}
