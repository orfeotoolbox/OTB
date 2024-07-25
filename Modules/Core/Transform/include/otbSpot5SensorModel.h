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
public:

  Spot5SensorModel(const std::string & productType, const Spot5Param & Spot5Param);

  Spot5SensorModel(const ImageMetadata & imd);
  virtual ~Spot5SensorModel() = default;

  Spot5SensorModel(const Spot5SensorModel&) = delete; // non construction-copyable
  Spot5SensorModel& operator=(const Spot5SensorModel&) = delete; // non copyable

  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;
  using MatrixType = itk::Matrix<double, 3, 3>;
  using Vector3DType = itk::Vector<double, 3>;
  using PolygonType = otb::Polygon<>;
  using ContinuousIndexType = PolygonType::ContinuousIndexType;


  using TimeType = MetaData::TimePoint;
  using DurationType = MetaData::Duration;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  void WorldToLineSample(const Point3DType& inGeoPoint,
                         Point2DType& outLineSample) const;


  void LineSampleHeightToWorld(const Point2DType& imPt,
                               double heightAboveEllipsoid,
                               Point3DType& worldPt) const;


  void LineSampleToWorld(const Point2DType& imPt,
                         Point3DType& worldPt) const;

  /*
   * Given an image point, returns a ray originating at some arbitrarily high
   * point (ideally at the sensor position) and pointing towards the target.
  */
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

  void InitBilinearTransform();

  /** Coordinate transformation from ECEF to geographic */
  itk::Point<double, 3> EcefToWorld(const itk::Point<double, 3> & ecefPoint) const;

  /** Coordinate transformation from geographic to ECEF */
  itk::Point<double, 3> WorldToEcef(const itk::Point<double, 3> & worldPoint) const;

  ContinuousIndexType Point2DToIndex(const itk::Point<double, 2> point) const;
  ContinuousIndexType Point3DToIndex(const itk::Point<double, 3> point) const;

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