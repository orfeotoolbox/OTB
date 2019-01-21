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


#include "otbPhysicalToRPCSensorModelImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbPhysicalToRPCSensorModelImageFilter(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef otb::PhysicalToRPCSensorModelImageFilter<ImageType> PhysicalToSensorModelType;

  // Object instantiation
  PhysicalToSensorModelType::Pointer estimator = PhysicalToSensorModelType::New();

  if(argc == 4)
    {
    const char * infname  = argv[1];
    unsigned int gridSize = atoi(argv[2]);
    const char * outfname  = argv[3];

    // Reader
    typedef otb::ImageFileReader<ImageType>             ReaderType;
    ReaderType::Pointer                reader    = ReaderType::New();

    // Set the fileName
    reader->SetFileName(infname);
    reader->UpdateOutputInformation();

    // wire the filter
    estimator->SetInput(reader->GetOutput());
    estimator->SetGridSize(gridSize);

    // Write the resampled image
    typedef otb::ImageFileWriter<ImageType>    WriterType;
    WriterType::Pointer writer= WriterType::New();
    writer->SetAutomaticTiledStreaming();
    writer->SetFileName(outfname);
    writer->SetInput(estimator->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout <<"Unit Test" << std::endl;
    }

  return 0;
}
