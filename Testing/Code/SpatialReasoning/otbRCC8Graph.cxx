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
#include "otbRCC8Graph.h"
#include "otbRCC8VertexBase.h"
#include "otbRCC8VertexIterator.h"
#include "otbRCC8EdgeIterator.h"
#include "otbRCC8InEdgeIterator.h"
#include "otbRCC8OutEdgeIterator.h"
#include "otbMacro.h"
#include "otbPolygon.h"

int otbRCC8Graph(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int nbVertices = 2;
  typedef otb::Polygon<>                          PathType;
  typedef PathType::VertexType                    PointType;
  typedef otb::RCC8VertexBase<PathType>           VertexType;
  typedef otb::RCC8Graph<VertexType>              RCC8GraphType;
  typedef otb::RCC8VertexIterator<RCC8GraphType>  VertexIteratorType;
  typedef otb::RCC8EdgeIterator<RCC8GraphType>    EdgeIteratorType;

  // Instantiation
  RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();
  // Setting the number of vertices
  rcc8Graph->SetNumberOfVertices(nbVertices - 1);
  // Call to the build method
  rcc8Graph->Build();

  PointType p1, p2, p3, p4, p5, p6;

  p1[0] = 0;
  p1[1] = 0;
  p2[0] = 10;
  p2[1] = 10;
  p3[0] = 10;
  p3[1] = 0;
  p4[0] = 20;
  p4[1] = 20;
  p5[0] = 20;
  p5[1] = 10;
  p6[0] = 10;
  p6[1] = 20;

  unsigned int vertex1SegLevel = 0;
  unsigned int vertex2SegLevel = 10;
  bool         vertex1SegType = true;
  bool         vertex2SegType = false;

  PathType::Pointer path1 = PathType::New();
  path1->AddVertex(p1);
  path1->AddVertex(p2);
  path1->AddVertex(p3);

  VertexType::Pointer vertex1 = VertexType::New();
  vertex1->SetSegmentationLevel(vertex1SegLevel);
  vertex1->SetSegmentationType(vertex1SegType);
  vertex1->SetPath(path1);

  PathType::Pointer path2 = PathType::New();
  path2->AddVertex(p4);
  path2->AddVertex(p5);
  path2->AddVertex(p6);

  VertexType::Pointer vertex2 = VertexType::New();
  vertex2->SetSegmentationLevel(vertex2SegLevel);
  vertex2->SetSegmentationType(vertex2SegType);
  vertex2->SetPath(path2);

  rcc8Graph->SetVertex(0, vertex1);
  rcc8Graph->SetVertex(1, vertex2);

  otbControlConditionTestMacro(rcc8Graph->GetNumberOfVertices() != nbVertices,
                               "rcc8Graph->GetNumberOfVertices()!=nbVertices");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(0)->GetSegmentationLevel() != vertex1SegLevel,
                               "rcc8Graph->GetVertex(0)->GetSegmentationLevel()!=vertex1SegLevel");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(0)->GetSegmentationType() != vertex1SegType,
                               "rcc8Graph->GetVertex(0)->GetSegmentationType()!=vertex1SegType");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(0) != p1,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(0)!=p1");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(1) != p2,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(1)!=p2");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(2) != p3,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(2)!=p3");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(1)->GetSegmentationLevel() != vertex2SegLevel,
                               "rcc8Graph->GetVertex(1)->GetSegmentationLevel()!=vertex2SegLevel");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(1)->GetSegmentationType() != vertex2SegType,
                               "rcc8Graph->GetVertex(1)->GetObjectLabelInImgage()!=vertex2SegType");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(0) != p4,
                               "rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(0)!=p4");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(1) != p5,
                               "rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(1)!=p5");
  otbControlConditionTestMacro(rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(2) != p6,
                               "rcc8Graph->GetVertex(1)->GetPath()->GetVertexList()->GetElement(2)!=p6");

  VertexIteratorType v(rcc8Graph);
  v.GoToBegin();
  otbControlConditionTestMacro(v.Get()->GetSegmentationLevel() != vertex1SegLevel,
                               "v.Get()->GetSegmentationLevel()!=vertex1SegLevel");
  otbControlConditionTestMacro(v.Get()->GetSegmentationType() != vertex1SegType,
                               "v.Get()->GetSegmentationType()!=vertex1SegType");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(0) != p1,
                               "v.Get()->GetPath()->GetVertexList()->GetElement(0)!=p1");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(1) != p2,
                               "v.Get()->GetPath()->GetVertexList()->GetElement(1)!=p2");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(2) != p3,
                               "v.Get()->GetPath()->GetVertexList()->GetElement(2)!=p3");
  ++v;
  otbControlConditionTestMacro(v.Get()->GetSegmentationLevel() != vertex2SegLevel,
                               "v.Get()->GetSegmentationLevel()!=vertex2SegLevel");
  otbControlConditionTestMacro(v.Get()->GetSegmentationType() != vertex2SegType,
                               "v.Get()->GetObjectLabelInImgage()!=vertex2SegType");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(0) != p4,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(0)!=p4");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(1) != p5,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(1)!=p5");
  otbControlConditionTestMacro(v.Get()->GetPath()->GetVertexList()->GetElement(2) != p6,
                               "rcc8Graph->GetVertex(0)->GetPath()->GetVertexList()->GetElement(2)!=p6");

  // Testing the edge iterator
  rcc8Graph->AddEdge(0, 1, otb::OTB_RCC8_NTPPI);

  otbControlConditionTestMacro(rcc8Graph->GetNumberOfEdges() != 1,
                               "rcc8Graph->GetNumberOfEdges()!=1");

  EdgeIteratorType e(rcc8Graph);
  e.GoToBegin();
  otbControlConditionTestMacro(e.GetValue() != otb::OTB_RCC8_NTPPI,
                               "e.GetValue()!=otb::OTB_RCC8_NTPPI");
  otbControlConditionTestMacro(e.GetSourceIndex() != 0,
                               "e.GetSourceIndex()!=0");
  otbControlConditionTestMacro(e.GetTargetIndex() != 1,
                               "e.GetTargetIndex()!=1");

  return EXIT_SUCCESS;
}
