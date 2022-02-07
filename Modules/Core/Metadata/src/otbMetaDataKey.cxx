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


#include "otbMetaDataKey.h"
#include <iomanip>
#include <ios>

namespace otb
{
namespace MetaDataKey
{
char const* DriverShortNameKey = "DriverShortName";
char const* DriverLongNameKey  = "DriverLongName";

char const* ProjectionRefKey = "ProjectionRef";

char const* GCPProjectionKey = "GCPProjection";
char const* GCPParametersKey = "GCP_";
char const* GCPCountKey      = "GCPCount";

char const* GeoTransformKey = "GeoTransform";

char const* MetadataKey    = "Metadata_";
char const* SubMetadataKey = "SubMetadata_";

char const* UpperLeftCornerKey  = "UpperLeftCorner";
char const* UpperRightCornerKey = "UpperRightCorner";
char const* LowerLeftCornerKey  = "LowerLeftCorner";
char const* LowerRightCornerKey = "LowerRightCorner";

char const* ColorTableNameKey  = "ColorTableName";
char const* ColorEntryCountKey = "ColorEntryCount";
char const* ColorEntryAsRGBKey = "ColorEntryAsRGB";

char const* VectorDataKeywordlistKey          = "VectorDataKeywordlist";
char const* VectorDataKeywordlistDelimiterKey = "VectorDataKeywordlistDelimiter";

char const* ResolutionFactor = "ResolutionFactor";
char const* SubDatasetIndex  = "SubDatasetIndex";
char const* CacheSizeInBytes = "CacheSizeInBytes";

char const* TileHintX = "TileHintX";
char const* TileHintY = "TileHintY";

char const* NoDataValueAvailable = "NoDataValueAvailable";
char const* NoDataValue          = "NoDataValue";

char const* DataType = "DataType";

}

const MetaDataKey::KeyTypeDef Types[] = {
    MetaDataKey::KeyTypeDef(MetaDataKey::DriverShortNameKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::DriverLongNameKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::ProjectionRefKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::GCPProjectionKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::GCPParametersKey, MetaDataKey::TGCP),
    MetaDataKey::KeyTypeDef(MetaDataKey::GCPCountKey, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::GeoTransformKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::MetadataKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::SubMetadataKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::UpperLeftCornerKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::UpperRightCornerKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::LowerLeftCornerKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::LowerRightCornerKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::ColorTableNameKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryCountKey, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryAsRGBKey, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistKey, MetaDataKey::TVECTORDATAKEYWORDLIST),
    MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistDelimiterKey, MetaDataKey::TSTRING),
    MetaDataKey::KeyTypeDef(MetaDataKey::ResolutionFactor, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::SubDatasetIndex, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::CacheSizeInBytes, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::TileHintX, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::TileHintY, MetaDataKey::TENTIER),
    MetaDataKey::KeyTypeDef(MetaDataKey::NoDataValueAvailable, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::NoDataValue, MetaDataKey::TVECTOR),
    MetaDataKey::KeyTypeDef(MetaDataKey::DataType, MetaDataKey::TENTIER),
};

MetaDataKey::KeyType MetaDataKey::GetKeyType(const std::string& name)
{
  // MetaDataKey::KeyType ktype(MetaDataKey::TSTRING);

  for (const auto& key : Types)
  {
    if (name.find(key.keyname) != std::string::npos)
    {
      return MetaDataKey::KeyType(key.type);
      // ktype = key.type;
      // return ktype;
    }
  }
  return MetaDataKey::KeyType(MetaDataKey::TSTRING);
}

