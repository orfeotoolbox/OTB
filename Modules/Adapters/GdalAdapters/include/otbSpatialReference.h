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

#include <memory>
#include <stdexcept>

class OGRSpatialReference;

namespace otb
{
/**
 * \class InvalidSRDescriptionException
 * \brief Exception for invalid spatial reference description
 *
 * This class describes an exception that might be thrown by
 * SpatialReference constructors
 * 
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT InvalidSRDescriptionException : public std::runtime_error
{
public:
  InvalidSRDescriptionException(const std::string & description);
  virtual ~InvalidSRDescriptionException() = default;
};

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
 * method. There are no public constructors (appart from copy and
 * assignment)
 * 
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT SpatialReference
{
friend class CoordinateTransformation;
friend bool operator==(const SpatialReference& sr1, const SpatialReference & sr2) noexcept;
friend bool operator!=(const SpatialReference& sr1, const SpatialReference & sr2) noexcept;

public:
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
  static SpatialReference FromDescription(const std::string & sr_description);

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
   * \param north true for northern hemisphere, false otherwise
   *
   * \throws InvalidSRDescriptionException in case of failure of
   * setUTM()
   */
  static SpatialReference FromUTM(unsigned int zone, bool north);
  
  /// Default destructor
  ~SpatialReference() noexcept;

  /// Copy constructor
  SpatialReference(const SpatialReference& other) noexcept;

  /// Asignment operator
  SpatialReference & operator=(const SpatialReference& other) noexcept;
  
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

  /**
   * Find which UTM zone a given (lat,lon) point falls in.
   * \pre -90<=lat<=90
   * \pre -180<=lon<=180
   * \param lat Point lattitude
   * \param lon Point longitude
   * \param zone Output UTM zone
   * \param north Output hemisphere (true if north, false if south)
   */ 
  static void UTMFromGeoPoint(double lat, double lon, unsigned int & zone, bool & north);

private:
  /// Constructor from wrapped type. ref will be cloned.
  SpatialReference(const OGRSpatialReference * ref);

  /// Constructor from unique_ptr to wrapped type. On success (no
  /// throw) the passed unique_ptr will be cleared, and ownership transferred
  SpatialReference(std::unique_ptr<OGRSpatialReference> ref);
  
  // unique ptr to the internal OGRSpatialReference
  std::unique_ptr<OGRSpatialReference> m_SR;
};

OTBGdalAdapters_EXPORT std::ostream & operator << (std::ostream& o, const SpatialReference & i);

/// Equal operator (based on OGRSpatialReference::IsSame())
 OTBGdalAdapters_EXPORT bool operator==(const SpatialReference& sr1, const SpatialReference & sr2) noexcept;
  
/// Different operator (based on OGRSpatialReference::IsSame())
 OTBGdalAdapters_EXPORT bool operator!=(const SpatialReference& sr1,const SpatialReference& sr2) noexcept;
}

#endif
