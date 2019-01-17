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

#include <complex>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbMacro.h"

int otbWritingComplexDataWithComplexImageTest(int itkNotUsed(argc), char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef otb::ExtractROI<PixelType, PixelType> ExtractROIFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
  extract->SetInput(reader->GetOutput());
  extract->SetStartX(atoi(argv[3]));
  extract->SetStartY(atoi(argv[4]));
  extract->SetSizeX(atoi(argv[5]));
  extract->SetSizeY(atoi(argv[6]));

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(extract->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
