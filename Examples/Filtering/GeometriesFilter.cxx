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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesToGeometriesFilter.h"
#include <string>
#include <iostream>
#include "otbGeometriesSet.h"

/*===========================================================================*/
/*=============================[ SwapXYFunctor ]=============================*/
/*===========================================================================*/
struct SwapXYFunctor
{
  typedef OGRGeometry TransformedElementType;
  otb::ogr::UniqueGeometryPtr operator()(OGRGeometry const* in) const
    {
    otb::ogr::UniqueGeometryPtr out(in ? in->clone() : nullptr);
    if (out)
      {
#if GDAL_VERSION_NUM >= 1900
      out->swapXY();
#else
      assert(!"Sorry, This example filter requires GDAL v1.9.0 or later");
#endif
      }
    return otb::move(out);
    }
};

/*===========================================================================*/
/*=================================[ main ]==================================*/
/*===========================================================================*/
typedef otb::DefaultGeometriesToGeometriesFilter<SwapXYFunctor> FilterType;

int main (int argc, char **argv)
{
  if (argc < 2)
    {
    std::cerr << argv[0] << " inputGeometriesFile [outputGeometriesFile]" << std::endl;
    return EXIT_FAILURE;
    }
  try
    {
    const bool workingInplace = argc == 2;
    const bool outputIsStdout = !workingInplace && !strcmp(argv[2], "-");

    const std::string inputFile = argv[1];
    const std::string outputFile = workingInplace ? argv[1] : argv[2];

    otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
      inputFile,
      workingInplace ? otb::ogr::DataSource::Modes::Update_LayerOverwrite : otb::ogr::DataSource::Modes::Read);

    otb::ogr::DataSource::Pointer output
      = workingInplace ? input
      : outputIsStdout ? nullptr
      :                  otb::ogr::DataSource::New( outputFile, otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
    std::cout << "input: " << otb::ogr::GetFileListAsStringVector(&input->ogr())[0] << " should be: " << inputFile << "\n";
    if (output)
      {
      std::cout << "output: " << otb::ogr::GetFileListAsStringVector(&output->ogr())[0] << " should be: " << outputFile << "\n";
      }
    // std::cout << "\n";

    FilterType::Pointer filter = FilterType::New();
    if (!workingInplace)
      {
      otb::GeometriesSet::Pointer in_set = otb::GeometriesSet::New(input);
      filter->SetInput(in_set);
      }
    if (output)
      {
      otb::GeometriesSet::Pointer out_set = otb::GeometriesSet::New(output);
      filter->SetOutput(out_set);
      out_set->Update();
      }
    else
      {
      filter->UpdateOutputData(nullptr);
      assert(filter->GetOutput() && "output not set");
      filter->GetOutput()->Print(std::cout, 0);
      }

    return EXIT_SUCCESS;
    }
  catch (std::exception const& e)
    {
    std::cerr << e.what() << "\n";
    }
  return EXIT_FAILURE;
}
