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

using namespace otb;

int otbSpatialReferenceTest(int, char**)
{
  bool success = true;
  
  try
    {
    SpatialReference sr;
    SpatialReference sr4("EPSG:32631");
    SpatialReference srFromEPSG(32631);
    SpatialReference srFromUTM(31,true);
    SpatialReference sr2(sr);
    SpatialReference sr3 = sr;

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
  bool north;
  // Lat and lon of Toulouse, France, in UTM31N
  double lon = 43.60426;
  double lat = 1.44367;

  SpatialReference::UTMFromGeoPoint(lat,lon,zone,north);

  if(zone!=31 || !north)
    {
    std::cerr<<"Fail: Expected utm zone 31 N, got "<<zone<<(north?"N":"S")<<std::endl;
    success = false;
    }
  
  try
    {
    SpatialReference sr5("dummy");
    // Wrong EPSG
    SpatialReference sr6(1000000);
    // Wrong utm zone
    SpatialReference sr7(1000,false);
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