namespace MetaData
{

std::string LUTAxis::ToJSON(bool multiline) const
{
  std::ostringstream oss;
  std::string sep;
  if (multiline)
  {
    sep = "\n";
  }
  oss << "{"
      << "\"Size\": \"" << Size << "\", " << sep
      << "\"Origin\": \"" << Origin << "\", " << sep
      << "\"Spacing\": \"" << Spacing << "\", " << sep
	  << "\"Values\": [";
  for (const auto& value : Values)
    oss << value << ", ";
  oss << "]}";
  return oss.str();
}

template <unsigned int VDim>
std::string LUT<VDim>::ToJSON(bool multiline) const
{
  std::ostringstream oss;
  std::string sep;
  if (multiline)
  {
    sep = "\n";
  }
  oss << "{"
      << "\"Axis\": [";
  for (unsigned int loop = 0 ; loop < VDim  ; loop++)
    oss << Axis[loop].ToJSON(multiline) << ", ";
  oss << "], " << sep
      << "\"Array\": [";
  for (const auto& value : Array)
    oss << value << ", ";
  oss << "]}";
  return oss.str();
}

template <unsigned int VDim>
std::string LUT<VDim>::ToString() const
{
  std::ostringstream oss;
  for (unsigned int dim = 0 ; dim < VDim ; dim++)
  {
    oss << "LUT" << VDim << "D.DIM" << dim << ".SIZE = " << Axis[dim].Size << "\n";
    if (! Axis[dim].Values.empty())
    // Irregular sampling
    {
      oss << "LUT" << VDim << "D.DIM" << dim << ".VALUES = ";
      otb::Join(oss, Axis[dim].Values, " ");
      oss << "\n";
    }
    else
    {
    // Regular sampling
      oss << "LUT" << VDim << "D.DIM" << dim << ".ORIGIN = " << Axis[dim].Origin << "\n"
          << "LUT" << VDim << "D.DIM" << dim << ".SPACING = " << Axis[dim].Spacing << "\n";
    }
  }
  oss << "LUT" << VDim << "D.ARRAY = ";
  otb::Join(oss, Array, " ");
  return oss.str();
}

template <unsigned int VDim>
void LUT<VDim>::FromString(std::string str)
{
  std::vector<std::string> lines;
  std::vector<std::string> parts;
  boost::split(lines, str, [](char c){return c == '\n';});
  for (std::string line : lines)
  {
    boost::split(parts, line, [](char c){return c == '=';});
    boost::trim(parts[1]);
    if (Utils::LexicalCast<int>(parts[0].substr(3, 1), "VDim") != VDim)
      throw std::invalid_argument("Wrong LUT dimension");
    if(parts[0].substr(6, 5) == "ARRAY")
    // this->Array
    {
      std::vector<std::string> str_array;
      boost::split(str_array, parts[1], [](char c){return c == ' ';});
      Array.reserve(str_array.size());
      std::transform(str_array.begin(), str_array.end(), back_inserter(Array),
                      [](std::string const& val) {return std::stod(val);});
    }
    else
    {
      unsigned int dim = Utils::LexicalCast<int>(parts[0].substr(9, 1), "parts[0].substr(9, 1)");
      if (dim > VDim)
        throw std::invalid_argument("LUT dimension higher than expected");
      std::string element = parts[0].substr(11);
      if (element == "SIZE ")
      // this->Axis[dim].Size
      {
        Axis[dim].Size = Utils::LexicalCast<int>(parts[1], "Axis[dim].Size");
      }
      else if (element == "VALUES ")
      // this->Axis[dim].Values
      {
        std::vector<std::string> str_array;
        boost::split(str_array, parts[1], [](char c){return c == ' ';});
        Axis[dim].Values.reserve(str_array.size());
        std::transform(str_array.begin(), str_array.end(), back_inserter(Axis[dim].Values),
                        [](std::string const& val) {return std::stod(val);});
      }
      else if (element == "ORIGIN ")
      // this->Axis[dim].Origin
      {
        Axis[dim].Origin = Utils::LexicalCast<double>(parts[1], "Axis[dim].Origin");
      }
      else if (element == "SPACING ")
      // this->Axis[dim].Spacing
      {
        Axis[dim].Spacing = Utils::LexicalCast<double>(parts[1], "Axis[dim].Spacing");
      }
    }
  }
}

template class LUT<1>;
template class LUT<2>;

MDNumBmType MDNumNames = bimapGenerator<MDNum>(std::map<MDNum, std::string> {
  {MDNum::TileHintX,"TileHintX"},
  {MDNum::TileHintY,"TileHintY"},
  {MDNum::DataType,"DataType"},
  {MDNum::NoData, "NoData"},
  {MDNum::NumberOfLines,"NumberOfLines"},
  {MDNum::NumberOfColumns,"NumberOfColumns"},
  {MDNum::AverageSceneHeight,"AverageSceneHeight"},
  {MDNum::OrbitNumber, "OrbitNumber"},
  {MDNum::PhysicalGain,"PhysicalGain"},
  {MDNum::PhysicalBias,"PhysicalBias"},
  {MDNum::SolarIrradiance,"SolarIrradiance"},
  {MDNum::SunElevation,"SunElevation"},
  {MDNum::SunAzimuth,"SunAzimuth"},
  {MDNum::SatElevation,"SatElevation"},
  {MDNum::SatAzimuth,"SatAzimuth"},
  {MDNum::SpectralStep,"SpectralStep"},
  {MDNum::SpectralMin,"SpectralMin"},
  {MDNum::SpectralMax,"SpectralMax"},
  {MDNum::CalScale,"CalScale"},
  {MDNum::CalFactor,"CalFactor"},
  {MDNum::AbsoluteCalibrationConstant,"AbsoluteCalibrationConstant"},
  {MDNum::PRF,"PRF"},
  {MDNum::RSF,"RSF"},
  {MDNum::RadarFrequency,"RadarFrequency"},
  {MDNum::CenterIncidenceAngle,"CenterIncidenceAngle"},
  {MDNum::RescalingFactor,"RescalingFactor"},
  {MDNum::LineSpacing,"LineSpacing"},
  {MDNum::PixelSpacing,"PixelSpacing"},
  {MDNum::RangeTimeFirstPixel,"RangeTimeFirstPixel"},
  {MDNum::RangeTimeLastPixel,"RangeTimeLastPixel"},
  {MDNum::RedDisplayChannel,"RedDisplayChannel"},
  {MDNum::GreenDisplayChannel,"GreenDisplayChannel"},
  {MDNum::BlueDisplayChannel,"BlueDisplayChannel"},
});

MDStrBmType MDStrNames = bimapGenerator<MDStr>(std::map<MDStr, std::string> {
  {MDStr::SensorID,"SensorID"},
  {MDStr::Mission,"Mission"},
  {MDStr::Instrument,"Instrument"},
  {MDStr::InstrumentIndex,"InstrumentIndex"},
  {MDStr::ProductType,"ProductType"},
  {MDStr::GeometricLevel,"GeometricLevel"},
  {MDStr::RadiometricLevel,"RadiometricLevel"},
  {MDStr::Polarization,"Polarization"},
  {MDStr::Mode, "Mode"},
  {MDStr::Swath, "Swath"},
  {MDStr::OrbitDirection, "OrbitDirection"},
  {MDStr::BandName,"BandName"},
  {MDStr::EnhancedBandName,"EnhancedBandName"},
  {MDStr::BeamMode, "BeamMode"},
  {MDStr::BeamSwath, "BeamSwath"},
  {MDStr::AreaOrPoint, "AREA_OR_POINT"},
  {MDStr::LayerType, "LAYER_TYPE"},
  {MDStr::MetadataType, "METADATATYPE"},
  {MDStr::OtbVersion, "OTB_VERSION"},
});

MDTimeBmType MDTimeNames = bimapGenerator<MDTime>(std::map<MDTime, std::string> {
  {MDTime::AcquisitionDate,"AcquisitionDate"},
  {MDTime::ProductionDate,"ProductionDate"},
  {MDTime::AcquisitionStartTime,"AcquisitionStartTime"},
  {MDTime::AcquisitionStopTime,"AcquisitionStopTime"},
});

MDL1DBmType MDL1DNames = bimapGenerator<MDL1D>(std::map<MDL1D, std::string> {
  {MDL1D::SpectralSensitivity,"SpectralSensitivity"},
});

MDL2DBmType MDL2DNames = bimapGenerator<MDL2D>(std::map<MDL2D, std::string> {});

MDGeomBmType MDGeomNames = bimapGenerator<MDGeom>(std::map<MDGeom, std::string> {
  {MDGeom::ProjectionWKT,  "ProjectionWKT"},
  {MDGeom::ProjectionEPSG, "ProjectionEPSG"},
  {MDGeom::ProjectionProj, "ProjectionProj"},
  {MDGeom::RPC,            "RPC"},
  {MDGeom::SAR,            "SAR"},
  {MDGeom::SARCalib,       "SARCalib"},
  {MDGeom::SensorGeometry, "SensorGeometry"},
  {MDGeom::GCP,            "GCP"},
  {MDGeom::Adjustment,     "Adjustment"}
});

template<>
std::string EnumToString(MDGeom value)
{
  return MetaData::MDGeomNames.left.at(value);
}

template<>
std::string EnumToString(MDNum value)
{
  return MetaData::MDNumNames.left.at(value);
}

template<>
std::string EnumToString(MDStr value)
{
  return MetaData::MDStrNames.left.at(value);
}

template<>
std::string EnumToString(MDL1D value)
{
  return MetaData::MDL1DNames.left.at(value);
}

template<>
std::string EnumToString(MDL2D value)
{
  return MetaData::MDL2DNames.left.at(value);
}

template<>
std::string EnumToString(MDTime value)
{
  return MetaData::MDTimeNames.left.at(value);
}

// Specialization for extra keys
template<>
std::string EnumToString(std::string value)
{
  return value;
}






} // end namespace MetaData

} // end namespace otb
