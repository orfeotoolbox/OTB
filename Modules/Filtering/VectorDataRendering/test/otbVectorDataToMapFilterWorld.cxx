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


#include "otbVectorDataFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include <fstream>
#include <iostream>

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorDataToMapFilter.h"

int otbVectorDataToMapFilterWorld(int argc, char * argv [])
{

  if (argc < 11)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename>"
              << " <output vector filename> "
              << " <sizeX> <sizeY> "
              << " <origin lon> <origin lat> "
              << " <spacing lon> <spacing lat> "
              << " <font filename>"
              << std::endl;
    return EXIT_FAILURE;
    }

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;

  std::string outputProjRef("GEOGCS[\"GCS_WGS_1984\", DATUM[\"D_WGS_1984\", "
    "SPHEROID[\"WGS_1984\", 6378137, 298.257223563]], PRIMEM[\"Greenwich\", 0],"
    " UNIT[\"Degree\", 0.017453292519943295]]");

  VectorDataFileReaderType::Pointer reader0 = VectorDataFileReaderType::New();
  reader0->SetFileName(argv[1]);
  ProjectionFilterType::Pointer projection0 = ProjectionFilterType::New();
  projection0->SetInput(reader0->GetOutput());
  projection0->SetOutputProjectionRef(outputProjRef);

  VectorDataFileReaderType::Pointer reader1 = VectorDataFileReaderType::New();
  reader1->SetFileName(argv[2]);
  ProjectionFilterType::Pointer projection1 = ProjectionFilterType::New();
  projection1->SetInput(reader1->GetOutput());
  projection1->SetOutputProjectionRef(outputProjRef);

  //Convert the vector data into an image
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  ImageType::SizeType size;
  size[0] = atoi(argv[4]);
  size[1] = atoi(argv[5]);

  ImageType::PointType origin;
  origin[0] = atof(argv[6]);
  origin[1] = atof(argv[7]);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[8]);
  spacing[1] = atof(argv[9]);

  typedef otb::VectorDataToMapFilter<VectorDataType, ImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();
  vectorDataRendering->SetInput(0, projection0->GetOutput());
  vectorDataRendering->SetInput(1, projection1->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetFontFileName(argv[10]);
  vectorDataRendering->AddStyle("world");
  vectorDataRendering->AddStyle("city");
  vectorDataRendering->UseAsOverlayOff();

  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  return EXIT_SUCCESS;
}
