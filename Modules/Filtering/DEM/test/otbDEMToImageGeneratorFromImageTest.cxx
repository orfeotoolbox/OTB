/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

#include "otbDEMToImageGenerator.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbExtractROI.h"

int otbDEMToImageGeneratorFromImageTest(int argc, char* argv[])
{
  std::cout << argc << std::endl;
  if ((argc != 9) && (argc != 4))
  {
    std::cout << argv[0] << " input filename, folder path, output filename 1, "
                            " (option) output filename 2,"
                            " (option) X Output Origin, (option) Y Output Origin,"
                            " (option) X Output Size, (option) Y Output Size."
              << std::endl;
    return EXIT_FAILURE;
  }

  bool useExtractFilter = false;
  if (argc == 9)
  {
    useExtractFilter = true;
  }

  char* inputName   = argv[1];
  char* folderPath  = argv[2];
  char* outputName1 = argv[3];


  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> VImageType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::DEMToImageGenerator<ImageType> DEMToImageGeneratorType;

  typedef otb::ImageFileReader<VImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;

  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType> ExtractVFilterType;
  typedef otb::ExtractROI<PixelType, PixelType>                   ExtractFilterType;

  // Instantiating object
  ReaderType::Pointer              reader           = ReaderType::New();
  DEMToImageGeneratorType::Pointer generatorFilter1 = DEMToImageGeneratorType::New();
  DEMToImageGeneratorType::Pointer generatorFilter2 = DEMToImageGeneratorType::New();
  ExtractVFilterType::Pointer      extract1         = ExtractVFilterType::New();
  ExtractFilterType::Pointer       extract2         = ExtractFilterType::New();

  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();

  otb::DEMHandler::Instance()->OpenDEMDirectory(folderPath);

  // Read input image
  reader->SetFileName(inputName);
  reader->GenerateOutputInformation();

  // First pipeline:

  if (useExtractFilter)
  {
    extract1->SetInput(reader->GetOutput());
    extract1->SetSizeX(atoi(argv[7]));
    extract1->SetSizeY(atoi(argv[8]));
    extract1->SetStartX(atoi(argv[5]));
    extract1->SetStartY(atoi(argv[6]));
    extract1->SetChannel(1);

    extract1->UpdateOutputInformation();
  }

  if (useExtractFilter)
  {
    generatorFilter1->SetOutputParametersFromImage(extract1->GetOutput());
  }
  else
  {
    generatorFilter1->SetOutputParametersFromImage(reader->GetOutput());
  }

  writer1->SetFileName(outputName1);
  writer1->SetInput(generatorFilter1->GetOutput());

  writer1->Update();

  // Second pipeline:
  if (useExtractFilter)
  {
    char* outputName2 = argv[4];
    generatorFilter2->SetOutputParametersFromImage(reader->GetOutput());
    generatorFilter2->InstantiateTransform();

    extract2->SetInput(generatorFilter2->GetOutput());
    extract2->SetSizeX(atoi(argv[7]));
    extract2->SetSizeY(atoi(argv[8]));
    extract2->SetStartX(atoi(argv[5]));
    extract2->SetStartY(atoi(argv[6]));

    writer2->SetFileName(outputName2);
    writer2->SetInput(extract2->GetOutput());

    writer2->Update();
  }

  return EXIT_SUCCESS;
}
