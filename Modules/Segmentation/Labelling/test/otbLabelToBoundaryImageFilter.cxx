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


#include "otbLabelToBoundaryImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbLabelToBoundaryImageFilter(int argc, char* argv[])
{
  typedef unsigned int  InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::LabelToBoundaryImageFilter<InputImageType, OutputImageType> BoundaryFilterType;
  typedef BoundaryFilterType::Pointer BoundaryFilterPointerType;

  if (argc != 3)
  {
    std::cerr << "Usage : " << argv[0] << "  input_label_image  output_boundary" << std::endl;
    return EXIT_FAILURE;
  }

  std::string inputPath(argv[1]);
  std::string outputPath(argv[2]);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputPath);

  BoundaryFilterPointerType filter = BoundaryFilterType::New();
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputPath);
  writer->Update();

  return EXIT_SUCCESS;
}
