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

void ToKeywordlist(Keywordlist& kwl) const
{
  kwl.clear();
  // TODO : Geometry
  for (const auto& kv : StringKeys)
    {
    kwl.emplace(MetaData::MDStrNames[kv.first], kv.second);
    }
  for (const auto& kv : NumericKeys)
    {
    std::ostringstream oss;
    oss << kv.second;
    kwl.emplace(MetaData::MDNumNames[kv.first], oss.str());
    }
  // TODO : LUT1D

  // TODO : LUT2D

  // TODO : Time
  
  std::string prefix("Extra.");
  for (const auto& kv : ExtraKeys)
    {
    kwl.emplace(prefix + kv.first, kv.second);
    }
}

std::string ToJSON(bool multiline=false) const
{
  Keywordlist kwl;
  ToKeywordlist(kwl);
  std::ostringstream oss;
  std::string sep();
  if (multiline) sep = "\n";
  oss << "{";
  for (const auto& kv : kwl)
    {
    oss << "\"" kv.first << "\": \"" << kv.second << "\"," << sep
    }
  oss << "}";
  return oss.str();
}

// ----------------------- [ImageMetadata] ------------------------------


ImageMetadataNG ImageMetadata::slice(int start, int end)
{
  // TODO
  return ImageMetadata();
}

/** concatenate with an other ImageMetadata */
void ImageMetadata::append(const ImageMetadata& )
{
  // TODO
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
    os << band << ",";
    }
  os << "]\n";
  os << "}";

  return os;
}

}
