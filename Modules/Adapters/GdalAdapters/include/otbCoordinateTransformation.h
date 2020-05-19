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
#ifndef otbCoordinateTransformation_h
#define otbCoordinateTransformation_h

#include "otbSpatialReference.h"

#include "OTBGdalAdaptersExport.h"

#include <memory>
#include <tuple>

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
// Disable following warning :
// warning C4251: 'otb::CoordinateTransformation::m_Transform':
// class 'std::unique_ptr<OGRCoordinateTransformation,std::default_delete<_Ty>>'
// needs to have dll-interface to be used by clients of class
// 'otb::CoordinateTransformation'
// As long as otb::CoordinateTransformation::m_Transform is private no need to
// export this type.
#endif

class OGRCoordinateTransformation;

namespace otb
{
// Destructor of OGRCoordinateTransformation
namespace internal
{
struct OTBGdalAdapters_EXPORT OGRCoordinateTransformationDeleter
{
public:
  void operator()(OGRCoordinateTransformation* del) const;
};
}

// Forward declaration needed for the operators declared bellow.
class CoordinateTransformation;

/// equal operator
OTBGdalAdapters_EXPORT bool operator==(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;

/// different operator
OTBGdalAdapters_EXPORT bool operator!=(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;

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
  OTBGdalAdapters_EXPORT friend bool operator==(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;
  OTBGdalAdapters_EXPORT friend bool operator!=(const CoordinateTransformation& ct1, const CoordinateTransformation& ct2) noexcept;

public:
  typedef std::unique_ptr<OGRCoordinateTransformation, internal::OGRCoordinateTransformationDeleter> CoordinateTransformationPtr;
  /**
   * Builds a coordinate transformation out of source and target
   * spatial reference systems.
   * \param source The source spatial reference
   * \param target The target spatial reference
   *
   * \throws InvalidCoordinateTransfromationException in case of failure
   */
  CoordinateTransformation(const SpatialReference& source, const SpatialReference& destination);

  /// Copy constructor
  CoordinateTransformation(const CoordinateTransformation& other);

  /// Asignment operator
  CoordinateTransformation& operator=(const CoordinateTransformation& other) noexcept;

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
  std::tuple<double, double, double> Transform(const std::tuple<double, double, double>& in) const;

  /**
   * Transform a 2D point from source to target spatial reference
   * \param input coords as a 2 double tuple
   * \return output coords as a 2 double tuple
   * \throws TransformFailureException if transform failed
   */
  std::tuple<double, double> Transform(const std::tuple<double, double>& in) const;


private:
  // unique ptr to the internal OGRCoordinateTransformation
  CoordinateTransformationPtr m_Transform;
};

/// Stream operator for CoordinateTransformation
OTBGdalAdapters_EXPORT std::ostream& operator<<(std::ostream& o, const CoordinateTransformation& i);
}

#endif
