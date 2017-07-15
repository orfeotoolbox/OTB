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
#include "otbGeometriesSet.h"
#include "otbOGRDataSourceWrapper.h"

class CopyGeometryFunctor
{
public:
  typedef OGRGeometry TransformedElementType;

  otb::ogr::UniqueGeometryPtr operator()(OGRGeometry const* in) const
    {
    return otb::ogr::UniqueGeometryPtr(in->clone());
    }
};

int main(int argc, char **argv)
{
  if (argc < 3)
    {
    std::cerr << "otbGeometriesFilter  input_vectors  output_vectors \n";
    return EXIT_FAILURE;
    }
  otb::ogr::DataSource::Pointer ds = otb::ogr::DataSource::New(argv[1]);
  otb::ogr::DataSource::Pointer outDs = otb::ogr::DataSource::New(
    argv[2], otb::ogr::DataSource::Modes::Overwrite);

  otb::GeometriesSet::Pointer input = otb::GeometriesSet::New(ds);
  otb::GeometriesSet::Pointer output = otb::GeometriesSet::New(outDs);

  typedef otb::DefaultGeometriesToGeometriesFilter<CopyGeometryFunctor> FilterType;
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(input);
  filter->SetOutput(output);

  output->UpdateOutputInformation();
  otb::GeometriesRegion largest = output->GetLargestPossibleRegion();
  std::cout << "Output region : " << largest << std::endl;

  otb::GeometriesRegion request;
  request.SetStartId(2);
  request.SetCount(3);
  output->SetRequestedRegion(request);
  output->PropagateRequestedRegion();

  std::cout << "Input requested region : " << input->GetRequestedRegion() << std::endl;

  otb::GeometriesRegion largest2;
  largest2.SetMode(otb::GeometriesRegion::SPATIAL);
  input->SetLargestPossibleRegion(largest2);
  output->UpdateOutputInformation();

  largest = output->GetLargestPossibleRegion();
  std::cout << "Output region (spatial) : " << largest << std::endl;

  return EXIT_SUCCESS;
}
