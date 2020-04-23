/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageSeriesFileReader.h"
#include "otbImageFileWriter.h"
#include "itksys/SystemTools.hxx"

int otbImageSeriesFileReader(int itkNotUsed(argc), char* argv[])
{
  const char* enviMetaFile = argv[1];
  const char* outputFile   = argv[2];

  const unsigned int    Dimension = 2;
  typedef unsigned char PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageList<ImageType> ImageListType;

  typedef otb::ImageSeriesFileReader<ImageType> ImageReader;
  ImageReader::Pointer                          reader = ImageReader::New();
  reader->SetFileName(enviMetaFile);
  reader->Update();

  std::cout << reader << std::endl;

  ImageListType* imageList = reader->GetOutput();

  typedef otb::ImageFileWriter<ImageType> WriterType;

  for (unsigned int i = 0; i < imageList->Size(); ++i)
  {
    std::cout << "Image no " << i << "\n";

    std::string       path = itksys::SystemTools::GetFilenamePath(outputFile);
    std::string       name = itksys::SystemTools::GetFilenameWithoutExtension(outputFile);
    std::string       ext  = itksys::SystemTools::GetFilenameExtension(outputFile);
    std::stringstream outputFilenameStream;
    outputFilenameStream << path << "/" << name << "-" << i << ext;

    std::string outputFilename = itksys::SystemTools::ConvertToOutputPath(outputFilenameStream.str().c_str());
    std::cout << "saved in " << outputFilename << "\n";
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput(i));
    writer->Update();
  }

  return EXIT_SUCCESS;
}
