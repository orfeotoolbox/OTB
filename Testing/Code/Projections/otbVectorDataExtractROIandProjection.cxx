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

#include "otbVectorDataFileReader.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataExtractROI.h"
#include <fstream>
#include <iostream>

int otbVectorDataExtractROIandProjection(int itkNotUsed(argc), char * argv[])
{

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());
//   projection->Update();
  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  projection->SetOutputProjectionRef(projectionRefWkt);

  //Convert the vector data into an image
  typedef otb::Image<unsigned int, 2> ImageType;

  ImageType::SizeType size;
  size[0] = 500;
  size[1] = 500;

  ImageType::PointType origin;
  origin[0] = 374149.980555821; //UL easting
  origin[1] = 4829183.99443839; //UL northing

  ImageType::SpacingType spacing;
  spacing[0] = 0.6;
  spacing[1] = -0.6;

  typedef otb::RemoteSensingRegion<double> RegionType;
  RegionType           region;
  RegionType::SizeType sizeInUnit;
  sizeInUnit[0] = size[0] * spacing[0];
  sizeInUnit[1] = size[1] * spacing[1];
  region.SetSize(sizeInUnit);
  region.SetOrigin(origin);
  region.SetRegionProjection(projectionRefWkt);

  typedef otb::VectorDataExtractROI<VectorDataType> ExtractROIType;
  ExtractROIType::Pointer extractROI = ExtractROIType::New();
  extractROI->SetRegion(region);
  extractROI->SetInput(projection->GetOutput());
  extractROI->Update();

  unsigned int elementsKeptAfterProj = extractROI->GetOutput()->Size();
  std::cout << "After projection: kept " << elementsKeptAfterProj << " features." << std::endl;

  extractROI->SetInput(reader->GetOutput());
  extractROI->Update();

  unsigned int elementsKeptAfterReader = extractROI->GetOutput()->Size();
  std::cout << "Directly from reader: kept " << elementsKeptAfterReader << " features." << std::endl;

  if (elementsKeptAfterProj != elementsKeptAfterReader)
    {
    std::cout<<"Test supposed to fail: shows the error induced by extract+project regarding project+extract" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
