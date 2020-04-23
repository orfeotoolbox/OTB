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
#include "otbRegionImageToRectangularPathListFilter.h"
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"

#include <stdio.h>
#include <iostream>

int otbRegionImageToRectangularPathListFilter(int itkNotUsed(argc), char* argv[])
{

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef unsigned short InputPixelType;
  const unsigned int     Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  reader->Update();

  typedef otb::PolyLineParametricPathWithValue<double, Dimension>               PathType;
  typedef otb::RegionImageToRectangularPathListFilter<InputImageType, PathType> RectangleListFilterType;

  RectangleListFilterType::Pointer rectangleFilter = RectangleListFilterType::New();

  rectangleFilter->SetInput(reader->GetOutput());

  rectangleFilter->SetMinimumFit(::atof(argv[3]));
  rectangleFilter->SetMinimumSize(::atoi(argv[4]));

  rectangleFilter->Update();

  typedef RectangleListFilterType::OutputPathListType ListType;

  ListType* pathList = rectangleFilter->GetOutput();

  ListType::Iterator listIt = pathList->Begin();

  FILE* file = fopen(outputFilename, "w");
  if (file == nullptr)
  {
    fprintf(stderr, "Error, can't open file");
    exit(-1);
  }

  while (listIt != pathList->End())
  {

    for (PathType::VertexListType::ConstIterator vit = listIt.Get()->GetVertexList()->Begin(); vit != listIt.Get()->GetVertexList()->End(); ++vit)
    {
      double x = vit.Value()[0];
      double y = vit.Value()[1];
      fprintf(file, "%8.3f %8.3f\n", x, y);
    }

    double score = listIt.Get()->GetValue();
    fprintf(file, "%8.3f\n", score);

    ++listIt;
  }

  fclose(file);

  return EXIT_SUCCESS;
}
