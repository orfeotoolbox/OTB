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

int otbVectorDataFileWriterPolygons(int itkNotUsed(argc), char * argv[])
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
  DataNodeType::Pointer folder = DataNodeType::New();

  DataNodeType::Pointer polygon1 = DataNodeType::New();
  DataNodeType::Pointer polygon2 = DataNodeType::New();
  DataNodeType::Pointer polygon3 = DataNodeType::New();
  DataNodeType::Pointer polygon4 = DataNodeType::New();
  DataNodeType::Pointer polygon5 = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  polygon1->SetNodeType(otb::FEATURE_POLYGON);

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("THE_FOLDER");
  polygon1->SetNodeId("FEATURE_POLYGON");

  VertexType p1;
  p1.Fill(5);

  VertexType p3;
  p3.Fill(0);

  VertexType p2;
  p2[0] = 0;
  p2[1] = 10;

  PolygonType::Pointer poly = PolygonType::New();
  poly->AddVertex(p1);
  poly->AddVertex(p2);
  poly->AddVertex(p3);
  polygon1->SetLine(poly);

  polygon2 = polygon1;
  polygon3 = polygon1;
  polygon4 = polygon1;
  polygon5 = polygon1;

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder, document);
  data->GetDataTree()->Add(polygon1, folder);
  data->GetDataTree()->Add(polygon2, folder);
  data->GetDataTree()->Add(polygon3, folder);
  data->GetDataTree()->Add(polygon4, folder);
  data->GetDataTree()->Add(polygon5, folder);

  writer->SetFileName(argv[1]);
  writer->SetInput(data);
  writer->Update();

  return EXIT_SUCCESS;
}
