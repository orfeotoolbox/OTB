/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesToGeometriesFilter.h"
#include <string>
#include <iostream>
#include "otbOGRDataSourceWrapper.h"
#include "otbGeometriesSet.h"

/*===========================================================================*/
/*=============================[ SwapXYFunctor ]=============================*/
/*===========================================================================*/
struct SwapXYFunctor
{
  typedef OGRGeometry TransformedElementType;
  otb::ogr::UniqueGeometryPtr operator()(OGRGeometry const* in) const
    {
    otb::ogr::UniqueGeometryPtr out(in ? in->clone() : 0);
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
      workingInplace ? otb::ogr::DataSource::Modes::write : otb::ogr::DataSource::Modes::read);

    otb::ogr::DataSource::Pointer output
      = workingInplace ? input
      : outputIsStdout ? 0
      :                  otb::ogr::DataSource::New( outputFile, otb::ogr::DataSource::Modes::write);
    std::cout << "input: " << input -> ogr().GetName() << " should be: " << inputFile << "\n";
    if (output)
      {
      std::cout << "output: " << output -> ogr().GetName() << " should be: " << outputFile << "\n";
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
      filter->UpdateOutputData(0);
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
