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
#include "otbImageFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataExtractROI.h"
#include <fstream>
#include <iostream>

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorDataToImageFilter.h"

int otbVectorDataToImageFilter(int argc, char * argv[])
{

  if (argc < 4)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename> <font filename>"  << std::endl;

    return EXIT_FAILURE;
    }

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());

  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  projection->SetOutputProjectionRef(projectionRefWkt);

  //Convert the vector data into an image
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

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

  typedef otb::VectorDataToImageFilter<VectorDataType, ImageType> VectorDataToImageFilterType;
  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();
  vectorDataRendering->SetInput(extractROI->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetFontFileName(argv[3]);
  vectorDataRendering->AddStyle("minor-roads-casing");
  vectorDataRendering->AddStyle("roads-text");

  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbVectorDataToImageFilterBinary(int argc, char * argv[])
{

  if (argc < 3)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename>"  << std::endl;

    return EXIT_FAILURE;
    }

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());

  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  projection->SetOutputProjectionRef(projectionRefWkt);

  //Convert the vector data into an image
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

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

  typedef otb::VectorDataToImageFilter<VectorDataType, ImageType> VectorDataToImageFilterType;
  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();
  vectorDataRendering->SetInput(extractROI->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetRenderingStyleType(VectorDataToImageFilterType::Binary);

  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
