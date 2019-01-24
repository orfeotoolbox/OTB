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


#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataFileWriterMultiPolygons(int itkNotUsed(argc), char * argv[])
{

  typedef otb::VectorData<double, 2>                VectorDataType;
  typedef VectorDataType::DataNodeType              DataNodeType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  typedef DataNodeType::LineType                    LineType;
  typedef DataNodeType::PolygonType                 PolygonType;
  typedef LineType::VertexType                      VertexType;

  //Instantiation
  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();

  DataNodeType::Pointer polygon1 = DataNodeType::New();
  DataNodeType::Pointer polygon2 = DataNodeType::New();
  DataNodeType::Pointer polygon3 = DataNodeType::New();
  DataNodeType::Pointer polygon4 = DataNodeType::New();
  DataNodeType::Pointer polygon5 = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  polygon1->SetNodeType(otb::FEATURE_POLYGON);
  polygon2->SetNodeType(otb::FEATURE_POLYGON);
  polygon5->SetNodeType(otb::FEATURE_MULTIPOLYGON);

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");
  polygon5->SetNodeId("FEATURE_MULTIPOLYGON");
  polygon1->SetNodeId("FEATURE_POLYGON");
  polygon2->SetNodeId("FEATURE_POLYGON");

  VertexType p1;
  p1.Fill(5);

  VertexType p3;
  p3.Fill(0);

  VertexType p2;
  p2[0] = 0;
  p2[1] = 10;

  VertexType p4;
  p4[0] = 1;
  p4[1] = 15;

  PolygonType::Pointer poly = PolygonType::New();
  poly->AddVertex(p1);
  poly->AddVertex(p2);
  poly->AddVertex(p3);
  polygon1->SetPolygonExteriorRing(poly);

  PolygonType::Pointer poly1 = PolygonType::New();
  poly1->AddVertex(p1);
  poly1->AddVertex(p4);
  poly1->AddVertex(p2);
  polygon2->SetPolygonExteriorRing(poly1);

  polygon3 = polygon1;
  polygon4 = polygon2;

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder, document);
  data->GetDataTree()->Add(polygon5, folder);
  data->GetDataTree()->Add(polygon1, polygon5);
  data->GetDataTree()->Add(polygon2, polygon5);
  data->GetDataTree()->Add(polygon3, polygon5);

  writer->SetFileName(argv[1]);
  writer->SetInput(data);
  writer->Update();

  return EXIT_SUCCESS;
}
