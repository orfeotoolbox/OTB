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
#include "otbGeometriesProjectionFilter.h"
#include <fstream>
#include "otbGeometriesSet.h"
#include "otbStandardFilterWatcher.h"

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/
int otbGeometriesProjectionFilter(int argc, char * argv[])
{
  if (argc < 3)
    {
    std::cerr << argv[0] << " <input filename> <output filename> [<output text>]\n";
    return EXIT_FAILURE;
    }

  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;
  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
    argv[1], otb::ogr::DataSource::Modes::Read);
  InputGeometriesType::Pointer in_set = InputGeometriesType::New(input);

  typedef otb::GeometriesProjectionFilter GeometriesFilterType;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();

  filter->SetInput(in_set);

  const std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  filter->SetOutputProjectionRef(projectionRefWkt);

  otb::StandardFilterWatcher watcher(filter, "GeometriesProjection");

  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
    argv[2], otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  filter->SetOutput(out_set);
  filter->Update();

  //Output the tree in a text file
  if (argc >= 4)
    {
    std::ofstream file(argv[3]);

    file << "Original data\n" << in_set << "\n\n";

    file << "Reprojected data\n" << out_set << "\n\n";
    }

  return EXIT_SUCCESS;
}
