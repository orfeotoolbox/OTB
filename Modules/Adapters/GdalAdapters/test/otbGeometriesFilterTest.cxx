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

#include "otbGeometriesToGeometriesFilter.h"
#include "otbOGRDataSourceWrapper.h"

int main(int argc, char **argv)
{
  if (argc < 3)
    {
    std::cerr << "otbGeometriesFilter  input_vectors  output_vectors \n";
    return EXIT_FAILURE;
    }
  otb::ogr::DataSource::Pointer ds = otb::ogr::DataSource::New(argv[1]);
  otb::ogr::DataSource::Pointer outDs = otb::ogr::DataSource::New(argv[2]);

  // TODO : create and use instead a GeometriesFunctorFilter class
  otb::GeometriesToGeometriesFilter::Pointer filter =
    otb::GeometriesToGeometriesFilter::New();

  filter->SetInput(ds);
  filter->SetOutput(outDs);

  outDs->UpdateOutputInformation();
  std::cout << "Output region : " << outDs->GetLargestPossibleRegion() << std::endl;
  
  return EXIT_SUCCESS;
}
