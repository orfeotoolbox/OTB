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

#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygonListToRCC8GraphFilter.h"
#include "otbRCC8GraphFileWriter.h"

// typedefs
typedef otb::Polygon<>                                                    PolygonType;
typedef otb::ObjectList<PolygonType>                                      PolygonListType;
typedef otb::RCC8VertexBase<PolygonType>                                  VertexType;
typedef otb::RCC8Graph<VertexType>                                        RCC8GraphType;
typedef otb::PolygonListToRCC8GraphFilter<PolygonListType, RCC8GraphType> RCC8GraphFilterType;
typedef otb::RCC8GraphFileWriter<RCC8GraphType>                           GraphWriterType;


PolygonType::VertexType Vertex(double i,double j)
{
  PolygonType::VertexType vertex;
  vertex[0] = i;
  vertex[1] = j;

  return vertex;
}


int otbPolygonListToRCC8GraphFilter(int itkNotUsed(argc), char* argv[])
{
  char * outfile = argv[1];

  PolygonListType::Pointer regions = PolygonListType::New();

  RCC8GraphFilterType::SegmentationRangesType ranges;

  ranges.push_back(0);

  PolygonType::Pointer polygon = PolygonType::New();
  polygon->AddVertex(Vertex(0,0));
  polygon->AddVertex(Vertex(2,0));
  polygon->AddVertex(Vertex(2,6));
  polygon->AddVertex(Vertex(0,6));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(1,1));
  polygon->AddVertex(Vertex(4,1));
  polygon->AddVertex(Vertex(4,5));
  polygon->AddVertex(Vertex(1,5));
  regions->PushBack(polygon);

  ranges.push_back(2);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(3,1));
  polygon->AddVertex(Vertex(4,1));
  polygon->AddVertex(Vertex(4,2));
  polygon->AddVertex(Vertex(3,2));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(5,1));
  polygon->AddVertex(Vertex(8,1));
  polygon->AddVertex(Vertex(8,5));
  polygon->AddVertex(Vertex(5,5));
  regions->PushBack(polygon);

  ranges.push_back(4);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(6,2));
  polygon->AddVertex(Vertex(7,2));
  polygon->AddVertex(Vertex(7,3));
  polygon->AddVertex(Vertex(6,3));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(8,1));
  polygon->AddVertex(Vertex(9,1));
  polygon->AddVertex(Vertex(9,5));
  polygon->AddVertex(Vertex(8,5));
  regions->PushBack(polygon);

  // Filter instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  filter->SetInput(regions);
  filter->SetSegmentationRanges(ranges);

  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfile);
  writer->Update();

  return EXIT_SUCCESS;
}
