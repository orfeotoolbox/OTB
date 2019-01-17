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
#include <iostream>
#include <list>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbDrawLineSpatialObjectFilter.h"

int otbDrawLineSpatialObject(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  // two points to represent a straight line
  double Ux((double) ::atof(argv[3]));
  double Uy((double) ::atof(argv[4]));
  double Vx((double) ::atof(argv[5]));
  double Vy((double) ::atof(argv[6]));

  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::DrawLineSpatialObjectFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);

  writer->SetFileName(outputFilename);

  // Definition of the line
  typedef itk::LineSpatialObject<2> LineType;
  LineType::PointListType list;
  LineType::LinePointType point;

  point.SetPosition(Ux, Uy);
  list.push_back(point);
  point.SetPosition(Vx, Vy);
  list.push_back(point);

  LineType::Pointer line = LineType::New();
  line->SetId(0);
  line->SetPoints(list);
  line->ComputeBoundingBox();

  filter->SetValue(static_cast<OutputPixelType>(245));
  filter->SetInputLine(line);
  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
