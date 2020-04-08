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

#include "itkMacro.h"

#include "otbImage.h"
#include "otbPolygon.h"
#include "otbPersistentVectorizationImageFilter.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbPersistentVectorizationFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const unsigned int     ImageDimension = 2;
  typedef unsigned short LabelType;

  typedef otb::Image<LabelType, ImageDimension> ImageType;
  typedef otb::Polygon<LabelType> PolygonType;
  typedef otb::PersistentVectorizationImageFilter<ImageType, PolygonType> FilterType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->Update();

  typedef FilterType::PathListType PathListType;

  std::ofstream f;
  f.open(outfname);

  for (PathListType::Iterator it = filter->GetPathList()->Begin(); it != filter->GetPathList()->End(); ++it)
  {
    for (unsigned int i = 0; i < it.Get()->GetVertexList()->Size(); ++i)
    {
      f << "[ " << it.Get()->GetVertexList()->GetElement(i)[0] << ", " << it.Get()->GetVertexList()->GetElement(i)[1] << "] ";
    }
    f << std::endl;
  }

  f.close();

  return EXIT_SUCCESS;
}
