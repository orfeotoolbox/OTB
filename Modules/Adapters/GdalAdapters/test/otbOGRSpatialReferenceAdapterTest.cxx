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

#include <iostream>

using namespace otb;

int otbOGRSpatialReferenceAdapterTest(int, char**)
{
  bool success = true;
  
  try
    {
    OGRSpatialReferenceAdapter sr("EPSG:4326");
    OGRSpatialReferenceAdapter sr4("EPSG:32631");

    OGRSpatialReferenceAdapter sr2(sr);

    OGRSpatialReferenceAdapter sr3 = sr;

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
    
    std::cout<<sr<<std::endl;
    std::cout<<sr4<<std::endl;
    
    }
  catch(InvalidSRDescriptionException & err)
    {
    std::cerr<<"Fail: Constructor should not throw with valid description EPSG:4326 and EPSG:32631"<<std::endl;
    success = false;
    }
 
  try
    {
    OGRSpatialReferenceAdapter sr5("dummy");
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
