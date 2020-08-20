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

#ifndef otbSARMetadata_h
#define otbSARMetadata_h

#include "OTBMetadataExport.h"
#include "otbMetaDataKey.h"


#include <string>
#include <vector>
#include <sstream>

namespace otb
{

/** \struct AzimuthFmRate
 *
 * \brief This structure is used to manage parameters
 * related to the Azimuth Frequency Modulation rate
 */
struct OTBMetadata_EXPORT AzimuthFmRate
{
  /** Zero Doppler azimuth time to which azimuth FM rate parameters apply */
  MetaData::Time azimuthTime;
  /** Two way slant range time origin used for azimuth FM rate calculation */
  double t0;
  /** Azimuth FM rate coefficients c0 c1 c2 */
  std::vector<double> azimuthFmRatePolynomial;
};

/** \struct CalibrationVector
 *
 * \brief This structure is used to handle calibration look up tables
 */
struct OTBMetadata_EXPORT CalibrationVector
{
  /** Image line at which the calibration vector applies */
  int line;
  /** Zero Doppler azimuth time at which calibration vector applies */
  MetaData::Time azimuthTime;
  /** Sigma nought calibration vector */
  MetaData::LUT1D sigmaNought;
  /* Beta nought calibration vector */
  MetaData::LUT1D betaNought;
  /* Gamma calibration vector */
  MetaData::LUT1D gamma;
  /* Digital number calibration vector */
  MetaData::LUT1D dn;
};

/** \struct DopplerCentroid
 *
 * \brief This structure is used to handle Doppler centroid estimates
 */
struct OTBMetadata_EXPORT DopplerCentroid
{
  /** Zero Doppler azimuth time of this Doppler centroid estimate */
  MetaData::Time azimuthTime;
  /* Two-way slant range time origin for Doppler centroid estimate */
  double t0;
  /* Doppler centroid estimated from data */
  std::vector<double> dopCoef;
  /* Doppler centroid estimated from orbit */
  std::vector<double> geoDopCoef;
};

/** \struct SARNoise
 *
 * \breif This structure is used to handle Noise look up tables
 */
struct OTBMetadata_EXPORT SARNoise
{
  /** Image line at which the noise vector applies */
  int line;
  /** Zero Doppler azimuth time at which noise vector applies */
  MetaData::Time azimuthTime;
  /** Noise look up talbe */
  MetaData::LUT1D noiseLut;
};

/** \struct Orbit
 *
 * \breif This structure is used to handle orbit information
 */
struct OTBMetadata_EXPORT Orbit
{
  /** Timestamp at which orbit state vectors apply */
  MetaData::Time time;
  /** Position vector */
  double posX;
  double posY;
  double posZ;
  /** Velocity vector */
  double velX;
  double velY;
  double velZ;
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

  /** Calibration vector list */
  std::vector<CalibrationVector> calibrationVectors;

  MetaData::Time calibrationStartTime;
  MetaData::Time calibrationStopTime;

  /** Doppler centroid estimates */
  std::vector<DopplerCentroid> dopplerCentroids;

  /** Noise look up tables */
  std::vector<SARNoise> noiseVector;

  /** List of orbit information */
  std::vector<Orbit> orbits;
};

} // end namespace otb

#endif

