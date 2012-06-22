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
#include "itkMacro.h"
#include "otbOGR.h"
#include "otbGeometriesSet.h"
#include "otbGeometriesProjectionFilter.h"

int otbGeometriesProjectionFilterFromMapToEPSG(int argc, char * argv[])
{
  if (argc < 4)
    {
    std::cout << argv[0] << " <input vector filename> <output vector filename> <epsg>"  << std::endl;
    return EXIT_FAILURE;
    }

  const char* inputVDFilename = argv[1];
  const char* outputVDFilename = argv[2];
  int epsg = atoi(argv[3]);

  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;

  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
      inputVDFilename, otb::ogr::DataSource::Modes::Read);
  InputGeometriesType::Pointer in_set = InputGeometriesType::New(input);

  typedef otb::GeometriesProjectionFilter GeometriesFilterType;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();

  filter->SetInput(in_set);
  filter->SetOutputProjectionRef( otb::GeoInformationConversion::ToWKT(epsg) );

  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
      outputVDFilename, otb::ogr::DataSource::Modes::Overwrite);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  filter->SetOutput(out_set);
  filter->Update();

  return EXIT_SUCCESS;
}
