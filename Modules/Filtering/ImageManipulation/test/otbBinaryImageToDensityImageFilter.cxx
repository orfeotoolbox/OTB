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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbBinaryImageDensityFunction.h"
#include "otbBinaryImageToDensityImageFilter.h"

int otbBinaryImageToDensityImageFilter(int itkNotUsed(argc), char* argv[])
{

  const char*        infname  = argv[1];
  const char*        outfname = argv[2];
  const unsigned int radius   = atoi(argv[3]);

  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::Image<PixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  typedef otb::BinaryImageDensityFunction<InputImageType> CountFunctionType;
  typedef otb::BinaryImageToDensityImageFilter<InputImageType, InputImageType, CountFunctionType> FilterType;

  /** Instantiation of a Smart Pointer*/
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  /** */
  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetNeighborhoodRadius(radius);

  /** */
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
