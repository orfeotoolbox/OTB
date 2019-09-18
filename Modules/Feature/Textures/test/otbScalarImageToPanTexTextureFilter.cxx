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

#include "otbScalarImageToPanTexTextureFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbScalarImageToPanTexTextureFilter(int argc, char* argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " infname outprefix nbBins radius" << std::endl;
    return EXIT_FAILURE;
  }
  const char*        infname   = argv[1];
  const char*        outprefix = argv[2];
  const unsigned int nbBins    = atoi(argv[3]);
  const unsigned int radius    = atoi(argv[4]);

  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::Image<PixelType, Dimension>                            ImageType;
  typedef otb::ScalarImageToPanTexTextureFilter<ImageType, ImageType> PanTexTextureFilterType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer              reader = ReaderType::New();
  PanTexTextureFilterType::Pointer filter = PanTexTextureFilterType::New();
  WriterType::Pointer              writer = WriterType::New();

  // Read image
  reader->SetFileName(infname);

  // Build radius
  PanTexTextureFilterType::SizeType sradius;
  sradius.Fill(radius);

  filter->SetInput(reader->GetOutput());
  filter->SetRadius(sradius);

  otb::StandardFilterWatcher watcher(filter, "Textures filter");

  filter->SetNumberOfBinsPerAxis(nbBins);
  filter->SetInputImageMinimum(0);
  filter->SetInputImageMaximum(255);

  // Write outputs
  std::ostringstream oss;

  oss.str("");
  oss << outprefix << "PanTex.tif";
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(oss.str());
  writer->SetNumberOfDivisionsStrippedStreaming(2);
  writer->Update();

  return EXIT_SUCCESS;
}
