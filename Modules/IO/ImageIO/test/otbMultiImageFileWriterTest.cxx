/*
 * Copyright (C) 2017-2019 CS Systemes d'Information (CS SI)
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

#include "otbMultiImageFileWriter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

int otbMultiImageFileWriterTest(int argc, char* argv[])
{
  typedef unsigned short PixelType1;
  typedef otb::Image<PixelType1, 2> ImageType1;
  typedef otb::ImageFileReader<ImageType1> ReaderType1;

  typedef double PixelType2;
  typedef otb::Image<PixelType2, 2> ImageType2;
  typedef otb::ImageFileReader<ImageType2> ReaderType2;

  typedef otb::MultiImageFileWriter WriterType;

  if (argc < 6)
  {
    std::cout << "Usage: " << argv[0] << " inputImageFileName1 inputImageFileName2 outputImageFileName1 outputImageFileName2 numberOfLinesPerStrip\n";
    return EXIT_FAILURE;
  }

  const char*       inputImageFileName1   = argv[1];
  const char*       inputImageFileName2   = argv[2];
  const std::string outputImageFileName1  = argv[3];
  const std::string outputImageFileName2  = argv[4];
  const int         numberOfLinesPerStrip = atoi(argv[5]);

  ReaderType1::Pointer reader1 = ReaderType1::New();
  reader1->SetFileName(inputImageFileName1);

  ReaderType2::Pointer reader2 = ReaderType2::New();
  reader2->SetFileName(inputImageFileName2);

  WriterType::Pointer writer = WriterType::New();
  writer->AddInputImage(reader1->GetOutput(), outputImageFileName1);
  writer->AddInputImage(reader2->GetOutput(), outputImageFileName2);
  writer->SetNumberOfLinesStrippedStreaming(numberOfLinesPerStrip);

  writer->Update();

  std::cout << writer << std::endl;

  return EXIT_SUCCESS;
}
