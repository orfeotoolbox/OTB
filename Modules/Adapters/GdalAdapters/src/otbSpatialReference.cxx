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

#include "otbSpatialReference.h"

#include "ogr_spatialref.h"
#include "cpl_conv.h"

#include "boost/lexical_cast.hpp"

#include <sstream>

namespace otb
{

OTBGdalAdapters_EXPORT std::ostream & operator << (std::ostream& o, const SpatialReference & i)
{
  return o << i.ToWkt();
}

OTBGdalAdapters_EXPORT std::ostream & operator << (std::ostream& o, const SpatialReference::hemisphere & hem)
{
  return o << (hem == SpatialReference::hemisphere::north ? "N" : "S");
}


OTBGdalAdapters_EXPORT bool operator==(const SpatialReference& sr1,const SpatialReference& sr2) noexcept
{
  bool rawIsSame ( sr1.m_SR->IsSame(sr2.m_SR.get()) );

  // By default, gdal does not compare datum (and IsSame with
  // papzOptions is not in public API
  if(rawIsSame)
    {    
    const std::string datum1 = (sr1.m_SR->GetAttrValue("DATUM")?sr1.m_SR->GetAttrValue("DATUM"):"");
    const std::string datum2 = (sr2.m_SR->GetAttrValue("DATUM")?sr2.m_SR->GetAttrValue("DATUM"):"");

    // Either both are empty or they are equal
    if((datum1.empty() && datum2.empty()) || !datum1.compare(datum2))
      {
      return true;
      }
    }
  
  return false;
}

OTBGdalAdapters_EXPORT bool operator!=(const SpatialReference& sr1,const SpatialReference& sr2) noexcept
{
  return !(sr1==sr2);
}

SpatialReference::SpatialReference(const SpatialReference & other) noexcept
  : m_SR(other.m_SR->Clone())
{}

SpatialReference::SpatialReference(const OGRSpatialReference * ref)
{
  if(!ref)
    throw InvalidSRDescriptionException("Can not construct SpatialReference from null pointer");
  m_SR = std::unique_ptr<OGRSpatialReference>(ref->Clone());
}

SpatialReference::SpatialReference(std::unique_ptr<OGRSpatialReference> ref)
{
  if(!ref)
    throw InvalidSRDescriptionException("Can not construct SpatialReference from null pointer");

  // Move (will empty ref)
  m_SR = std::move(ref);
}

// Destructor
SpatialReference::~SpatialReference() noexcept {}

SpatialReference & SpatialReference::operator=(const SpatialReference& other) noexcept
{
  m_SR.reset(other.m_SR->Clone());
  return *this;
}


SpatialReference SpatialReference::FromWGS84()
{
  // GetWGS84SRS() is only avalaible since gdal 2.0, so we use the
  // epsg code instead
  return FromEPSG(4326);
}

SpatialReference SpatialReference::FromDescription(const std::string & description)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code1 = tmpSR->SetFromUserInput(description.c_str());
  
  if(code1!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss<<"FromDescription("<<description<<")";
    throw InvalidSRDescriptionException(description);
    }

  return SpatialReference(std::move(tmpSR));
}

SpatialReference SpatialReference::FromEPSG(unsigned int epsg)
{
  
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code = tmpSR->importFromEPSGA(epsg);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss << "FromEPSG("<< epsg<<")";
    throw InvalidSRDescriptionException(oss.str());
    }

  return SpatialReference(std::move(tmpSR));
}

SpatialReference SpatialReference::FromUTM(unsigned int zone, hemisphere hem)
{
  assert(zone<=60&&"UTM zone should be in range [0,60]");
  
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());
  
  // Build EPSG code from zone and hem
  // We prefer this upon the SetFromUTM() of the OGRSpatialReference
  // class because the latter does not set the datum and other useful fields.
  int epsg = 32600;

  if(hem == hemisphere::south)
    {
    epsg = 32700;
    }

  epsg+=zone;
  
  OGRErr code = tmpSR->importFromEPSGA(epsg);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss << "FromUTM(" << zone <<", "<<hem<<"), could not use generated EPSG code "<<epsg;
    throw InvalidSRDescriptionException(oss.str());
    }

  return SpatialReference(std::move(tmpSR));
}

std::string SpatialReference::ToWkt() const
{
  char * cwkt;
  m_SR->exportToWkt(&cwkt);
  std::string wkt(cwkt);
  // as recommanded in Gdal doc of exportToWkt()
  CPLFree(cwkt);

  return wkt;
}

unsigned int SpatialReference::ToEPSG() const
{
  unsigned int code = 0;
  
  std::unique_ptr<OGRSpatialReference> tmpSRS(m_SR->Clone());

  tmpSRS->Fixup();
  tmpSRS->AutoIdentifyEPSG();
  
  const char * epsg = nullptr;
  if (tmpSRS->IsGeographic())
    {
    code = 0;
    epsg = tmpSRS->GetAuthorityCode("GEOGCS");
    }
  else if (tmpSRS->IsProjected())
    {
    code = 0;
    epsg = tmpSRS->GetAuthorityCode("PROJCS");
    }
  
  if (epsg!=nullptr && std::string(epsg).compare("")!=0 )
    {
    try
      {
      code = boost::lexical_cast<int>(epsg);
      }
    catch(boost::bad_lexical_cast &)
      {
      code = 0;
      }
    }
  return code;
}

bool SpatialReference::NormalizeESRI()
{
  std::unique_ptr<OGRSpatialReference> tmpSRS(m_SR->Clone());

  // Morph to ESRI
  OGRErr code = tmpSRS->morphToESRI();

  // Check if it is still valid
  if(code != OGRERR_NONE || tmpSRS->Validate() != OGRERR_NONE)
    return false;

  m_SR = std::move(tmpSRS);
  return true;
}

void SpatialReference::UTMFromGeoPoint(double lon, double lat, unsigned int & zone, hemisphere & hem)
{
  // Pre-conditions
  assert(lat>=-90);
  assert(lon>=-180);
  assert(lat<=90);
  assert(lon<=180);

  // General expression
  zone = 1 + static_cast<unsigned int>((lon + 180) / 6);
  hem = lat>0 ? hemisphere::north : hemisphere::south;

  // Corner cases (from
  // https://github.com/owaremx/LatLngUTMConverter/blob/master/LatLngUTMConverter.cs#L107 )
  if (lon >= 8 && lon <= 13 && lat > 54.5 && lat < 58)
    {
    zone = 32;
    }
  else if (lat >= 56.0 && lat < 64.0 && lon >= 3.0 && lon < 12.0)
    {
    zone = 32;
    }
  else if (lat >= 72.0 && lat < 84.0)
    {
    if (lon >= 0.0 && lon < 9.0)
      {
      zone = 31;
      }
    else if (lon >= 9.0 && lon < 21.0)
      {
      zone = 33;
      }
    else if (lon >= 21.0 && lon < 33.0)
      {
      zone = 35;
      }
    else if (lon >= 33.0 && lon < 42.0)
      {
      zone = 37;
      }
    }

  // post conditions
  assert(zone<=60);
}
}
