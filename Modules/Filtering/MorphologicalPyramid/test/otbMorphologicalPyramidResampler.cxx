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

#include "itkMacro.h"
#include "otbMorphologicalPyramidResampler.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidResampler(int itkNotUsed(argc), char * argv[])
{
  const char*        inputFilename = argv[1];
  const char*        outputFilename1 = argv[2];
  const char*        outputFilename2 = argv[3];
  const unsigned int size_x = atoi(argv[4]);
  const unsigned int size_y = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef double OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::MorphologicalPyramid::Resampler<InputImageType, OutputImageType>
  ResamplerType;

  // Input image reading
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  InputImageType::SizeType size;
  size[0] = size_x;
  size[1] = size_y;

  // Instantiation
  ResamplerType::Pointer resampler = ResamplerType::New();
  resampler->SetInput(reader->GetOutput());
  resampler->SetSize(size);

  // File writing
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(resampler->GetOutput());
  writer->SetFileName(outputFilename1);
  writer->Update();

  ResamplerType::Pointer resampler2 = ResamplerType::New();
  resampler2->SetInput(resampler->GetOutput());
  resampler2->SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(resampler2->GetOutput());
  writer2->SetFileName(outputFilename2);
  writer2->Update();

  return EXIT_SUCCESS;
}
