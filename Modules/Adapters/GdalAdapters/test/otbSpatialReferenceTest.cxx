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
#include <stdexcept>

using namespace otb;

bool CheckUTMZone(double lat, double lon, unsigned int expected_zone, SpatialReference::hemisphere expected_hem)
{
  unsigned int                 zone;
  SpatialReference::hemisphere hem;
  SpatialReference::UTMFromGeoPoint(lon, lat, zone, hem);

  if (expected_zone != zone || expected_hem != hem)
  {
    std::cerr << "For (" << lat << ", " << lon << "), expecting zone " << expected_zone << expected_hem << ", got " << zone << hem << "\n";
    return false;
  }
  return true;
}

int otbSpatialReferenceTest(int, char* [])
{
  bool success = true;

  try
  {
    auto             sr         = SpatialReference::FromWGS84();
    auto             sr4        = SpatialReference::FromDescription("EPSG:32631");
    auto             srFromEPSG = SpatialReference::FromEPSG(32631);
    auto             srFromUTM  = SpatialReference::FromUTM(31, SpatialReference::hemisphere::north);
    SpatialReference sr2(sr);
    auto             sr3 = sr;

    if (sr4 == sr)
    {
      std::cerr << "Fail: sr4 != sr" << std::endl;
      success = false;
    }

    if (!(sr2 == sr))
    {
      std::cerr << "Fail: sr2 == sr" << std::endl;
      success = false;
    }

    if (sr3 != sr)
    {
      std::cerr << "Fail: sr3 == sr" << std::endl;
      success = false;
    }

    if (sr4 != srFromUTM)
    {
      std::cerr << "Fail: sr4 == srFromUTM" << std::endl;
      success = false;
    }

    if (sr4 != srFromEPSG)
    {
      std::cerr << "Fail: sr4 == srFromEPSG" << std::endl;
      success = false;
    }

    std::cout << "FromWGS84(): " << sr << std::endl << std::endl;
    std::cout << "FromDescription(EPSG::32631): " << sr4 << std::endl << std::endl;
    std::cout << "FromEPSG(32631): " << srFromEPSG << std::endl << std::endl;
    std::cout << "FromUTM(31,SpatialReference::hemisphere::north): " << srFromUTM << std::endl << std::endl;

    if (!sr.NormalizeESRI())
    {
      std::cerr << "Fail: NormalizeESRI() returns error code" << std::endl;
      success = false;
    }

    int epsg_utm31n = sr4.ToEPSG();

    if (epsg_utm31n != 32631)
    {
      std::cerr << "Fail: EPSG code for sr4 should be 32631" << std::endl;
      success = false;
    }
  }
  catch (std::runtime_error&)
  {
    std::cerr << "Fail: Constructor should not throw with valid description EPSG:4326 and EPSG:32631" << std::endl;
    success = false;
  }

  // Lat and lon of Toulouse, France, in UTM31N
  success = success & CheckUTMZone(43.60426, 1.44367, 31, SpatialReference::hemisphere::north);

  // NYC
  success = success & CheckUTMZone(40.70516, -74.01331, 18, SpatialReference::hemisphere::north);

  // Norway
  success = success & CheckUTMZone(60.400929, 3.972600, 32, SpatialReference::hemisphere::north);

  // Nowrway corner case (zone 31 V)
  success = success & CheckUTMZone(60.308054, 1.045553, 31, SpatialReference::hemisphere::north);

  // Special case zone 33
  success = success & CheckUTMZone(74.8, 19.2, 33, SpatialReference::hemisphere::north);

  // Special case zone 36
  success = success & CheckUTMZone(71.9833, 30.0024, 36, SpatialReference::hemisphere::north);

  // Case in south hemisphere
  success = success & CheckUTMZone(-80.003, 179.951, 60, SpatialReference::hemisphere::south);

  try
  {
    SpatialReference sr5 = SpatialReference::FromDescription("dummy");
    // Wrong EPSG
    SpatialReference sr6 = SpatialReference::FromEPSG(1000000);
    // Wrong utm zone
    SpatialReference sr7 = SpatialReference::FromUTM(1000, SpatialReference::hemisphere::south);
    std::cerr << "Fail: Calling constructor with wrong description should throw" << std::endl;
    success = false;
  }
  catch (std::runtime_error&)
  {
  }

  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
