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

#include "cpl_error.h"
#include "otbOGRExtendedFilenameToOptions.h"
#include "otbOGRDataSourceWrapper.h"
#include <iostream>
#include <fstream>


int otbOGRExtendedFileNameGDAL(int , char* argv[])
{
  bool success = true ;
  const char * inputOpen = argv[1];
  const char * inputCreation  = argv[2];
  auto test = otb::ogr::DataSource::New( inputOpen );
  // std::cout<<"here"<<std::endl;
  std::string  error = CPLGetLastErrorMsg();
  if ( !error.empty() )
    std::cout<<error<<std::endl;
  // if ( error.find( "does not support open option openOption" ) != std::string::npos )
  //   success = true && success ;
  // else
  //   success = false ;
  // std::cout<<success<<std::endl;


  test = otb::ogr::DataSource::New( inputCreation , otb::ogr::DataSource::Modes::Update_LayerOverwrite);
error = CPLGetLastErrorMsg();
  if ( !error.empty() )
    std::cout<<error<<std::endl;
  // error = CPLGetLastErrorMsg();
  // std::cout<<error<<std::endl;
  // if ( std::string(error).find( "does not support creation option creationOption" ) != std::string::npos )
  //   success = true && success ;
  // else
  //   success = false ;
  // std::cout<<success<<std::endl;

  test->CreateLayer( "2layertest" , 
      ITK_NULLPTR , 
      wkbUnknown , 
      std::vector< std::string > { "newOption=OPTION" } );
error = CPLGetLastErrorMsg();
  if ( !error.empty() )
    std::cout<<error<<std::endl;
  // error = CPLGetLastErrorMsg();
  // std::cout<<error<<std::endl;
  // if ( std::string(error).find( "toto" ) != std::string::npos )
  //   success = true && success;
  // else
  //   success = false ;
  // std::cout<<success<<std::endl;

  return EXIT_SUCCESS;
}


