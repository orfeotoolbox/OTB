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

#ifndef otbMetaDataKey_h
#define otbMetaDataKey_h

#include <string>
#include <vector>
#include <cstdio>

#include <boost/bimap.hpp>
#include <boost/algorithm/string.hpp>

#include "itkDataObject.h"
#include "itkVariableLengthVector.h"
#include "OTBMetadataExport.h"
#include "otbStringUtils.h"
#include "otbJoinContainer.h"

namespace otb
{
/** \namespace MetaDataKey
 * Definition of the key of metadata used in the metadata dictionary.
 */
namespace MetaDataKey
{
extern OTBMetadata_EXPORT char const* DriverShortNameKey;
extern OTBMetadata_EXPORT char const* DriverLongNameKey;

extern OTBMetadata_EXPORT char const* ProjectionRefKey;

extern OTBMetadata_EXPORT char const* GCPProjectionKey;
extern OTBMetadata_EXPORT char const* GCPParametersKey;
extern OTBMetadata_EXPORT char const* GCPCountKey;

extern OTBMetadata_EXPORT char const* GeoTransformKey;

extern OTBMetadata_EXPORT char const* MetadataKey;
extern OTBMetadata_EXPORT char const* SubMetadataKey;

extern OTBMetadata_EXPORT char const* UpperLeftCornerKey;
extern OTBMetadata_EXPORT char const* UpperRightCornerKey;
extern OTBMetadata_EXPORT char const* LowerLeftCornerKey;
extern OTBMetadata_EXPORT char const* LowerRightCornerKey;

extern OTBMetadata_EXPORT char const* ColorTableNameKey;
extern OTBMetadata_EXPORT char const* ColorEntryCountKey;
extern OTBMetadata_EXPORT char const* ColorEntryAsRGBKey;

extern OTBMetadata_EXPORT char const* OSSIMKeywordlistKey;
extern OTBMetadata_EXPORT char const* OSSIMKeywordlistDelimiterKey;

extern OTBMetadata_EXPORT char const* VectorDataKeywordlistKey;
extern OTBMetadata_EXPORT char const* VectorDataKeywordlistDelimiterKey;

extern OTBMetadata_EXPORT char const* ResolutionFactor;
extern OTBMetadata_EXPORT char const* SubDatasetIndex;
extern OTBMetadata_EXPORT char const* CacheSizeInBytes;

extern OTBMetadata_EXPORT char const* TileHintX;
extern OTBMetadata_EXPORT char const* TileHintY;

extern OTBMetadata_EXPORT char const* NoDataValueAvailable;
extern OTBMetadata_EXPORT char const* NoDataValue;

extern OTBMetadata_EXPORT char const* DataType;


enum KeyType
{
  TSTRING,
  TENTIER,
  TDOUBLE,
  TGCP,
  TVECTOR,
  TOSSIMKEYWORDLIST,
  TVECTORDATAKEYWORDLIST,
  TBOOLVECTOR
};
/*
typedef struct
{
  std::string keyname;
  KeyType type;
} KeyTypeDef; */

struct KeyTypeDef
{
  std::string keyname;
  KeyType     type;

  KeyTypeDef()
  {
  }
  KeyTypeDef(const std::string& _keyname, const KeyType& _type)
  {
    keyname = _keyname;
    type    = _type;
  }
};

KeyType OTBMetadata_EXPORT GetKeyType(const std::string& name);

typedef std::vector<double>               VectorType;
typedef std::vector<bool>                 BoolVectorType;
typedef itk::VariableLengthVector<double> VariableLengthVectorType;
}

/** Metadata as double*/
enum class MDNum
{
// generic
  TileHintX,
  TileHintY,
  DataType,
  NoData,
  OrbitNumber,
  NumberOfLines,
  NumberOfColumns,
  AverageSceneHeight,
// optical section
  PhysicalGain,
  PhysicalBias,
  SolarIrradiance,
  SunElevation,
  SunAzimuth,
  SatElevation,
  SatAzimuth,
  FirstWavelength,
  LastWavelength,
  SpectralStep,
  SpectralMin,
  SpectralMax,
// SAR section
  CalScale,
  PRF,
  RSF,
  RadarFrequency,
  CenterIncidenceAngle,
  RescalingFactor,
  AntennaPatternNewGainPolyDegX,
  AntennaPatternNewGainPolyDegY,
  AntennaPatternOldGainPolyDegX,
  AntennaPatternOldGainPolyDegY,
  IncidenceAnglePolyDegX,
  IncidenceAnglePolyDegY,
  RangeSpreadLossPolyDegX,
  RangeSpreadLossPolyDegY,
  NoisePolyDegX,
  NoisePolyDegY,
  LineSpacing,
  PixelSpacing,
  END
};

/** Metadata as std::string */
enum class MDStr
{
  SensorID,
  Mission,
  Instrument,
  BandName,
  ProductType,
  GeometricLevel,
  RadiometricLevel,
  Polarization,
  Mode,
  Swath,
  OrbitDirection,
  BeamMode,
  BeamSwath,
  // ...
  END
};

/** Metadata as LUT 1D */
enum class MDL1D
{
  SpectralSensitivity,
  END
};

/** Metadata as LUT 2D */
enum class MDL2D
{
  // Sar calibration lut ...
  END
};

/** Metadata as Time */
enum class MDTime
{
  AcquisitionDate,
  ProductionDate,
  AcquisitionStartTime,
  AcquisitionStopTime,
  END
};

enum class MDGeom
{
  ProjectionWKT,  // -> string
  ProjectionEPSG, // -> int
  ProjectionProj, // -> string
  RPC,            // -> RPCParam
  SAR,            // -> SARParam
  SensorGeometry, // -> boost::any
  GCP,            // -> GCPParam
  Adjustment,     // -> ?
  END
};

namespace MetaData
{

struct OTBMetadata_EXPORT Time : tm
{
  double frac_sec;

