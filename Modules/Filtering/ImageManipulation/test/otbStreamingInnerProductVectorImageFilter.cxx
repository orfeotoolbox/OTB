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

#include "otbStreamingInnerProductVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbStreamingInnerProductVectorImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char * inputFileName = argv[1];
  const char * outfname = argv[2];
  const bool   centerdata = atoi(argv[3]);

  typedef double PixelType;
  const unsigned int Dimension = 2;

  // Typedef
  typedef otb::VectorImage<PixelType, Dimension>                 ImageType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::StreamingInnerProductVectorImageFilter<ImageType> FilterType;

  ReaderType::Pointer reader     = ReaderType::New();
  reader->SetFileName(inputFileName);

  // Instantiation object
  FilterType::Pointer filter = FilterType::New();
  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 10 );
  filter->SetCenterData(centerdata);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file.precision(10);
  file << std::fixed;
  file << "Inner Product: Dim [" << filter->GetInnerProduct().size() << "]:" << std::endl;
  file << filter->GetInnerProduct() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
