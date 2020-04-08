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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbTileImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef otb::Image<unsigned char>             ImageType;
typedef otb::VectorImage<unsigned char>       VectorImageType;
typedef otb::TileImageFilter<ImageType>       TileImageFilterType;
typedef otb::TileImageFilter<VectorImageType> TileVectorImageFilterType;
typedef otb::ImageFileReader<VectorImageType> VectorImageReaderType;
typedef otb::ImageFileWriter<VectorImageType> VectorImageWriterType;


int otbTileImageFilter(int argc, char* argv[])
{
  TileVectorImageFilterType::SizeType layout;
  layout[0] = atoi(argv[1]);
  layout[1] = atoi(argv[2]);

  const char* outfname = argv[3];

  unsigned int numberOfImages = layout[0] * layout[1];

  std::vector<VectorImageReaderType::Pointer> readersVector;

  TileVectorImageFilterType::Pointer tileFilter = TileVectorImageFilterType::New();
  tileFilter->SetLayout(layout);

  for (unsigned int i = 0; i < numberOfImages; ++i)
  {
    if (i + 4 > (unsigned int)argc)
    {
      std::cerr << "Not enough images to support layout!" << std::endl;
      return EXIT_FAILURE;
    }

    VectorImageReaderType::Pointer reader = VectorImageReaderType::New();
    reader->SetFileName(argv[i + 4]);

    readersVector.push_back(reader);

    tileFilter->SetInput(i, reader->GetOutput());
  }


  VectorImageWriterType::Pointer writer = VectorImageWriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(tileFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
