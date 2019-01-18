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

#include "otbRCC8VertexBase.h"
#include "otbRCC8GraphFileWriter.h"
#include "otbPolygon.h"

int otbRCC8GraphFileWriter(int itkNotUsed(argc), char* argv[])
{
  const char * outputFile = argv[1];

  typedef otb::Polygon<>                          PathType;
  typedef otb::RCC8VertexBase<PathType>           VertexType;
  typedef otb::RCC8Graph<VertexType>              RCC8GraphType;
  typedef otb::RCC8GraphFileWriter<RCC8GraphType> RCC8GraphFileWriterType;

  // Graph instantiation
  RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();
  rcc8Graph->SetNumberOfVertices(4);
  rcc8Graph->Build();

  // Vertex filling
  PathType::Pointer path = PathType::New();
  path->Initialize();

  PathType::ContinuousIndexType p1, p2, p3;

  p1[0] = 0;
  p1[1] = 0;

  p2[0] = 10;
  p2[1] = 10;

  p3[0] = -5;
  p3[1] = 2;

  path->AddVertex(p1);
  path->AddVertex(p2);
  path->AddVertex(p3);

  VertexType::Pointer vertex1, vertex2, vertex3, vertex4;
  vertex1 = VertexType::New();
  vertex1->SetSegmentationLevel(0);
  vertex1->SetSegmentationType(0);
  vertex1->SetPath(path);
  rcc8Graph->SetVertex(0, vertex1);

  vertex2 = VertexType::New();
  vertex2->SetSegmentationLevel(1);
  vertex2->SetSegmentationType(1);
  vertex2->SetPath(path);
  rcc8Graph->SetVertex(1, vertex2);

  vertex3 = VertexType::New();
  vertex3->SetSegmentationLevel(2);
  vertex3->SetSegmentationType(0);
  vertex3->SetPath(path);
  rcc8Graph->SetVertex(2, vertex3);

  vertex4 = VertexType::New();
  vertex4->SetSegmentationLevel(3);
  vertex4->SetSegmentationType(0);
  vertex4->SetPath(path);
  rcc8Graph->SetVertex(3, vertex4);

  // Edge filling
  rcc8Graph->AddEdge(0, 1, otb::OTB_RCC8_EC);
  rcc8Graph->AddEdge(1, 2, otb::OTB_RCC8_PO);
  rcc8Graph->AddEdge(2, 3, otb::OTB_RCC8_TPP);
  rcc8Graph->AddEdge(0, 2, otb::OTB_RCC8_TPPI);
  rcc8Graph->AddEdge(1, 3, otb::OTB_RCC8_NTPP);
  rcc8Graph->AddEdge(0, 3, otb::OTB_RCC8_NTPPI);

  // Instantiation
  RCC8GraphFileWriterType::Pointer rcc8GraphWriter
    = RCC8GraphFileWriterType::New();
  rcc8GraphWriter->SetFileName(outputFile);
  rcc8GraphWriter->SetInput(rcc8Graph);
  rcc8GraphWriter->Update();

  return EXIT_SUCCESS;
}
