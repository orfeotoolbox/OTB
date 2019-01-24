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




#include "otbVectorImage.h"
#include "itkMacro.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbGDALImageIO.h"

template<class PixelType>
int otbGenericGDALImageIOTest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  std::vector<std::string> creationOptions;
  for (int curArg = 3; curArg < argc; curArg++)
    {
    creationOptions.push_back(argv[curArg]);
    std::cout << "Creation option : " << argv[curArg] << std::endl;
    }

  typedef PixelType InputPixelType;
  typedef PixelType OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();


  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  // Be sure to use GDALImageIO
  otb::GDALImageIO::Pointer io = otb::GDALImageIO::New();
  io->SetOptions(creationOptions);
  writer->SetImageIO(io.GetPointer());

  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbGDALImageIOTest_uint8(int argc, char* argv[])
{
  return otbGenericGDALImageIOTest<unsigned char>(argc, argv);
}

int otbGDALImageIOTest_uint16(int argc, char* argv[])
{
  return otbGenericGDALImageIOTest<unsigned short>(argc, argv);
}
