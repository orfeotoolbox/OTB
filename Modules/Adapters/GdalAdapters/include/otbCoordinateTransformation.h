/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbCoordinateTransformation_h
#define otbCoordinateTransformation_h

#include "otbSpatialReference.h"

#include "OTBGdalAdaptersExport.h"

#include <memory>
#include <stdexcept>
#include <tuple>


class OGRCoordinateTransformation;

namespace otb
{

/**
 * \class InvalidCoordinateTransfromationException
 * \brief Exception for invalid coordinate transform
 *
 * This class describes an exception that might be thrown by
 * CoordinateTransformation constructors
 * 
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT InvalidCoordinateTransfromationException : public std::runtime_error
{
public:
  InvalidCoordinateTransfromationException(const std::string & description);
  virtual ~InvalidCoordinateTransfromationException() = default;
};

/**
 * \class TransformFailureException
 * \brief Exception for failure of coordinate transform
 *
 * This class describes an exception that might be thrown by
 * CoordinateTransformation::Transform()
 * 
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT TransformFailureException : public std::runtime_error
{
public:
  TransformFailureException(const std::string & description);
  virtual ~TransformFailureException() = default;
};



/**
 * \class CoordinateTransformation
 * \brief This class is a wrapper around OGRCoordinateTransformation
 * 
 * This class is a wrapper around OGRCoordinateTransformation. It aims
 * at manipulating coordinate transformations between spatial
 * reference systems within OTB, in a safe and easy way. The class
 * constructors enforce RAII: either they fail or they provide a
 * definitive, valid object.
 *
 * \ingroup OTBGdalAdapters 
 */

class OTBGdalAdapters_EXPORT CoordinateTransformation
{
friend bool operator==(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;
friend bool operator!=(const CoordinateTransformation& ct1, const CoordinateTransformation & ct2) noexcept;

public:
  /** 
   * Builds a coordinate transformation out of source and target
   * spatial reference systems.
   * \param source The source spatial reference
   * \param target The target spatial reference
   *
   * \throws InvalidCoordinateTransfromationException in case of failure
   */
  CoordinateTransformation(const SpatialReference & source, const SpatialReference & destination);

  /// Destructor
  ~CoordinateTransformation() noexcept;

  /// Copy constructor
  CoordinateTransformation(const CoordinateTransformation& other);

  /// Asignment operator
  CoordinateTransformation & operator=(const CoordinateTransformation& other) noexcept;

  /// \return The source spatial reference
  SpatialReference GetSourceSpatialReference() const;

  /// \return The target spatial reference
  SpatialReference GetTargetSpatialReference() const;

  /**
   * Transform a 3D point from source to target spatial reference
   * \param input coords as a 3 double tuple
   * \return output coords as a 3 double tuple
   * \throws TransformFailureException if transform failed
   */
  std::tuple<double,double,double> Transform(const std::tuple<double,double,double> & in) const;

  /**
   * Transform a 2D point from source to target spatial reference
   * \param input coords as a 2 double tuple
   * \return output coords as a 2 double tuple
   * \throws TransformFailureException if transform failed
   */
  std::tuple<double,double> Transform(const std::tuple<double,double> & in) const;

  
private:
  // unique ptr to the internal OGRCoordinateTransformation
  std::unique_ptr<OGRCoordinateTransformation> m_Transform;
};

OTBGdalAdapters_EXPORT std::ostream & operator << (std::ostream& o, const CoordinateTransformation & i);

  /// equal operator
OTBGdalAdapters_EXPORT bool operator==(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;

  /// different operator
OTBGdalAdapters_EXPORT bool operator!=(const CoordinateTransformation& ct1, const CoordinateTransformation & ct2) noexcept;

}

#endif
