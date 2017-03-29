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



#include "itkRGBPixel.h"
#include "otbImageFileReader.h"
#include "otbTileMapImageIO.h"
#include "otbTileMapTransform.h"
#include "otbInverseSensorModel.h"
#include "otbExtractROI.h"
#include "otbImageFileWriter.h"

int otbTileMapImageIOTest(int argc, char* argv[])
{

  if (argc != 7)
    {
    std::cout << argv[0] << " <inputFilename> <outputFilename> "
              << "<cacheDirectory> <lon> <lat> <depth>"
              << std::endl;

    return EXIT_FAILURE;
    }

  char * inputFilename = argv[1];
  char * outputFilename = argv[2];
  char * cacheDirectory = argv[3];
  double lon = atof(argv[4]);
  double lat = atof(argv[5]);
  int    depth = atoi(argv[6]);

  typedef itk::RGBPixel<unsigned char>    RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::TileMapImageIO             ImageIOType;

  ImageIOType::Pointer tileIO = ImageIOType::New();
  ReaderType::Pointer  readerTile = ReaderType::New();
  tileIO->SetDepth(depth);
  tileIO->SetCacheDirectory(cacheDirectory);
  readerTile->SetImageIO(tileIO);
  readerTile->SetFileName(inputFilename);
  readerTile->UpdateOutputInformation();

  typedef otb::TileMapTransform<otb::TransformDirection::FORWARD> TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetDepth(depth);

  typedef itk::Point <double, 2> PointType;
  PointType lonLatPoint;
  lonLatPoint[0] = lon;
  lonLatPoint[1] = lat;

  PointType tilePoint;
  tilePoint = transform->TransformPoint(lonLatPoint);

  long int startX = static_cast<long int>(tilePoint[0]);
  long int startY = static_cast<long int>(tilePoint[1]);
  long int sizeX = 500;
  long int sizeY = 500;

  std::cerr << startX << ", " << startY << std::endl;
  std::cerr << sizeX << ", " << sizeY << std::endl;

  typedef otb::ExtractROI<RGBPixelType,  RGBPixelType> ExtractROIFilterType;
  ExtractROIFilterType::Pointer extractROIOsmFilter = ExtractROIFilterType::New();
  extractROIOsmFilter->SetStartX(startX - sizeX / 2);
  extractROIOsmFilter->SetStartY(startY - sizeY / 2);
  extractROIOsmFilter->SetSizeX(sizeX);
  extractROIOsmFilter->SetSizeY(sizeY);

  extractROIOsmFilter->SetInput(readerTile->GetOutput());

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(extractROIOsmFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
