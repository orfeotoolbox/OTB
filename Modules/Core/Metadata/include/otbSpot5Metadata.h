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

#ifndef otbSpot5Metadata_h
#define otbSpot5Metadata_h

#include "itkPoint.h"
#include "itkPointSet.h"
#include "OTBMetadataExport.h"
#include "otbMetaDataKey.h"
#include "otbDateTime.h"

#include <string>
#include <vector>

namespace otb
{

/** \struct Ephemeris
 *
 * \brief This structure is used to handle Ephemeris information
 */
struct OTBMetadata_EXPORT Ephemeris
{
  using PointType = itk::Point<double, 3>;
  using VectorType = itk::Vector<double, 3>;

  /** Timestamp at which Ephemeris state vectors apply */
  //MetaData::TimePoint time;
  double time;

  /** Position vector */
  PointType position;
  /** Velocity vector */
  VectorType velocity;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix = "") const;

  /** Keywordlist import */
  static Ephemeris FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix = "");
};

/** \struct Spot5Param
 *
 * \brief Spot5 sensors parameters
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT Spot5Param
{ 
  // Constructors
  Spot5Param() = default;
  Spot5Param(const Spot5Param &) = default; // CopyConstructible required for boost::any
  Spot5Param& operator=(Spot5Param &) = default; //CopyAssignment optional for boost::any

  using Point3DType = itk::Point<double, 3>;
  using Point2DType = itk::Point<double, 2>;

  double RefLineTime;
  double LineSamplingPeriod;

  int RefLineTimeLine;

  Point2DType ImageSize; 
  Point2DType SubImageOffset;


  std::vector<Point3DType> AttitudesSamples;
  std::vector<double> AttitudesSamplesTimes;

  std::vector<double> PixelLookAngleX;
  std::vector<double> PixelLookAngleY;

  //std::vector<Ephemeris> EcefSamples;
  std::vector<Point3DType> EcefPosSamples;
  std::vector<Point3DType> EcefVelSamples;
  std::vector<double> EcefTimeSamples;
};

}

#endif
