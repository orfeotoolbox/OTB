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

#include "otbOGRSpatialReferenceAdapter.h"

#include "ogr_spatialref.h"

#include "boost/lexical_cast.hpp"

#include <sstream>

namespace otb {

std::ostream & operator << (std::ostream& o, const OGRSpatialReferenceAdapter & i)
{
  return o << i.ToWkt();
}

InvalidSRDescriptionException::InvalidSRDescriptionException(const std::string & description) : m_Description(description) {};

InvalidSRDescriptionException::~InvalidSRDescriptionException()
{}

const char * InvalidSRDescriptionException::what() const noexcept
{
  return m_Description.c_str();
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter()
{
  std::unique_ptr<OGRSpatialReference> tmpSR(OGRSpatialReference::GetWGS84SRS()->Clone());  

  if(!tmpSR)
    {
    throw InvalidSRDescriptionException("WGS84");
    }

  m_SR.swap(tmpSR);
}


OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const std::string & description)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code1 = tmpSR->SetFromUserInput(description.c_str());
  
  if(code1!=OGRERR_NONE)
    {
    throw InvalidSRDescriptionException(description);
    }

  m_SR.swap(tmpSR);
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const unsigned int & epsg)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code = tmpSR->importFromEPSGA(epsg);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss<<"EPSG:"<<epsg;
    throw InvalidSRDescriptionException(oss.str());
    }

  m_SR.swap(tmpSR);
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const unsigned int & zone, bool north)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());
  OGRErr code = tmpSR->SetUTM(zone,north);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss<<"UTM"<<zone<<(north?"N":"S");
    throw InvalidSRDescriptionException(oss.str());
    }

  m_SR.swap(tmpSR);

}

OGRSpatialReferenceAdapter::~OGRSpatialReferenceAdapter() noexcept
{}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const OGRSpatialReferenceAdapter & other) noexcept
  : m_SR(other.m_SR->Clone())
{}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const OGRSpatialReference * ref) noexcept
: m_SR(ref->Clone())
{}

OGRSpatialReferenceAdapter & OGRSpatialReferenceAdapter::operator=(const OGRSpatialReferenceAdapter& other) noexcept
{
  m_SR = std::unique_ptr<OGRSpatialReference>(other.m_SR->Clone());
  return *this;
}

bool OGRSpatialReferenceAdapter::operator==(const OGRSpatialReferenceAdapter& other) noexcept
{
  return m_SR->IsSame(other.m_SR.get());
}

bool OGRSpatialReferenceAdapter::operator!=(const OGRSpatialReferenceAdapter& other) noexcept
{
  return !(*this==other);
}

std::string OGRSpatialReferenceAdapter::ToWkt() const
{
  char * cwkt;
  m_SR->exportToWkt(&cwkt);
  std::string wkt(cwkt);
  delete cwkt;

  return wkt;
}

int OGRSpatialReferenceAdapter::ToEPSG() const
{
  int code = -1;
  
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
  if (epsg!=nullptr && strcmp( epsg, "" )!=0 )
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

bool OGRSpatialReferenceAdapter::NormalizeESRI()
{
  std::unique_ptr<OGRSpatialReference> tmpSRS(m_SR->Clone());

  // Morph to ESRI
  OGRErr code = tmpSRS->morphToESRI();

  // Check if it is still valid
  if(code != OGRERR_NONE || tmpSRS->Validate() != OGRERR_NONE)
    return false;

  m_SR.swap(tmpSRS);
  return true;
}

void OGRSpatialReferenceAdapter::UTMFromGeoPoint(const double & lat, const double & lon, unsigned int & zone, bool & north)
{
  // Pre-conditions
  assert(lat_Degrees>-90);
  assert(lon_Degrees>-180);
  assert(lat_Degrees<90);
  assert(lon_Degrees<180);
  
  zone = 0;
  north = lat>0;

  // TODO: Code forked from OSSIM. Copyright ?
  int lat_Degrees  = (int)(lat + 0.00000005);
  int long_Degrees = (int)(lon + 0.00000005);
  
  if (lon < 180)
     zone = (int)( (31 + (lon / (6 * 180)) )+ 0.00000005);
   else
     zone = (int)( ((lon / (6 * 180)) - 29) + 0.00000005);
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
