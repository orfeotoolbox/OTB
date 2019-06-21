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
#include "otbFillGapsFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbDrawLineSpatialObjectListFilter.h"

int otbFillGapsFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::DrawLineSpatialObjectListFilter<InputImageType, OutputImageType> FilterType;

  typedef otb::FillGapsFilter        FillGapsFilterType;
  typedef otb::LineSpatialObjectList LinesListType;
  typedef LinesListType::LineType    LineType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  FillGapsFilterType::Pointer fillgaps = FillGapsFilterType::New();

  LinesListType::Pointer linesListBeforeFillGaps = LinesListType::New();
  const LinesListType *  linesListAfterFillGaps;

#if ITK_VERSION_MAJOR < 5
  LineType::PointListType pointList;
#else
  LineType::LinePointListType pointList;
#endif

  LineType::LinePointType point;

  // Definition of the first line
  double Ux, Uy, Vx, Vy;
  Ux = 10.;
  Uy = 10.;
  Vx = 10.;
  Vy = 20.;
  
#if ITK_VERSION_MAJOR < 5
  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);
#else
  point.SetPositionInObjectSpace(Ux, Uy);
  pointList.push_back(point);
  point.SetPositionInObjectSpace(Vx, Vy);
  pointList.push_back(point);
#endif

  LineType::Pointer line = LineType::New();
  line->SetId(0);
  line->SetPoints(pointList);
  line->ComputeBoundingBox();

  linesListBeforeFillGaps->push_back(line);

  pointList.clear();

  // Definition of a second line

  Ux = 10.;
  Uy = 30.;
  Vx = 10.;
  Vy = 50.;
  
#if ITK_VERSION_MAJOR < 5
  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);
#else
  point.SetPositionInObjectSpace(Ux, Uy);
  pointList.push_back(point);
  point.SetPositionInObjectSpace(Vx, Vy);
  pointList.push_back(point);
#endif

  LineType::Pointer line2 = LineType::New();
  line2->SetId(0);
  line2->SetPoints(pointList);
  line2->ComputeBoundingBox();

  linesListBeforeFillGaps->push_back(line2);

  pointList.clear();

  // Definition of a third line

  Ux = 20.;
  Uy = 50.;
  Vx = 50.;
  Vy = 50.;

#if ITK_VERSION_MAJOR < 5
  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);
#else
  point.SetPositionInObjectSpace(Ux, Uy);
  pointList.push_back(point);
  point.SetPositionInObjectSpace(Vx, Vy);
  pointList.push_back(point);
#endif

  LineType::Pointer line3 = LineType::New();
  line3->SetId(0);
  line3->SetPoints(pointList);
  line3->ComputeBoundingBox();

  linesListBeforeFillGaps->push_back(line3);

  pointList.clear();
  //  FillGapsFilter parameters

  fillgaps->SetRadius(15.);
  fillgaps->SetAngularBeam(1.0);  // Angle in Radian
  fillgaps->SetInput(linesListBeforeFillGaps);
  linesListAfterFillGaps = fillgaps->GetOutput();
  fillgaps->Update();

  filter->SetInputLineSpatialObjectList(linesListAfterFillGaps);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
