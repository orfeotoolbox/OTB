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

#include "otbOGRCoordinateTransformationAdapter.h"

#include <iostream>

using namespace otb;

int otbOGRCoordinateTransformationAdapterTest(int, char**)
{
  bool success = true;
  
  OGRSpatialReferenceAdapter inSR("EPSG:4326");
  OGRSpatialReferenceAdapter outSR("EPSG:32631");
  OGRSpatialReferenceAdapter outSRBis("EPSG:2154");

  try
    {
    OGRCoordinateTransformationAdapter transformation(inSR,outSR);
    
    OGRCoordinateTransformationAdapter transformation2(transformation);
    OGRCoordinateTransformationAdapter transformation3 = transformation;
    OGRCoordinateTransformationAdapter transformation4(inSR,outSRBis);
    
  if(!(transformation==transformation2))
    {
    std::cerr<<"Fail: transformation and transformation2 should be equal"<<std::endl;
    success = false;
    }

  if(!(transformation==transformation3))
    {
    std::cerr<<"Fail: transformation and transformation3 should be equal"<<std::endl;
    success = false;
    }

  if(transformation==transformation4)
    {
    std::cerr<<"Fail: transformation and transformation4 should be different"<<std::endl;
    success = false;
    }

  if(transformation.GetSourceSpatialReference() != inSR)
    {
    std::cerr<<"Fail: SourceSpatialReference from transform different from input spatial reference"<<std::endl;
    success = false;
    }

  if(transformation.GetTargetSpatialReference() != outSR)
    {
    std::cerr<<"Fail: TargetSpatialReference from transform different from input spatial reference"<<std::endl;
    success = false;
    }
  
  std::cout<<transformation<<std::endl;

  double x = 43.60426;
  double y = 1.44367;
  double z = 300;

  double outX, outY, outZ;

  if(!transformation.Transform(x,y,z,outX,outY,outZ))
    {
    std::cerr<<"Fail: transformation call to 3D version of Transform() returned error"<<std::endl;
    success = false;
    }

  if(!transformation.Transform(x,y,outX,outY))
    {
    std::cerr<<"Fail: transformation call to 2D version of Transform() returned error"<<std::endl;
    success = false;
    }
  }
  catch(InvalidCoordinateTransfromationException & err)
    {
    std::cerr<<"Exception raised during test: "<<err.what()<<std::endl;
    success = false;
    }
  
  if(success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
