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
class OTBGdalAdapters_EXPORT InvalidSRDescriptionException : std::exception
{
public:
  InvalidSRDescriptionException(const std::string & description);
  virtual ~InvalidSRDescriptionException();
  virtual const char * what() const noexcept;

private:
  std::string m_Description;
};

/**
 * \class OGRSpatialReferenceAdapter
 * \brief This class is a wrapper around OGRSpatialReference
 * 
 * This class is a wrapper around OGRSpatialReference. It aims at
 * manipulating spatial reference within OTB. Constructor builds a
 * spatial reference with an Empty state.
 */

class OTBGdalAdapters_EXPORT OGRSpatialReferenceAdapter
{
friend class OGRCoordinateTransformationAdapter;

public:
  // Default constructor builds an reference (m_Empty == true)
  OGRSpatialReferenceAdapter(const std::string & sr_description);

  // Default destructor builds an reference (m_Empty == true)
  ~OGRSpatialReferenceAdapter() noexcept;

  // Copy constructor
  OGRSpatialReferenceAdapter(const OGRSpatialReferenceAdapter& other) noexcept;

  // Asignment operator
  OGRSpatialReferenceAdapter & operator=(const OGRSpatialReferenceAdapter& other) noexcept;

  // equal operator
  bool operator==(const OGRSpatialReferenceAdapter& other) noexcept;
  
  // equal operator
  bool operator!=(const OGRSpatialReferenceAdapter& other) noexcept;

  // Export to Wkt
  std::string ToWkt() const;

protected:
  // Constructor from wrapped type
  OGRSpatialReferenceAdapter(const OGRSpatialReference * ref) noexcept;

private:
  // unique ptr to the internal OGRSpatialReference
  std::unique_ptr<OGRSpatialReference> m_SR;
};

std::ostream & OTBGdalAdapters_EXPORT operator << (std::ostream& o, const OGRSpatialReferenceAdapter & i);
}

#endif
