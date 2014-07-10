/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataFileWriter(int itkNotUsed(argc), char * argv[])
{

  typedef otb::VectorData<double, 2>                VectorDataType;
  typedef VectorDataType::DataNodeType              DataNodeType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  typedef DataNodeType::PointType                   PointType;
  typedef DataNodeType::LineType                    LineType;
  typedef DataNodeType::PolygonType                 PolygonType;
  typedef LineType::VertexType                      VertexType;

  //Instantiation
  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder1 = DataNodeType::New();
  DataNodeType::Pointer folder2 = DataNodeType::New();
  DataNodeType::Pointer folder3 = DataNodeType::New();
  DataNodeType::Pointer line = DataNodeType::New();

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
