/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "itkMacro.h"

#include "otbDEMToImageGenerator.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbDEMToImageGeneratorTest(int argc, char* argv[])
{
  if (argc < 9)
  {
    std::cout
        << argv[0]
        << " folder path , output filename , Longitude Output Orign point , Latitude Output Origin point , X Output Size, Y Output size , X Spacing , Y Spacing"
        << std::endl;
    return EXIT_FAILURE;
  }

  char* folderPath = argv[1];
  char* outputName = argv[2];

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension> ImageType;
  typedef otb::DEMToImageGenerator<ImageType>  DEMToImageGeneratorType;
  typedef DEMToImageGeneratorType::PointType   PointType;
  typedef DEMToImageGeneratorType::SizeType    SizeType;
  typedef DEMToImageGeneratorType::SpacingType SpacingType;
  typedef otb::ImageFileWriter<ImageType>      WriterType;

  // Instantiating object
  auto object = DEMToImageGeneratorType::New();
  auto              writer = WriterType::New();

  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  otb::DEMHandler::GetInstance().OpenDEMDirectory(folderPath);

  object->SetOutputOrigin(origin);
  object->SetOutputSize(size);
  object->SetOutputSpacing(spacing);

  std::cout << object << std::endl;

  otb::StandardFilterWatcher watcher(object, "DEM to image generator");

  writer->SetFileName(outputName);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
