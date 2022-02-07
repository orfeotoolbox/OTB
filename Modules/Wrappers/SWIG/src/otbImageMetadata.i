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

%module("threads"=1) otbImageMetadata

%{
#include "itkBase.includes"
#include "otbWrapperSWIGIncludes.h"
#include <string>
#include <sstream>
#include <locale>
#include "otbMetaDataKey.h"
#include "otbImageMetadata.h"
#define SWIG_FILE_WITH_INIT
%}

// Language specific extension
%include "Python.i"
%include "itkMacro.i"
%include "itkBase.i"

namespace otb
{

class GCP
{
public:
  std::string m_Id;
  std::string m_Info;
  double m_GCPCol;
  double m_GCPRow;
  double m_GCPX;
  double m_GCPY;
  double m_GCPZ;
  GCP();
  void Print(std::ostream& os) const;
};

namespace MetaData
{

class TimePoint
{

public:
  double GetJulianDay() const;
  double GetModifiedJulianDay() const;
};

struct LUTAxis
{
  int Size;
  double Origin;
  double Spacing;
  std::vector<double> Values;
  std::string ToJSON(bool multiline=false) const;
};

template <unsigned int VDim> class LUT
{
public:
  LUTAxis Axis[VDim];
  std::vector<double> Array;
  std::string ToJSON(bool multiline=false) const;
  std::string ToString() const;
  void FromString(std::string);
};

%template(lut1d) LUT<1>;
%template(lut2d) LUT<2>;
} // end namespace MetaData

class ImageMetadataBase
{
public:
  std::string GetProjectedGeometry() const;
  std::string GetProjectionWKT() const;
  std::string GetProjectionProj() const;

  std::string GetKeyListNum() const;
  std::string GetKeyListStr() const;
  std::string GetKeyListL1D() const;
//  std::string GetKeyListL2D() const;
  std::string GetKeyListTime() const;
};

class ImageMetadata: public ImageMetadataBase
{
public:
  std::vector<ImageMetadataBase> Bands;
};

};

%extend otb::MetaData::TimePoint{
  std::string __repr__() {
    std::ostringstream oss;
    oss << *$self;
    return oss.str().c_str();
  }
}

