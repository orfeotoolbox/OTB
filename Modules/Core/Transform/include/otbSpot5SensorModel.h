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

#ifndef otbSpot5SensorModel_h
#define otbSpot5SensorModel_h

#include "otbImageMetadata.h"
#include "otbSpot5Metadata.h"
#include "otbGeocentricTransform.h"
#include "otbBilinearProjection.h"
#include "otbPolygon.h"


#include "itkPoint.h"
#include <itkMatrix.h>
#include "itkVector.h"
#include <itkVersor.h>

namespace otb
{

class Spot5SensorModel
{

  /**
   * @brief SPOT5 sensor class, based on OSSIM Spot5SensorModel with ITK/GDAL implementation
   */

public:


  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;
  using MatrixType = itk::Matrix<double, 3, 3>;
  using Vector3DType = itk::Vector<double, 3>;
  using PolygonType = otb::Polygon<>;
  using ContinuousIndexType = PolygonType::ContinuousIndexType;
  using TimeType = MetaData::TimePoint;
  using DurationType = MetaData::Duration;

  Spot5SensorModel(const std::string & productType, const Spot5Param & Spot5Param);

  Spot5SensorModel(const ImageMetadata & imd);
  virtual ~Spot5SensorModel() = default;

  Spot5SensorModel(const Spot5SensorModel&) = delete; // non construction-copyable
  Spot5SensorModel& operator=(const Spot5SensorModel&) = delete; // non copyable


  /**
   * @brief Transform world point (lat,lon,hgt) to image point (col,row).
   * 
   * @param[in] inGeoPoint     ground point in WGS84 (lat, lon, hgt) coordinates
   * @return ground point converted to image point (col,row) coordinates
   */
  Point2DType WorldToLineSample(const Point3DType& inGeoPoint) const;

  /**
   * @brief Transform image point (col, row) with a height to world point (lat,lon,hgt).
   * 
   * @param[in] imPt                 image point (col, row)
   * @param[in] heightAboveEllipsoid height 
   * @return image point and height converted to world point (lat,lon,hgt)
   */
  Point3DType LineSampleHeightToWorld(Point2DType imPt,
                                      double heightAboveEllipsoid) const;

  /**
   * @brief Transform image point (col, row) to world point (lat,lon,hgt).
   * 
   * @param[in] imPt     image point (col, row)
   * @return image point converted to world point (lat,lon,hgt)
   */
  Point3DType LineSampleToWorld(Point2DType imPt) const;

  /**
    * @brief Compute a ray from sensor position and image point.
    * 
    * @param[in] imPt  image point (col row)
    * @return ephemeris point (3D space point with position and velocity)
    */
  Ephemeris ImagingRay(Point2DType imPt) const;

  /**
   * @brief Compute the nearest intersection (world point) between the ray and the ellipsoid.
   * 
   * @param[in] imRay     ephemeris point (3D space point with position and velocity)
   * @param[in] offset    double offset
   * @return  world point (lat,lon,hgt)
   */
  Point3DType NearestIntersection(const Ephemeris& imRay, double offset) const;

  /**
   * @brief Compute world point intersected by image ray.
   * 
   * @param[in] imRay     ephemeris point (3D space point with position and velocity)
   * @return world point (lat,lon,hgt)
   */
  Point3DType IntersectRay(const Ephemeris& imRay) const;

  /**
   * @brief Compute the bilinear interpolation from a 3D point vector with a double time vector.
   * 
   * @param[in] time time to interpolate
   * @param[in] V    3D point vector
   * @param[in] T    Index time double vector
   * @return 3D point interpolated
   */
  Point3DType GetBilinearInterpolation(const double& time,
                                       const std::vector<Point3DType>& V,
                                       const std::vector<double>& T) const;

  /**
  * @brief Compute the lagrange interpolation from a 3D point vector with a double time vector.
  * 
  * @param[in] time time to interpolate
  * @param[in] V    3D point vector
  * @param[in] T    Index time double vector
  * @return 3D point interpolated
  */
  Point3DType GetLagrangeInterpolation(const double& time,
                                       const std::vector<Point3DType>& V,
                                       const std::vector<double>& T) const;

