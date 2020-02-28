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

#include "otbImageMetadataNG.h"
namespace otb
{
// ---------------------- [ImageMetadataBase] ------------------------------

// -------------------- Double utility function ----------------------------

const double & ImageMetadataBase::operator[](const MDNum& key) const
  {
  assert(key != MDNum::END);
  int pos = static_cast<int>(key);
  if (NumericKeys.first[pos] == false)
    {
    itkGenericExceptionMacro(<< "Missing value for numeric key: "<< pos);
    }
  return NumericKeys.second[pos];
  }

void ImageMetadataBase::Set(const MDNum& key, const double &value)
  {
  assert(key != MDNum::END);
  int pos = static_cast<int>(key);
  NumericKeys.second[pos] = value;
  NumericKeys.first[pos] = true;
  }

void ImageMetadataBase::Remove(const MDNum& key)
  {
  assert(key != MDNum::END);
  int pos = static_cast<int>(key);
  NumericKeys.first[pos] = false;
  NumericKeys.second[pos] = 0.0;
  }

bool ImageMetadataBase::Has(const MDNum& key) const
  {
  assert(key != MDNum::END);
  int pos = static_cast<int>(key);
  return NumericKeys.first[pos];
  }

// -------------------- String utility function ----------------------------

const std::string & ImageMetadataBase::operator[](const MDStr& key) const
  {
  assert(key != MDStr::END);
  int pos = static_cast<int>(key);
  if (StringKeys.first[pos] == false)
    {
    itkGenericExceptionMacro(<< "Missing value for string key: "<< pos);
    }
  return StringKeys.second[pos];
  }

void ImageMetadataBase::Set(const MDStr& key, const std::string &value)
  {
  assert(key != MDStr::END);
  int pos = static_cast<int>(key);
  StringKeys.second[pos] = value;
  StringKeys.first[pos] = true;
  }

void ImageMetadataBase::Remove(const MDStr& key)
  {
  assert(key != MDStr::END);
  int pos = static_cast<int>(key);
  StringKeys.first[pos] = false;
  StringKeys.second[pos] = "";
  }

bool ImageMetadataBase::Has(const MDStr& key) const
  {
  assert(key != MDStr::END);
  int pos = static_cast<int>(key);
  return StringKeys.first[pos];
  }

// -------------------- LUT1D utility function ----------------------------

const MetaData::LUT1D & ImageMetadataBase::operator[](const MDL1D& key) const
  {
  assert(key != MDL1D::END);
  int pos = static_cast<int>(key);
  if (LUT1DKeys.first[pos] == false)
    {
    itkGenericExceptionMacro(<< "Missing value for 1D LUT key: "<< pos);
    }
  return LUT1DKeys.second[pos];
  }

void ImageMetadataBase::Set(const MDL1D& key, const MetaData::LUT1D &value)
  {
  assert(key != MDL1D::END);
  int pos = static_cast<int>(key);
  LUT1DKeys.second[pos] = value;
  LUT1DKeys.first[pos] = true;
  }

void ImageMetadataBase::Remove(const MDL1D& key)
  {
  assert(key != MDL1D::END);
  int pos = static_cast<int>(key);
  LUT1DKeys.first[pos] = false;
  LUT1DKeys.second[pos] = MetaData::LUT1D();
  }

bool ImageMetadataBase::Has(const MDL1D& key) const
  {
  assert(key != MDL1D::END);
  int pos = static_cast<int>(key);
  return LUT1DKeys.first[pos];
  }

// -------------------- 2D LUT utility function ----------------------------

const MetaData::LUT2D & ImageMetadataBase::operator[](const MDL2D& key) const
  {
  assert(key != MDL2D::END);
  int pos = static_cast<int>(key);
  if (LUT2DKeys.first[pos] == false)
    {
    itkGenericExceptionMacro(<< "Missing value for 2D LUT key: "<< pos);
    }
  return LUT2DKeys.second[pos];
  }

void ImageMetadataBase::Set(const MDL2D& key, const MetaData::LUT2D &value)
  {
  assert(key != MDL2D::END);
  int pos = static_cast<int>(key);
  LUT2DKeys.second[pos] = value;
  LUT2DKeys.first[pos] = true;
  }

void ImageMetadataBase::Remove(const MDL2D& key)
  {
  assert(key != MDL2D::END);
  int pos = static_cast<int>(key);
  LUT2DKeys.first[pos] = false;
  LUT2DKeys.second[pos] = MetaData::LUT2D();
  }

bool ImageMetadataBase::Has(const MDL2D& key) const
  {
  assert(key != MDL2D::END);
  int pos = static_cast<int>(key);
  return LUT2DKeys.first[pos];
  }

// -------------------- Time utility function ----------------------------

const MetaData::Time & ImageMetadataBase::operator[](const MDTime& key) const
  {
  assert(key != MDTime::END);
  int pos = static_cast<int>(key);
  if (TimeKeys.first[pos] == false)
    {
    itkGenericExceptionMacro(<< "Missing value for time key: "<< pos);
    }
  return TimeKeys.second[pos];
  }

void ImageMetadataBase::Set(const MDTime& key, const MetaData::Time &value)
  {
  assert(key != MDTime::END);
  int pos = static_cast<int>(key);
  TimeKeys.second[pos] = value;
  TimeKeys.first[pos] = true;
  }

void ImageMetadataBase::Remove(const MDTime& key)
  {
  assert(key != MDTime::END);
  int pos = static_cast<int>(key);
  TimeKeys.first[pos] = false;
  }

bool ImageMetadataBase::Has(const MDTime& key) const
  {
  assert(key != MDTime::END);
  int pos = static_cast<int>(key);
  return TimeKeys.first[pos];
  }

// -------------------- Extra keys utility function --------------------------

const std::string & ImageMetadataBase::operator[](const std::string & key) const
  {
  return ExtraKeys.at(key);
  }

void ImageMetadataBase::Set(const std::string& key, const std::string &value)
  {
  ExtraKeys[key] = value;
  }

void ImageMetadataBase::Remove(const std::string& key)
  {
  ExtraKeys.erase(key);
  }

bool ImageMetadataBase::Has(const std::string& key) const
  {
  if (ExtraKeys.find(key) == ExtraKeys.end())
    {
    return false;
    }
  return true;
  }


// ----------------------- [ImageMetadata] ------------------------------


ImageMetadataNG ImageMetadataNG::slice(int start, int end)
{
  // TODO
  return ImageMetadataNG();
}

/** concatenate with an other ImageMetadata */
void ImageMetadataNG::append(const ImageMetadataNG& )
{
  // TODO
}

/** if all bands share the same value of a key, put it at top level */
void ImageMetadataNG::compact()
{
  // TODO
}

}
