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


#include "otbMapFileProductWriter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbMapFileProductWriter(int itkNotUsed(argc), char* argv[])
{
  const char * infname = argv[1];
  const char * path = argv[2];
  const char * shapepath = argv[3];
  const char * cgipath = argv[4];
  unsigned int tileSize = atoi(argv[5]);

  typedef otb::VectorImage<double>               ImageType;
  typedef otb::MapFileProductWriter<ImageType>   MapFileProductWriterType;
  typedef otb::ImageFileReader<ImageType>        ImageFileReaderType;

  MapFileProductWriterType::Pointer    mapWriter  = MapFileProductWriterType::New();
  ImageFileReaderType::Pointer         reader     = ImageFileReaderType::New();

  // Read the image
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  mapWriter->SetInput(reader->GetOutput());
  mapWriter->SetFileName(path);
  mapWriter->SetShapeIndexPath(shapepath);
  mapWriter->SetCGIPath(cgipath);
  mapWriter->SetTileSize(tileSize);
  mapWriter->SetSRID(4326);

  mapWriter->Update();


  return EXIT_SUCCESS;
}