  /**
   * @brief Get the 3D position of the sensor at time (interpolation of position samples vector from metadata).
   * 
   * @param[in] time  input time
   * @return 3D position
   */
  Point3DType GetPositionEcf(double time) const;

  /**
   * @brief Get the 3D velocity of the sensor at time (interpolation of velocity samples vector from metadata).
   * 
   * @param[in]  time input time
   * @return 3D position
   */
  Point3DType GetVelocityEcf(double time) const;

  /**
   * @brief Get look angles on X and Y axis of the sensor at line (interpolation of angles samples vector from metadata).
   * 
   * @param[in] line  input line
   * @param[out] psiX X angle
   * @param[out] psiY Y angle
  */
  void GetPixelLookAngleXY(unsigned int line, double& psiX, double& psiY) const;

  /**
   * @brief Get the Attitude of the sensor at time (interpolation of attitude samples vector from metadata).
   * 
   * @param[in] time input time
   * @return 3D attitude
   */
  Point3DType GetAttitude(double time) const;

  /**
   * @brief Compute SatToOrb matrix with an input time.
   * 
   * @param[in] t       input time
   * @return 3X3 matrix computed
   */
  MatrixType ComputeSatToOrbRotation(double t) const;


  /** 
  * @brief Update a ImageMetadata object with the stored Spot5Param and GCPs, possibly modified from the 
  * original metadata by the Spot5SensorModel
  * 
  * @param[in] imd ImageMetadata to be updated
  */
  void UpdateImageMetadata(ImageMetadata & imd);


protected:

private:

  void InitBilinearTransform();

  /** */

  /**
   * @brief Coordinate transformation from ECEF to geographic 
   * 
   * @param ecefPoint              3D position ephemeris coordinates
   * @return itk::Point<double, 3> 3D position wgs84
   */
  itk::Point<double, 3> EcefToWorld(const itk::Point<double, 3> & ecefPoint) const;


  /**
   * @brief Coordinate transformation from geographic to ECEF
   * 
   * @param worldPoint              3D position wgs84
   * @return itk::Point<double, 3>  3D position ephemeris coordinates
   */
  itk::Point<double, 3> WorldToEcef(const itk::Point<double, 3> & worldPoint) const;

  /**
   * @brief Convert 2Dpoint to 2DContinuousIndex
   *  
   * @param point                2Dpoint
   * @return ContinuousIndexType ContinuousIndex
   */
  ContinuousIndexType Point2DToIndex(const itk::Point<double, 2> point) const;

  /**
   * @brief Convert 3Dpoint to 2DContinuousIndex
   * 
   * @param point                3Dpoint
   * @return ContinuousIndexType ContinuousIndex
   */
  ContinuousIndexType Point3DToIndex(const itk::Point<double, 3> point) const;


  /**
   * @brief Check if point is inside image 
   * 
   * @param point   2D point
   * @param epsilon offset value
   * @return true   is inside
   * @return false  not inside
   */
  virtual bool insideImage(const itk::Point<double, 2> point, double epsilon) const
  {
      return (point[0] >= -epsilon) &&
      (point[0] <= (m_Spot5Param.ImageSize[0]+epsilon-1)) &&
      (point[1] >= -epsilon) &&
      (point[1] <= (m_Spot5Param.ImageSize[1]+epsilon-1));
  }

  std::string m_ProductType;
  Spot5Param m_Spot5Param;

  // Speed of light 
  static constexpr double C = 299792458;

  // Bilinear transform
  BilinearProjection::Pointer m_BilinearProj;

  // Image and ground polygon
  PolygonType::Pointer m_ImageRect; 
  PolygonType::Pointer m_GroundRect;

  otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::Pointer m_EcefToWorldTransform;
  otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::Pointer m_WorldToEcefTransform;
};

}

#endif