  friend OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const Time& val);

  friend OTBMetadata_EXPORT std::istream& operator>>(std::istream& is, Time& val);

  friend OTBMetadata_EXPORT bool operator==(const Time & lhs, const Time & rhs)
  {
    tm tmLhs = lhs;
    tm tmRhs = rhs;
    return mktime(&tmLhs) + lhs.frac_sec == mktime(&tmRhs) + rhs.frac_sec;
  }

  friend OTBMetadata_EXPORT bool operator!=(const Time & lhs, const Time & rhs)
  {
    return !(lhs == rhs);
  }

  friend OTBMetadata_EXPORT bool operator<(const Time & lhs, const Time & rhs)
  {
    tm tmLhs = lhs;
    tm tmRhs = rhs;
    return mktime(&tmLhs) + lhs.frac_sec < mktime(&tmRhs) + rhs.frac_sec;
  }

  friend OTBMetadata_EXPORT bool operator>(const Time & lhs, const Time & rhs)
  {
    return rhs < lhs;
  }

  friend OTBMetadata_EXPORT bool operator<=(const Time & lhs, const Time & rhs)
  {
    return !(lhs > rhs);
  }

  friend OTBMetadata_EXPORT bool operator>=(const Time & lhs, const Time & rhs)
  {
    return !(lhs < rhs);
  }

};


struct LUTAxis
{
  /** number of measurements on this axis */
  int Size;
  /** start value on the axis */
  double Origin;
  /** spacing between measurements (if regular sampling) */
  double Spacing;
  /** list of measurements (if irregular sampling) */
  std::vector<double> Values;
  /** Export to JSON */
  std::string ToJSON(bool multiline=false) const;

  friend bool operator==(const LUTAxis & lhs, const LUTAxis & rhs)
  {
    return lhs.Size == rhs.Size
        && lhs.Origin == rhs.Origin
        && lhs.Spacing == rhs.Spacing
        && lhs.Values == rhs.Values;
  }
};

template <unsigned int VDim> class LUT
{
public:
  LUTAxis Axis[VDim];
  
  std::vector<double> Array;

  std::string OTBMetadata_EXPORT ToJSON(bool multiline=false) const;

  std::string OTBMetadata_EXPORT ToString() const;

  void OTBMetadata_EXPORT FromString(std::string);

  friend bool operator==(const LUT<VDim> & lhs, const LUT<VDim> & rhs)
  {
    return std::equal(std::begin(lhs.Array), std::end(lhs.Array), std::begin(rhs.Array) ) 
            && lhs.Array == rhs.Array;
  }

};


template <unsigned int VDim>
std::ostream& operator<<(std::ostream& os, const LUT<VDim>& val)
{
  os << val.ToString();
  return os;
}


typedef LUT<1> LUT1D;

typedef LUT<2> LUT2D;

template <typename T>
inline boost::bimap<T, std::string> bimapGenerator(std::map<T, std::string> inMap)
{
  boost::bimap<T, std::string> bm;
  for (const auto& kv : inMap)
    bm.insert({kv.first, kv.second});
    //bm.insert(typename boost::bimap<T, std::string>::value_type(kv.first, kv.second));
  return bm;
}

typedef boost::bimap<MDGeom, std::string> MDGeomBmType;
extern OTBMetadata_EXPORT MDGeomBmType MDGeomNames;

typedef boost::bimap<MDNum, std::string> MDNumBmType;
extern OTBMetadata_EXPORT MDNumBmType MDNumNames;

typedef boost::bimap<MDStr, std::string> MDStrBmType;
extern OTBMetadata_EXPORT MDStrBmType MDStrNames;

typedef boost::bimap<MDTime, std::string> MDTimeBmType;
extern OTBMetadata_EXPORT MDTimeBmType MDTimeNames;

typedef boost::bimap<MDL1D, std::string> MDL1DBmType;
extern OTBMetadata_EXPORT MDL1DBmType MDL1DNames;

typedef boost::bimap<MDL2D, std::string> MDL2DBmType;
extern OTBMetadata_EXPORT MDL2DBmType MDL2DNames;

template<class T>
std::string EnumToString(T t);

template<>
std::string EnumToString(MDGeom value);

template<>
std::string EnumToString(MDNum value);

template<>
std::string EnumToString(MDStr value);

template<>
std::string EnumToString(MDL1D value);

template<>
std::string EnumToString(MDL2D value);

template<>
std::string EnumToString(MDTime value);

// Specialization for extra keys
template<>
std::string EnumToString(std::string value);


} // end namespace MetaData

namespace Utils
{
template <>
inline MetaData::Time LexicalCast<MetaData::Time,std::string>(std::string const& in, std::string const& kind)
{
  MetaData::Time output;
  std::istringstream iss(in);
  iss >> output;
  if (iss.fail())
    {
    std::ostringstream oss;
    oss << "Cannot decode '" << in << "' as this is not a valid value for '" << kind << "'";
    throw std::runtime_error(oss.str());
    }
  return output;
}

} // end namespace Utils

} // end namespace otb

#endif
