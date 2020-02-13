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

#ifndef otbMetaDataKey_h
#define otbMetaDataKey_h

#include <string>
#include <vector>

#include "itkDataObject.h"
#include "itkVariableLengthVector.h"
#include "OTBMetadataExport.h"

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
  TOTB_GCP,
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

/** \class OTB_GCP
 *
 * \brief This OTB_GCP class is used to manage the GCP parameters
 * in OTB.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OTB_GCP
{
public:
  /** Unique identifier, often numeric */
  std::string m_Id;

  /** Informational message or "" */
  std::string m_Info;

  /** Pixel (x) location of GCP on raster */
  double m_GCPCol;

  /** Line (y) location of GCP on raster */
  double m_GCPRow;

  /** X position of GCP in georeferenced space */
  double m_GCPX;

  /** Y position of GCP in georeferenced space */
  double m_GCPY;

  /** Elevation of GCP, or zero if not known */
  double m_GCPZ;

  OTB_GCP();
  ~OTB_GCP();

  void Print(std::ostream& os) const;
};

/** Metadata as double*/
enum class MDNum
{
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
  CenterIndicenceAngle,
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
  NoisePolyDegY
};

/** Metadata as std::string */
enum class MDStr
{
  SensorID,
  Mission,
  Instrument,
  ProductType,
  GeometricLevel,
  RadiometricLevel,
  Polarization
  // ...
};

/** Metadata as LUT 1D */
enum class MDL1D
{
  SpectralSensitivity
};

/** Metadata as LUT 2D */
enum class MDL2D
{
  // Sar calibration lut ...
};

/** Metadata as Time */
enum class MDTime
{
  AcquisitionDate,
  ProductionDate
};

namespace MetaData
{

struct Time : tm
{
  double frac_sec;
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
};

template <unsigned int VDim> class LUT
{
public:
  LUTAxis Axes[VDim];
  
  std::vector<double> Array;
};

typedef LUT<1> LUT1D;

typedef LUT<2> LUT2D;

extern OTBMetadata_EXPORT std::map<MDNum, std::string> MDNumNames;

extern OTBMetadata_EXPORT std::map<MDStr, std::string> MDStrNames;

extern OTBMetadata_EXPORT std::map<MDTime, std::string> MDTimeNames;

extern OTBMetadata_EXPORT std::map<MDL1D, std::string> MDL1DNames;

extern OTBMetadata_EXPORT std::map<MDL2D, std::string> MDL2DNames;

} // end namespace MetaData

} // end namespace otb

#endif
