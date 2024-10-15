/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSARMetadata_h
#define otbSARMetadata_h

#include "OTBMetadataExport.h"
#include "otbMetaDataKey.h"
#include "otbSarCalibrationLookupData.h"

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "itkPoint.h"

#include "itkPointSet.h"
#include "otbDateTime.h"

namespace otb
{

class SarCalibrationLookupData;

/** \struct AzimuthFmRate
 *
 * \brief This structure is used to manage parameters
 * related to the Azimuth Frequency Modulation rate
 */
struct OTBMetadata_EXPORT AzimuthFmRate
{
  /** Zero Doppler azimuth time to which azimuth FM rate parameters apply */
  MetaData::TimePoint azimuthTime;
  /** Two way slant range time origin used for azimuth FM rate calculation */
  double t0;
  /** Azimuth FM rate coefficients c0 c1 c2 */
  std::vector<double> azimuthFmRatePolynomial;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static AzimuthFmRate FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct DopplerCentroid
 *
 * \brief This structure is used to handle Doppler centroid estimates
 */
struct OTBMetadata_EXPORT DopplerCentroid
{
  /** Zero Doppler azimuth time of this Doppler centroid estimate */
  MetaData::TimePoint azimuthTime;
  /* Two-way slant range time origin for Doppler centroid estimate */
  double t0;
  /* Doppler centroid estimated from data */
  std::vector<double> dopCoef;
  /* Doppler centroid estimated from orbit */
  std::vector<double> geoDopCoef;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static DopplerCentroid FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct Orbit
 *
 * \brief This structure is used to handle orbit information
 */
struct OTBMetadata_EXPORT Orbit
{
  using PointType = itk::Point<double, 3>;

  /** Timestamp at which orbit state vectors apply */
  MetaData::TimePoint time;
  /** Position vector */
  PointType position;
  /** Velocity vector */
  PointType velocity;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static Orbit FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct BurstRecord
 *
 * \brief This structure is used to handle burst records
 */
struct OTBMetadata_EXPORT BurstRecord
{
  MetaData::TimePoint      azimuthStartTime;
  MetaData::TimePoint      azimuthStopTime;
  unsigned long startLine;
  unsigned long endLine;
  unsigned long startSample;
  unsigned long endSample;
  double        azimuthAnxTime;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static BurstRecord FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};


/** \struct GCPTime
 *
 * \brief This structure contains the azimuth and range times associated with a gcp
 */
struct OTBMetadata_EXPORT GCPTime
{
  /** Azimuth time of the gcp */
  MetaData::TimePoint azimuthTime;

  /** Slant range time of the gcp */
  double slantRangeTime;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;
};

/** \struct CoordinateConversionRecord
 *
 * \brief This structure contains coefficients to convert between coordinates types, e.g. 
 * from ground range to slant range
 */
struct CoordinateConversionRecord
{
  MetaData::TimePoint azimuthTime;
  double rg0;
  std::vector<double> coeffs;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static CoordinateConversionRecord FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct InfoSceneCoord
 * \brief Represents an InfoSceneCoord
 */
struct InfoSceneCoord
{
  /** Reference row (refRow) */
  unsigned long referenceRow;

  /** Reference column (refColumn) */
  unsigned long referenceColumn;

  /** Latitude (lat) */
  double latitude;

  /** Longitude (lon) */
  double longitude;

  /** Azimuth time (azimuthTimeUTC) */
  MetaData::TimePoint azimuthTime;

  /** Range time (rangeTime) */
  double rangeTime;

  /** Incidence angle (incidenceAngle) */
  double incidenceAngle;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static InfoSceneCoord FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct SARParam
 *
 * \brief SAR sensors parameters
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT SARParam
{ 
  /** Azimuth Frequency Modulation (FM) rate list.
   * contains an entry for each azimuth FM rate update made along azimuth.
   */
  std::vector<AzimuthFmRate> azimuthFmRates;

  MetaData::Duration azimuthTimeInterval;
  double nearRangeTime;
  double rangeSamplingRate;
  double rangeResolution;

  unsigned long numberOfLinesPerBurst;
  unsigned long numberOfSamplesPerBurst;

  double azimuthBandwidth = 0.;
  double rangeBandwidth = 0.;
  double azimuthSteeringRate = 0.;

  bool rightLookingFlag = true;

  /** Doppler centroid estimates */
  std::vector<DopplerCentroid> dopplerCentroids;

  /** List of orbit information */
  std::vector<Orbit> orbits;

  /** List of burst records */
  std::vector<BurstRecord> burstRecords;

  /** map between GCP ids and corresponding azimuth and range times */
  std::unordered_map<std::string, GCPTime> gcpTimes;

  /** Conversion coefficients from slant range to ground range */
  std::vector<CoordinateConversionRecord> slantRangeToGroundRangeRecords;

  /** Conversion coefficients from ground range to slant range */
  std::vector<CoordinateConversionRecord> groundRangeToSlantRangeRecords;

  /** Scene coordinate */
  InfoSceneCoord centerSceneCoord;
  InfoSceneCoord ulSceneCoord;
  InfoSceneCoord urSceneCoord;
  InfoSceneCoord lrSceneCoord;
  InfoSceneCoord llSceneCoord;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const;

  /** Keywordlist import */
  void FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix);

  // Equality comparison operator (hidden friend idiom)
  friend bool operator==(const SARParam & lhs, const SARParam & rhs)
  {
    MetaData::Keywordlist lhs_kwl;
    lhs.ToKeywordlist(lhs_kwl, "");
    MetaData::Keywordlist rhs_kwl;
    rhs.ToKeywordlist(rhs_kwl, "");
    return lhs_kwl == rhs_kwl;
  }
};

/** \struct SARCalib
 *
 * \brief SAR calibration LUTs
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT SARCalib
{
  using PointSetType   = itk::PointSet<double, 2>;
  using ArrayType      = std::array<int, 2>;
  
  bool calibrationLookupFlag = false;
  double rescalingFactor;
  MetaData::TimePoint calibrationStartTime;
  MetaData::TimePoint calibrationStopTime;
  ArrayType radiometricCalibrationNoisePolynomialDegree;
  ArrayType radiometricCalibrationAntennaPatternNewGainPolynomialDegree;
  ArrayType radiometricCalibrationAntennaPatternOldGainPolynomialDegree;
  ArrayType radiometricCalibrationIncidenceAnglePolynomialDegree;
  ArrayType radiometricCalibrationRangeSpreadLossPolynomialDegree;
  PointSetType::Pointer radiometricCalibrationNoise = PointSetType::New();
  PointSetType::Pointer radiometricCalibrationAntennaPatternNewGain = PointSetType::New();
  PointSetType::Pointer radiometricCalibrationAntennaPatternOldGain = PointSetType::New();
  PointSetType::Pointer radiometricCalibrationIncidenceAngle = PointSetType::New();
  PointSetType::Pointer radiometricCalibrationRangeSpreadLoss = PointSetType::New();
  std::unordered_map<short, SarCalibrationLookupData::Pointer> calibrationLookupData;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const;
  /** Keywordlist import */
  void FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix);
};

} // end namespace otb

#endif
