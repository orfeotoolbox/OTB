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

#ifndef otbSarSensorModel_h
#define otbSarSensorModel_h

#include "otbImageMetadata.h"
#include "otbSARMetadata.h"

#include "itkPoint.h"

namespace otb
{

class SarSensorModel
{
public:
  SarSensorModel(const ImageMetadata & imd);
  virtual ~SarSensorModel() = default;

  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;

  using Vector3DType = itk::Point<double, 3>;

  using TimeType = MetaData::TimeType;
  using DurationType = MetaData::DurationType;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  void WorldToLineSample(const Point3DType& inGeoPoint,
                         Point2DType& outLineSample) const;

  bool WorldToAzimuthRangeTime(const Point3DType& inGeoPoint, 
                                          TimeType & azimuthTime, 
                                          double & rangeTime, 
                                          Point3DType& sensorPos, 
                                          Vector3DType& sensorVel) const;

  void LineSampleHeightToWorld(const Point2DType& imPt,
                               double heightAboveEllipsoid,
                               Point3DType& worldPt) const;

protected:

private:
  void OptimizeTimeOffsetsFromGcps();


  bool ZeroDopplerLookup(const Point3DType& inEcefPoint, 
                                          TimeType & azimuthTime, 
                                          Point3DType& sensorPos, 
                                          Vector3DType& sensorVel) const;


   /**
    * Interpolate sensor position and velocity at given azimuth time
    * using lagragian interpolation of orbital records.
    *
    * \param[in] azimuthTime The time at which to interpolate
    * \param[out] sensorPos Interpolated sensor position
    * \param[out] sensorvel Interpolated sensor velocity
    * \param[in] deg Degree of lagragian interpolation
    */
   void interpolateSensorPosVel(const TimeType & azimuthTime, 
                                Point3DType& sensorPos, 
                                Vector3DType& sensorVel, 
                                unsigned int deg = 8) const;

   /**
    * Convert azimuth time to fractional line.
    *
    * \param[in] azimuthTime The azimuth time to convert
    * \param[out] The estimated fractional line
    */
  void AzimuthTimeToLine(const TimeType & azimuthTime, 
                          double & line) const;

  void SlantRangeToGroundRange(double slantRange, 
                               const TimeType & azimuthTime, 
                               double & groundRange) const;


  void ApplyCoordinateConversion(double in,
                                 const TimeType& azimuthTime,
                                 const std::vector<CoordinateConversionRecord> & records,
                                 double & out) const;


  const GCP & findClosestGCP(const Point2DType& imPt, const Projection::GCPParam & gcpParam) const;

  void projToSurface(const GCP & gcp,
                     const Point2DType & imPt,
                     double heightAboveEllipsoid,
                     Point3DType & ecefPoint) const;

  const ImageMetadata & m_Imd;
  SARParam m_SarParam;

  DurationType m_AzimuthTimeOffset;
  double m_RangeTimeOffset; // Offset in seconds

  // Speed of light 
  const double C = 299792458;

  // True if the input product is a ground product
  bool m_IsGrd;
};

}

#endif