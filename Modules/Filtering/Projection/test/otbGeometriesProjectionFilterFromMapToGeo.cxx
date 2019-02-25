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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGeoInformationConversion.h"

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/
int otbGeometriesProjectionFilterFromMapToGeo(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << argv[0] << " <input vector filename> <output vector filename>\n";
    return EXIT_FAILURE;
    }

  // Input Geometries set
  typedef otb::GeometriesSet                InputGeometriesType;
  typedef otb::GeometriesSet                OutputGeometriesType;
  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
    argv[1], otb::ogr::DataSource::Modes::Read);
  InputGeometriesType::Pointer in_set = InputGeometriesType::New(input);

  // Output Geometries Set
  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
    argv[2], otb::ogr::DataSource::Modes::Overwrite);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  // Filter
  typedef otb::GeometriesProjectionFilter   GeometriesFilterType;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();
  filter->SetInput(in_set);
  filter->SetOutput(out_set);
  filter->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));

  filter->Update();

  return EXIT_SUCCESS;
}
