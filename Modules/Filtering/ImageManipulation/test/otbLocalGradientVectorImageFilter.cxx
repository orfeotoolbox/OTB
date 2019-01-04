/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbLocalGradientVectorImageFilter.h"

int otbLocalGradientVectorImageFilterTest ( int argc, char* argv[] )
{

  if (argc != 3)
    {
    std::cerr << "Usage: otbLocalGradientVectorImageFilterTest input output\n";
    return EXIT_FAILURE;
    }

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  // Image filtering
  typedef otb::LocalGradientVectorImageFilter< ImageType, ImageType >
    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  typedef otb::ImageFileWriter< ImageType > ImageWriterType;
  ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
