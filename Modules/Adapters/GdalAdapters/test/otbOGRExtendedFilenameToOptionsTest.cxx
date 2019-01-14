/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbOGRExtendedFilenameToOptions.h"
#include <algorithm>
#include <iostream>
#include <fstream>


using namespace otb;


typedef OGRExtendedFilenameToOptions FilenameHelperType;

int otbOGRExtendedFileName(int , char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputExtendedFilename  = argv[1];
  const char * outputFilename = argv[2];

  std::cout<< argv[1] <<"  "<<argv[2]<<std::endl;
  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  helper->SetExtendedFileName(inputExtendedFilename);

  std::ofstream file;
  file.open(outputFilename);

  file << helper->SimpleFileNameIsSet() << std::endl;
  file << helper->GetSimpleFileName() << std::endl;

  file << "Open option :"<<std::endl;
  FilenameHelperType::GDALOptionType open = helper->GetGDALOpenOptions();
  for ( auto option : open )
    {
    file<< option << std::endl;
    }

  file << "Create option :"<<std::endl;
  FilenameHelperType::GDALOptionType create = helper->GetGDALOptions("creation");
  for ( auto option : create )
    {
    file<< option << std::endl;
    }

  file << "Layer option :"<<std::endl;
  FilenameHelperType::GDALOptionType layer = helper->GetGDALOptions("layer");
  for ( auto option : layer )
    {
    file<< option << std::endl;
    }

  file<< "End of classic helper."<<std::endl;

  layer.push_back("TOTO=first");
  FilenameHelperType::Pointer layerHelper = 
    FilenameHelperType::GetGDALLayerOptionsHelper ( layer );
  std::cout<< layerHelper->GetGDALLayerOptions()[0] <<std::endl;
  FilenameHelperType::GDALOptionType newOptions;
    // std::vector< std::string> newOptions;
  newOptions.push_back("TOTO=second");
  newOptions.push_back("TiTi=option");
  layerHelper->AddGDALLayerOptions( newOptions );

  file << layerHelper->SimpleFileNameIsSet() << std::endl;
  file << layerHelper->HasGDALLayerOption() << std::endl;
  file << "Layer option from layer helper:"<<std::endl;
  FilenameHelperType::GDALOptionType latestOptions = layerHelper->GetGDALOptions("layer");
  // need to sort for dummy windows
  std::sort( latestOptions.begin() , latestOptions.end() );
  for ( auto option : latestOptions ) 
    {
    file<< option << std::endl;
    }

  file.close();
  return EXIT_SUCCESS;
}

