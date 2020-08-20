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

#ifndef otbGeometryMetadata_h
#define otbGeometryMetadata_h

#include "OTBMetadataExport.h"
#include "otbMetaDataKey.h"


#include <string>
#include <vector>
#include <sstream>

namespace otb
{

/** \class GCP
 *
 * \brief This GCP class is used to manage the GCP parameters
 * in OTB.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT GCP
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

  GCP() = default;
  GCP(std::string id, std::string info, double col, double row, double px, double py, double pz);

  void Print(std::ostream& os) const;
  std::string ToJSON(bool multiline=false) const;
};

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

namespace Projection
{

/** \struct GCPParam
 *
 * \brief This structure handles the list of the GCP parameters
 */
struct OTBMetadata_EXPORT GCPParam
{
  std::string GCPProjection;

  std::vector<GCP> GCPs;

  // JSON export
  std::string ToJSON(bool multiline=false) const;
};

/** \struct RPCParam
 *
 * \brief Coefficients for RPC model (quite similar to GDALRPCInfo)
 *
 * Details of the mathematical model:
 *
 *    x = (longitude - LonOffset) / LonScale
 *    y = (latitude - LatOffset) / LatScale
 *    z = (Height - HeightOffset) / HeightScale
 *    rn = (Row - LineOffset) / LineScale
 *    cn = (Column - SampleOffset) / SampleScale
 *
 *    rn = LineNum(x,y,z) / LineDen(x,y,z)
 *    cn = SampleNum(x,y,z) / SampleDen(x,y,z)
 *
 *    Each cubic polynomial function LineNum, LineDen, SampleNum, SampleDen has
 *    20 coefficients:
 *
 *    c1 +                                                        (order 0)
 *    c2*x + c3*y + c4*z +                                        (order 1)
 *    c5*x*y + c6*x*z + c7*y*z + c8*x^2 + c9*y^2 + c10*z^2 +      (order 2)
 *    c11*x*y*z + c12*x^3 + c13*x*y^2 + c14*x*z^2 + c15*x^2*y +
 *    c16*y^3 + c17*y*z^2 + c18*x^2*z + c19*y^2*z + c20*z^3       (order 3)
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT RPCParam
{
  // Offsets
  double LineOffset = 0.0;
  double SampleOffset = 0.0;
  double LatOffset = 0.0;
  double LonOffset = 0.0;
  double HeightOffset = 0.0;

  // Scales
  double LineScale = 0.0;
  double SampleScale = 0.0;
  double LatScale = 0.0;
  double LonScale = 0.0;
  double HeightScale = 0.0;

  // Line numerator coefficients
  double LineNum[20] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  // Line denominator coefficients
  double LineDen[20] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // Sample numerator coefficients
  double SampleNum[20] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // Sample denominator coefficients
  double SampleDen[20] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // JSON export
  std::string ToJSON(bool multiline=false) const;
  inline static std::string doubleArrayToString(const double* array)
  {
    std::ostringstream oss;
    oss << "[";
    for (int loop = 0 ; loop < 20  ; loop++)
    	oss << " \"" << array[loop] << "\", ";
    oss << "]";
    return oss.str();
  };

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

} // end namespace Projection

} // end namespace otb

#endif

