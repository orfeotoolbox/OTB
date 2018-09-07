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
#ifndef otbOGRSpatialReferenceAdapter_h
#define otbOGRSpatialReferenceAdapter_h

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
 * OGRSpatialReferenceAdapter constructors
 */
class OTBGdalAdapters_EXPORT InvalidSRDescriptionException : public std::runtime_error
{
public:
  InvalidSRDescriptionException(const std::string & description);
  virtual ~InvalidSRDescriptionException() = default;
};

/**
 * \class OGRSpatialReferenceAdapter
 * \brief This class is a wrapper around OGRSpatialReference
 * 
 * This class is a wrapper around OGRSpatialReference. It aims at
 * manipulating spatial reference within OTB,  in a safe and easy way.
 * The class provides several constructors that all enforce the RAII:
 * either they fail or they provide a definitive, valid object.
 */
class OTBGdalAdapters_EXPORT OGRSpatialReferenceAdapter
{
friend class OGRCoordinateTransformationAdapter;
friend bool operator==(const OGRSpatialReferenceAdapter& sr1, const OGRSpatialReferenceAdapter & sr2) noexcept;
friend bool operator!=(const OGRSpatialReferenceAdapter& sr1, const OGRSpatialReferenceAdapter & sr2) noexcept;

public:
/**
 * Default constructor builds a wgs84 spatial reference
 * \throws InvalidSRDescriptionException (very unlikely since there is
 * a standard method in gdal to build a WGS84 projection)
 */
OGRSpatialReferenceAdapter();

/**
 * Constructor from a description string. The description string is
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
  OGRSpatialReferenceAdapter(const std::string & sr_description);

  /**
   * Constructor from an epsg code
   * \param epsg EPSG code passed to
   * OGRSpatialReference::importFromEPSGA() from GDAL
   *
   * \throws InvalidSRDescriptionException in case of failure of
   * importFromEPSGA()
   */
  OGRSpatialReferenceAdapter(unsigned int epsg);

  /**
   * Constructor from a UTM zone passed to
   * OGRSpatialReference::SetUTM() from GDAL
   * \param zone UTM zone to use
   * \param north true for northern hemisphere, false otherwise
   *
   * \throws InvalidSRDescriptionException in case of failure of
   * setUTM()
   */
  OGRSpatialReferenceAdapter(unsigned int zone, bool north);
  
  /// Default destructor
  ~OGRSpatialReferenceAdapter() noexcept;

  /// Copy constructor
  OGRSpatialReferenceAdapter(const OGRSpatialReferenceAdapter& other) noexcept;

  /// Asignment operator
  OGRSpatialReferenceAdapter & operator=(const OGRSpatialReferenceAdapter& other) noexcept;
  
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
  /// Constructor from wrapped type
  OGRSpatialReferenceAdapter(const OGRSpatialReference * ref) noexcept;

  // unique ptr to the internal OGRSpatialReference
  std::unique_ptr<OGRSpatialReference> m_SR;
};

std::ostream & OTBGdalAdapters_EXPORT operator << (std::ostream& o, const OGRSpatialReferenceAdapter & i);

/// Equal operator (based on OGRSpatialReference::IsSame())
bool OTBGdalAdapters_EXPORT operator==(const OGRSpatialReferenceAdapter& sr1, const OGRSpatialReferenceAdapter & sr2) noexcept;
  
/// Different operator (based on OGRSpatialReference::IsSame())
bool OTBGdalAdapters_EXPORT operator!=(const OGRSpatialReferenceAdapter& sr1,const OGRSpatialReferenceAdapter& sr2) noexcept;
}

#endif
