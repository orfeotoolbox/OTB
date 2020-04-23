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


#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataFileWriter(int itkNotUsed(argc), char* argv[])
{

  typedef otb::VectorData<double, 2> VectorDataType;
  typedef VectorDataType::DataNodeType              DataNodeType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  typedef DataNodeType::LineType                    LineType;
  typedef LineType::VertexType                      VertexType;

  // Instantiation
  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data   = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder1  = DataNodeType::New();
  DataNodeType::Pointer folder2  = DataNodeType::New();
  DataNodeType::Pointer folder3  = DataNodeType::New();
  DataNodeType::Pointer line     = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder1->SetNodeType(otb::FOLDER);
  folder2->SetNodeType(otb::FOLDER);
  folder3->SetNodeType(otb::FOLDER);
  line->SetNodeType(otb::FEATURE_LINE);

  document->SetNodeId("DOCUMENT");
  folder1->SetNodeId("FOLDER1");
  folder2->SetNodeId("FOLDER2");
  folder3->SetNodeId("FOLDER3");
  line->SetNodeId("FEATURE_LINE");

  VertexType p1;
  p1.Fill(5);

  VertexType p3;
  p3.Fill(0);

  VertexType p2;
  p2[0] = 0;
  p2[1] = 10;

  LineType::Pointer l = LineType::New();
  l->AddVertex(p1);
  l->AddVertex(p2);
  l->AddVertex(p3);
  line->SetLine(l);

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder1, document);
  data->GetDataTree()->Add(folder2, document);
  data->GetDataTree()->Add(folder3, document);
  data->GetDataTree()->Add(line, folder2);

  writer->SetFileName(argv[1]);
  writer->SetInput(data);
  writer->Update();

  return EXIT_SUCCESS;
}
