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

namespace otb {

std::ostream & operator << (std::ostream& o, const SpatialReference & i)
{
  return o << i.ToWkt();
}

std::ostream & operator << (std::ostream& o, const SpatialReference::hemisphere & hem)
{
  return o << (hem == SpatialReference::hemisphere::north ? "N" : "S");
}


bool operator==(const SpatialReference& sr1,const SpatialReference& sr2) noexcept
{
  return sr1.m_SR->IsSame(sr2.m_SR.get());
}

bool operator!=(const SpatialReference& sr1,const SpatialReference& sr2) noexcept
{
  return !(sr1==sr2);
}

InvalidSRDescriptionException::InvalidSRDescriptionException(const std::string & description) : std::runtime_error(description) {};

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
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());
  bool north = true;

  switch(hem)
    {
    case hemisphere::north:
      break;
    case hemisphere::south:
      north = false;
      break;
    }
  
  OGRErr code = tmpSR->SetUTM(zone,north);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss << "FromUTM(" << zone <<", "<<hem<<")";
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

void SpatialReference::UTMFromGeoPoint(double lat, double lon, unsigned int & zone, hemisphere & hem)
{
  // Pre-conditions
  assert(lat>=-90);
  assert(lon>=-180);
  assert(lat<=90);
  assert(lon<=180);
  
  zone = 0;
  hem = lat>0 ? hemisphere::north : hemisphere::south;

  // TODO: Code forked from OSSIM. Copyright ?
  int lat_Degrees  = static_cast<int>(lat + 0.00000005);
  int long_Degrees = static_cast<int>(lon + 0.00000005);
  
  if (lon < 180)
     zone = static_cast<int>( (31 + (lon / 6) )+ 0.00000005);
   else
     zone = static_cast<int>( ((lon / 6) - 29) + 0.00000005);
   if (zone > 60)
     zone = 1;
   /* UTM special cases */
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > -1)
       && (long_Degrees < 3))
     zone = 31;
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > 2)
       && (long_Degrees < 12))
     zone = 32;
   if ((lat_Degrees > 71) && (long_Degrees > -1) && (long_Degrees < 9))
     zone = 31;
   if ((lat_Degrees > 71) && (long_Degrees > 8) && (long_Degrees < 21))
     zone = 33;
   if ((lat_Degrees > 71) && (long_Degrees > 20) && (long_Degrees < 33))
     zone = 35;
   if ((lat_Degrees > 71) && (long_Degrees > 32) && (long_Degrees < 42))
     zone = 37;

   // post conditions
   assert(zone>=0);
   assert(zone<=60);
}
}