%extend otb::ImageMetadataBase{

  bool is_num(const std::string& key) {
    auto it = otb::MetaData::MDNumNames.right.find(key);
    return it != otb::MetaData::MDNumNames.right.end();
  }

  double get_num(const std::string& key) {
    auto it = otb::MetaData::MDNumNames.right.find(key);
    return $self->operator[](it->second);
  }

  bool is_str(const std::string& key) {
    auto it = otb::MetaData::MDStrNames.right.find(key);
    return it != otb::MetaData::MDStrNames.right.end();
  }

  std::string get_str(const std::string& key) {
    auto it = otb::MetaData::MDStrNames.right.find(key);
    return $self->operator[](it->second);
  }

  bool is_l1d(const std::string& key) {
    auto it = otb::MetaData::MDL1DNames.right.find(key);
    return it != otb::MetaData::MDL1DNames.right.end();
  }

  otb::MetaData::LUT1D get_l1d(const std::string& key) {
    auto it = otb::MetaData::MDL1DNames.right.find(key);
    return $self->operator[](it->second);
  }

  bool is_l2d(const std::string& key) {
    auto it = otb::MetaData::MDL2DNames.right.find(key);
    return it != otb::MetaData::MDL2DNames.right.end();
  }

  otb::MetaData::LUT2D get_l2d(const std::string& key) {
    auto it = otb::MetaData::MDL2DNames.right.find(key);
    return $self->operator[](it->second);
  }

  bool is_time(const std::string& key) {
    auto it = otb::MetaData::MDTimeNames.right.find(key);
    return it != otb::MetaData::MDTimeNames.right.end();
  }

  otb::MetaData::TimePoint get_time(const std::string& key) {
    auto it = otb::MetaData::MDTimeNames.right.find(key);
    return $self->operator[](it->second);
  }

  bool is_geom(const std::string& key) {
    auto it = otb::MetaData::MDGeomNames.right.find(key);
    return it != otb::MetaData::MDGeomNames.right.end();
  }

  bool has(const std::string& key) const {
    auto it_num = otb::MetaData::MDNumNames.right.find(key);
    if (it_num != otb::MetaData::MDNumNames.right.end())
      return $self->Has(it_num->second);

    auto it_str = otb::MetaData::MDStrNames.right.find(key);
    if (it_str != otb::MetaData::MDStrNames.right.end())
      return $self->Has(it_str->second);

    auto it_l1d = otb::MetaData::MDL1DNames.right.find(key);
    if (it_l1d != otb::MetaData::MDL1DNames.right.end())
      return $self->Has(it_l1d->second);

    auto it_l2d = otb::MetaData::MDL2DNames.right.find(key);
    if (it_l2d != otb::MetaData::MDL2DNames.right.end())
      return $self->Has(it_l2d->second);

    auto it_time = otb::MetaData::MDTimeNames.right.find(key);
    if (it_time != otb::MetaData::MDTimeNames.right.end())
      return $self->Has(it_time->second);

    auto it_geom = otb::MetaData::MDGeomNames.right.find(key);
    if (it_geom != otb::MetaData::MDGeomNames.right.end())
      return $self->Has(it_geom->second);

    return false;
  }
  
  void __setitem__(const std::string& key, const double val) {
    auto it = otb::MetaData::MDNumNames.right.find(key);
    if (it != otb::MetaData::MDNumNames.right.end())
      $self->Add(it->second, val);
    else
      $self->Add(key, std::to_string(val));
  }
  void __setitem__(const std::string& key, const std::string& val) {
    auto it = otb::MetaData::MDStrNames.right.find(key);
    if (it != otb::MetaData::MDStrNames.right.end())
      $self->Add(it->second, val);
    else
      $self->Add(key, val);
  }
  void __setitem__(const std::string& key, const otb::MetaData::LUT1D& val) {
    auto it = otb::MetaData::MDL1DNames.right.find(key);
    if (it != otb::MetaData::MDL1DNames.right.end())
      $self->Add(it->second, val);
    else
      $self->Add(key, val.ToString());
  }
  void __setitem__(const std::string& key, const otb::MetaData::LUT2D& val) {
    auto it = otb::MetaData::MDL2DNames.right.find(key);
    if (it != otb::MetaData::MDL2DNames.right.end())
      $self->Add(it->second, val);
    else
      $self->Add(key, val.ToString());
  }
  void __setitem__(const std::string& key, const otb::MetaData::TimePoint& val) {
    auto it = otb::MetaData::MDTimeNames.right.find(key);
    if (it != otb::MetaData::MDTimeNames.right.end())
      $self->Add(it->second, val);
    else
    {
      std::ostringstream timeStream;
      timeStream << val;
      $self->Add(key, timeStream.str());
    }
  }
  void __setitem__(const std::string& key, const boost::any val) {
    auto it = otb::MetaData::MDGeomNames.right.find(key);
    if (it != otb::MetaData::MDGeomNames.right.end())
      $self->Add(it->second, val);
    else
      throw std::runtime_error("[" + key + "] is not a valid geometric key");
  }
  
  %pythoncode {
    def __getitem__(self, key):
      if self.is_num(key):
        return self.get_num(key)
      elif self.is_str(key):
        return self.get_str(key)
      elif self.is_l1d(key):
        return self.get_l1d(key)
      elif self.is_l2d(key):
        return self.get_l2d(key)
      elif self.is_time(key):
        return self.get_time(key)
      elif self.is_geom(key):
        return None # TODO: the geom part of the Imagemetadata is not wrapped yet
      else:
        return None
  }
}
