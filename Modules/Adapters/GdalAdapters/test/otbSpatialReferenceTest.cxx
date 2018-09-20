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

#include <iostream>
#include <limits>

using namespace otb;

bool compareLatLong(double lat, double lon, const double lat_ref, const double lon_ref)
{
  return ( ( std::abs(lat_ref-lat) < std::numeric_limits<double>::epsilon() ) &&  ( std::abs(lon_ref-lon) < std::numeric_limits<double>::epsilon() ) );
}
bool UTMFromGeoPointList(double lat, double lon)
{
  unsigned int zone;
  SpatialReference::hemisphere hem;

  SpatialReference::UTMFromGeoPoint(lat,lon,zone,hem);

  // Toulouse 43.60426, 1.44367
  if (compareLatLong(lat, lon, 43.60426, 1.44367) )
    {
      return (zone == 31);
    }
  // New York 40.70516 -74.01331
  else if ( compareLatLong(lat, lon, 40.70516, -74.01331) )
    {
      return (zone == 18);
    }
  //North sea (zone 32)
  else if ( compareLatLong(lat, lon, 60.400929, 3.972600) )
    {
      return (zone == 32);
    }
  //Special case North sea (zone 31)
  else if ( compareLatLong(lat, lon, 60.308054, 1.045553) )
    {
      return (zone == 31);
    }
  else
    {
      //Coordinates not tabulate yet
      return false;
    }
}

int otbSpatialReferenceTest(int, char**)
{
  bool success = true;

  try
    {
    auto sr = SpatialReference::FromWGS84();
    auto sr4 = SpatialReference::FromDescription("EPSG:32631");
    auto srFromEPSG = SpatialReference::FromEPSG(32631);
    auto srFromUTM = SpatialReference::FromUTM(31,SpatialReference::hemisphere::north);
    SpatialReference sr2(sr);
    auto sr3 = sr;

    if(sr4 == sr)
      {
      std::cerr<<"Fail: sr4 != sr";
      success = false;
      }

    if(!(sr2 == sr))
      {
      std::cerr<<"Fail: sr2 == sr";
      success = false;
      }

    if(sr3 != sr)
      {
      std::cerr<<"Fail: sr3 == sr";
      success = false;
      }

    if(sr4 != srFromUTM)
      {
      std::cerr<<"Fail: sr4 == srFromUTM";
      success = false;
      }

    if(sr4 != srFromEPSG)
      {
      std::cerr<<"Fail: sr4 == srFromEPSG";
      success = false;
      }

    std::cout<<sr<<std::endl;
    std::cout<<sr4<<std::endl;

    if(!sr.NormalizeESRI())
      {
      std::cerr<<"Fail: NormalizeESRI() returns error code"<<std::endl;
      success = false;
      }

    int epsg_utm31n = sr4.ToEPSG();

    if(epsg_utm31n != 32631)
      {
      std::cerr<<"Fail: EPSG code for sr4 should be 32631"<<std::endl;
      success = false;
      }

    }
  catch(InvalidSRDescriptionException & err)
    {
    std::cerr<<"Fail: Constructor should not throw with valid description EPSG:4326 and EPSG:32631"<<std::endl;
    success = false;
    }

  unsigned int zone;
  SpatialReference::hemisphere hem;
  // Lat and lon of Toulouse, France, in UTM31N
  double lat = 43.60426;
  double lon = 1.44367;

  SpatialReference::UTMFromGeoPoint(lat,lon,zone,hem);

  if(zone!=31 || hem != SpatialReference::hemisphere::north)
    {
    std::cerr<<"Fail: Expected utm zone 31 N, got "<<zone<<hem<<std::endl;
    success = false;
    }

  // Test UTM Zone (not the hemispher) on some coordinates
  if (!UTMFromGeoPointList(lat, lon))
    {
      std::cerr<<"Fail: Expected utm zone 31N, got "<<zone<<hem<<std::endl;
      success = false;
    }

  //NYC
  lat =  40.70516;
  lon = -74.01331;
  SpatialReference::UTMFromGeoPoint(lat,lon,zone,hem);

  if (!UTMFromGeoPointList(lat, lon))
    {
      std::cerr<<"Fail: Expected utm zone 18, got "<<zone<<hem<<std::endl;
      success = false;
    }

  //Norway
  lat =  60.400929;
  lon = 3.972600;
  SpatialReference::UTMFromGeoPoint(lat,lon,zone,hem);

  if (!UTMFromGeoPointList(lat, lon))
    {
      std::cerr<<"Fail: Expected utm zone 32, got "<<zone<<hem<<std::endl;
      success = false;
    }

  //Nowrway corner case (zone 31 V)
  lat = 60.308054;
  lon = 1.045553;
  SpatialReference::UTMFromGeoPoint(lat,lon,zone,hem);
  if (!UTMFromGeoPointList(lat, lon))
    {
      std::cerr<<"Fail: Expected utm zone 31, got "<<zone<<hem<<std::endl;
      success = false;
    }

  try
    {
    SpatialReference sr5 = SpatialReference::FromDescription("dummy");
    // Wrong EPSG
    SpatialReference sr6 = SpatialReference::FromEPSG(1000000);
    // Wrong utm zone
    SpatialReference sr7 = SpatialReference::FromUTM(1000,SpatialReference::hemisphere::south);
    std::cerr<<"Fail: Calling constructor with wrong description should throw"<<std::endl;
    success = false;
    }
  catch(InvalidSRDescriptionException & err)
    {}

  if(success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
