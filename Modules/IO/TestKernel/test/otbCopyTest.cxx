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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"


// Copy the input image in the output image. Used to test --compare-image. 
// We don't call --compare-image directly on the input data because the output (.diff, .base, .test)
// are written at the same location as the input, and we don't want to pollute OTB/Data
int otbCopyTest(int argc, char* argv[])
{ 
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFileName outputFileName" << std::endl;
    return EXIT_FAILURE;
  }

  using ImageType = otb::VectorImage<double, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;

  auto reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  auto writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(reader->GetOutput());
  
  writer->Update();

  return EXIT_SUCCESS;
}

