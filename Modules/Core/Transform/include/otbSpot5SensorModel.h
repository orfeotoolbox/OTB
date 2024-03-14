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

#ifndef otbSpot5SensorModel_h
#define otbSpot5SensorModel_h

#include "otbImageMetadata.h"
#include "otbSpot5Metadata.h"
#include "otbGeocentricTransform.h"

#include "itkPoint.h"
#include <itkMatrix.h>
#include "itkVector.h"
#include <itkVersor.h>

namespace otb
{

class Spot5SensorModel
{
public:

  Spot5SensorModel(const std::string & productType,
                 const Spot5Param & Spot5Param,
                 const Projection::GCPParam & gcps);

  Spot5SensorModel(const ImageMetadata & imd);
  virtual ~Spot5SensorModel() = default;

  Spot5SensorModel(const Spot5SensorModel&) = delete; // non construction-copyable
  Spot5SensorModel& operator=(const Spot5SensorModel&) = delete; // non copyable

  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;
  using MatrixType = itk::Matrix<double, 3, 3>;
  using Vector3DType = itk::Vector<double, 3>;


  using TimeType = MetaData::TimePoint;
  using DurationType = MetaData::Duration;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  void WorldToLineSample(const Point3DType& inGeoPoint,
                         Point2DType& outLineSample) const;


  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) and YZ frame */
  // Alex useless
  //void WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const;

  /** Transform world point (lat,lon,hgt) to satellite position (x,y,z) and satellite velocity */
  bool WorldToSatPositionAndVelocity(const Point3DType& inGeoPoint, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  /** Transform line index to satellite position (x,y,z) and satellite velocity */
  bool LineToSatPositionAndVelocity(double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  bool WorldToAzimuthRangeTime(const Point3DType& inGeoPoint, 
                                          TimeType & azimuthTime, 
                                          double & rangeTime, 
                                          Point3DType& sensorPos, 
                                          Vector3DType& sensorVel) const;

  void LineSampleHeightToWorld(const Point2DType& imPt,
                               double heightAboveEllipsoid,
                               Point3DType& worldPt) const;


  void LineSampleToWorld(const Point2DType& imPt,
                         Point3DType& worldPt) const;

  /*! DOC OSSIM
   * Given an image point, returns a ray originating at some arbitrarily high
   * point (ideally at the sensor position) and pointing towards the target.
  */
  // Alex type ossimEcefRay contenant position et direction, remplace par ephemeris
  // TODO  changer par une meilleure struct
  void ImagingRay(const Point2DType& imPt,
                          Ephemeris&   epPt) const;

  bool NearestIntersection(const Ephemeris& imRay, const double& offset, Point3DType& worldPt) const;
                        
  void IntersectRay(const Ephemeris& imRay, Point3DType& worldPt, double defaultElevation = 0.0) const;


  void GetBilinearInterpolation(const double& time,
                          const std::vector<Point3DType>& V,
                          const std::vector<double>& T,
                          Point3DType& li) const;
  void GetLagrangeInterpolation(const double& time,
                          const std::vector<Point3DType>& V,
                          const std::vector<double>& T,
                          Point3DType& li) const;                        

  void GetPositionEcf(const double& time, Point3DType& ecef) const;
  void GetVelocityEcf(const double& time, Point3DType& ecef) const;
  void GetPixelLookAngleXY(unsigned int line, double& psiX, double& psiY) const;
  void GetAttitude(const double& time, Point3DType& at) const;
  void ComputeSatToOrbRotation(MatrixType& result, double t) const;

                        
   /** Update a ImageMetadata object with the stored Spot5Param and GCPs, possibly modified from the 
    * original metadata by the Spot5SensorModel
    * \param imd The ImageMetadata to be updated
     */ 
   void UpdateImageMetadata(ImageMetadata & imd);


protected:

private:

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

  Point3DType projToSurface(const GCP & gcp,
                            const Point2DType & imPt,
                            std::function<double(double, double)> heightFunction) const;

  void LineToAzimuthTime(double line, TimeType & azimuthTime) const;

  /** Coordinate transformation from ECEF to geographic */
  itk::Point<double, 3> EcefToWorld(const itk::Point<double, 3> & ecefPoint) const;

  /** Coordinate transformation from geographic to ECEF */
  itk::Point<double, 3> WorldToEcef(const itk::Point<double, 3> & worldPoint) const;


  std::string m_ProductType;
  Projection::GCPParam m_GCP;
  Spot5Param m_Spot5Param;

  TimeType m_FirstLineTime;
  TimeType m_LastLineTime;

  DurationType m_AzimuthTimeOffset;
  double m_RangeTimeOffset; // Offset in seconds

  // Speed of light 
  static constexpr double C = 299792458;

  // True if the input product is a ground product
  bool m_IsGrd;

  otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::Pointer m_EcefToWorldTransform;
  otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::Pointer m_WorldToEcefTransform;
};

}

#endif