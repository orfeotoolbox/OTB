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

#include "cpl_error.h"
#include "otbOGRExtendedFilenameToOptions.h"
#include "otbOGRDataSourceWrapper.h"
#include <iostream>
#include <fstream>


int otbOGRExtendedFileNameGDALOpen(int, char* argv[])
{
  auto        test  = otb::ogr::DataSource::New(argv[1]);
  std::string error = CPLGetLastErrorMsg();
  return 0;
}

int otbOGRExtendedFileNameGDALCreate(int, char* argv[])
{
  auto        test  = otb::ogr::DataSource::New(argv[1], otb::ogr::DataSource::Modes::Overwrite);
  std::string error = CPLGetLastErrorMsg();
  if (error.find("does not support creation option creationOption"))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

int otbOGRExtendedFileNameGDALLayer(int, char* argv[])
{
  auto test = otb::ogr::DataSource::New(argv[1], otb::ogr::DataSource::Modes::Update_LayerOverwrite);
  test->CreateLayer("2layertest", nullptr, wkbUnknown);
  std::string error = CPLGetLastErrorMsg();
  if (error.find("does not support layer creation option layeroption"))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

int otbOGRExtendedFileNameGDALLayerOption(int, char* argv[])
{
  auto                     test = otb::ogr::DataSource::New(argv[1], otb::ogr::DataSource::Modes::Update_LayerOverwrite);
  std::vector<std::string> option{"vectorlayeroption=OPTION"};
  test->CreateLayer("2layertest", nullptr, wkbUnknown, option);
  std::string error = CPLGetLastErrorMsg();
  if (error.find("does not support layer creation option vectorlayeroption"))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
