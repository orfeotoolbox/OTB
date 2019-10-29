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
#ifndef otbSpatialReference_h
#define otbSpatialReference_h

#include "OTBGdalAdaptersExport.h"
#include "ogr_spatialref.h"

#include <memory>
#include <string>

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
// Disable following warning :
// warning C4251: 'otb::SpatialReference::m_SR':
// class 'std::unique_ptr<OGRSpatialReference,OGRSpatialReferenceDeleter>' needs
// to have dll-interface to be used by clients of class 'otb::SpatialReference'
// As long as otb::SpatialReference::m_SR is private no need to export this type.
#endif
class OGRSpatialReference;

namespace otb
{
// Destructor of OGRSpatialReference
namespace internal
{
struct OTBGdalAdapters_EXPORT OGRSpatialReferenceDeleter
{
public:
  void operator()(OGRSpatialReference* del) const;
};
}

// Forward declaration needed for the operators declared bellow.
class SpatialReference;

/// Equal operator (based on OGRSpatialReference::IsSame())
OTBGdalAdapters_EXPORT bool operator==(const SpatialReference& sr1, const SpatialReference& sr2) noexcept;

/// Different operator (based on OGRSpatialReference::IsSame())
OTBGdalAdapters_EXPORT bool operator!=(const SpatialReference& sr1, const SpatialReference& sr2) noexcept;

/**
 * \class SpatialReference
 * \brief This class is a wrapper around OGRSpatialReference
 *
 * This class is a wrapper around OGRSpatialReference. It aims at
 * manipulating spatial reference within OTB,  in a safe and easy way.
 * The class provides several constructors that all enforce the RAII:
 * either they fail or they provide a definitive, valid object.
 *
 * Building a SpatialReference requires to call one of the From*()
 * method. There are no public constructors (apart from copy and
 * assignment)
 *
 * \ingroup OTBGdalAdapters
 */

class OTBGdalAdapters_EXPORT SpatialReference
{
  friend class CoordinateTransformation;
  OTBGdalAdapters_EXPORT friend bool operator==(const SpatialReference& sr1, const SpatialReference& sr2) noexcept;
  OTBGdalAdapters_EXPORT friend bool operator!=(const SpatialReference& sr1, const SpatialReference& sr2) noexcept;

public:
  typedef std::unique_ptr<OGRSpatialReference, internal::OGRSpatialReferenceDeleter> OGRSpatialReferencePtr;
  /**
   * Build a SpatialRereference from a description string. The description string is
   * passed to OGRSpatialReference:SetFromUserInput()
   * from GDAL. Currently, supported syntax is:
   * - Well Known Text definition
   * - "EPSG:n" and "EPSGA:n" form EPSG codes
   * - PROJ definitions
   * - WMS auto projections ...
   * \param description a string containing the description of the
   * spatial reference to parse
   * \throws InvalidSRDescriptionException in case of failure of SetFromUserInput()
   */
  static SpatialReference FromDescription(const std::string& sr_description);

  /**
   * Build a SpatialRereference for wgs84
   * \throws InvalidSRDescriptionException (very unlikely since there is
   * a standard method in gdal to build a WGS84 projection)
   */
  static SpatialReference FromWGS84();

  /**
   *  Build a SpatialReference from an epsg code
   * \param epsg EPSG code passed to
   * OGRSpatialReference::importFromEPSGA() from GDAL
   *
   * \throws InvalidSRDescriptionException in case of failure of
   * importFromEPSGA()
   */
  static SpatialReference FromEPSG(unsigned int epsg);

  /**
   * Build a SpatialReference from a UTM zone passed to
   * OGRSpatialReference::SetUTM() from GDAL
   * \param zone UTM zone to use
   * \param hem hemisphere::north or hemisphere::south
   *
   * \throws InvalidSRDescriptionException in case of failure of
   * setUTM()
   */
  enum class hemisphere
  {
    north,
    south
  };

  static SpatialReference FromUTM(unsigned int zone, hemisphere hem);

  /// Copy constructor
  SpatialReference(const SpatialReference& other) noexcept;

  /// Asignment operator
  SpatialReference& operator=(const SpatialReference& other) noexcept;

  /**
   * Export the spatial reference to a Well Known Text string
   * \return A string containing the Well Known Text description
   */
  std::string ToWkt() const;

  /**
   * Try to normalize spatial reference fields to be compatible with
   * ESRI.
   * \returns True on success. Not modified if false is returned
   */
  bool NormalizeESRI();

  /**
   * Convert the spatial reference to an EPSG code
   * \returns If no EPSG code was found or the EPSG code
   */
  unsigned int ToEPSG() const;

#if GDAL_VERSION_NUM >= 3000000
  /** Set the Axis mapping strategy
   * proxy to the eponym ogr spatial reference method
   * \param strategy Axis mapping stategy
   */
  void SetAxisMappingStrategy(OSRAxisMappingStrategy strategy);
#endif

  /**
   * Find which UTM zone a given (lat,lon) point falls in.
   * \pre -180<=lon<=180
   * \pre -90<=lat<=90
   * \param lon Point longitude
   * \param lat Point latitude
   * \param zone Output UTM zone
   * \param hem output hemisphere
   */
  static void UTMFromGeoPoint(double lon, double lat, unsigned int& zone, hemisphere& hem);

private:
  /// Constructor from wrapped type. ref will be cloned.
  SpatialReference(const OGRSpatialReference* ref);

  /// Constructor from unique_ptr to wrapped type. On success (no
  /// throw) the passed unique_ptr will be cleared, and ownership transferred
  SpatialReference(OGRSpatialReferencePtr ref);

  // unique ptr to the internal OGRSpatialReference
  OGRSpatialReferencePtr m_SR;
};

/// Stream operator for hemisphere
OTBGdalAdapters_EXPORT std::ostream& operator<<(std::ostream& o, const SpatialReference::hemisphere& hem);

/// Stream operator for SpatialReference
OTBGdalAdapters_EXPORT std::ostream& operator<<(std::ostream& o, const SpatialReference& i);
}
#endif
