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


#include "itkMacro.h"
#include "otbDEMHandler.h"

int otbDEMHandlerTest(int argc, char * argv[])
{
  if(argc!=9)
    {
    std::cerr<<"Usage: "<<argv[0]<<" demdir[path|no] geoid[path|no] defaultHeight longitude latitude aboveMSLFlag targetValue tolerance"<<std::endl;
    return EXIT_FAILURE;
    }


  std::string demdir   = argv[1];
  std::string geoid    = argv[2];
  double defaultHeight = atof(argv[3]);
  double longitude      = atof(argv[4]);
  double latitude     = atof(argv[5]);
  bool aboveMSL        = atoi(argv[6]);
  double target        = atof(argv[7]);
  double tolerance     = atof(argv[8]);

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  demHandler->SetDefaultHeightAboveEllipsoid(defaultHeight);

  bool fail = false;

  std::cout<<std::fixed;
  std::cout.precision(12);

  if(demdir != "no")
    {
    if(!demHandler->IsValidDEMDirectory(demdir.c_str()))
      {
      std::cerr<<"IsValidDEMDirectory("<<demdir<<") = false"<<std::endl;
      fail = true;
      }

    demHandler->OpenDEMDirectory(demdir);
    std::cout<<"GetDEMDirectory() = "<<demHandler->GetDEMDirectory()<<std::endl;
    }

  if(geoid != "no")
    {
    try
      {
      demHandler->OpenGeoidFile(geoid);
      }
    catch( const std::exception & exception )
      {
      std::cout
	<< "Exception thrown while opening geod-file '" << geoid << "':" << std::endl
	<< exception.what();

      fail = true;
      }

    std::cout<<"GetGeoidFile() = "<<demHandler->GetGeoidFile()<<std::endl;
    }

  std::cout<<"PrintSelf: "<<demHandler<<std::endl;

  otb::DEMHandler::PointType point;
  point[0] = longitude;
  point[1] = latitude;

  double height = -32768;

  if(aboveMSL)
    {
    height = demHandler->GetHeightAboveMSL(point);

    std::cout<<"height above MSL ("<<longitude<<", "<<latitude<<") = "<<height<<" meters"<<std::endl;
    }
  else
    {
    height = demHandler->GetHeightAboveEllipsoid(point);
    std::cout<<"height above ellipsoid ("<<longitude<<", "<<latitude<<") = "<<height<<" meters"<<std::endl;
    }

  // Check for Nan
  if(vnl_math_isnan(height))
    {
    std::cerr<<"Computed value is NaN"<<std::endl;
    fail = true;
    }

  double error = std::abs(height-target);

  if(error>tolerance)
    {
    std::cerr<<"Target value is "<<target<<" meters, computed value is "<<height<<" meters. error ("<<error<<" meters) > tolerance ("<<tolerance<<" meters)"<<std::endl;
    fail = true;
    }

  if(fail)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
