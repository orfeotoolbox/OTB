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
#include "itkMacro.h"
#include "otbRCC8VertexWithCompacity.h"
#include "otbMacro.h"
#include "otbPolygon.h"

int otbRCC8VertexWithCompacity(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Polygon<> PathType;
  typedef double         PrecisionType;

  unsigned int  imageLevel = atoi(argv[1]);
  bool          imageType = atoi(argv[2]);
  PrecisionType compacity = atof(argv[3]);

  typedef otb::RCC8VertexWithCompacity<PathType, PrecisionType> RCC8VertexType;
  typedef RCC8VertexType::AttributesMapType                     AttributesMapType;
  // Instantiation
  RCC8VertexType::Pointer vertex1 = RCC8VertexType::New();

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

  // Getters / setters tests
  vertex1->SetSegmentationLevel(imageLevel);
  vertex1->SetSegmentationType(imageType);
  vertex1->SetPath(path);
  vertex1->SetCompacity(compacity);

  otbControlConditionTestMacro(vertex1->GetSegmentationLevel() != imageLevel,
                               "Test failed: vertex1->GetSegmentationLevel()!=imageLevel");
  otbControlConditionTestMacro(vertex1->GetSegmentationType() != imageType,
                               "Test failed: vertex1->GetSegmentationType()!=imageType");

  otbControlConditionTestMacro(vertex1->GetPath()->GetVertexList()->GetElement(0) != p1,
                               "Test failed:vertex1->GetPath()->GetVertexList()->GetElement(0)!=p1");
  otbControlConditionTestMacro(vertex1->GetPath()->GetVertexList()->GetElement(1) != p2,
                               "Test failed:vertex1->GetPath()->GetVertexList()->GetElement(1)!=p2");
  otbControlConditionTestMacro(vertex1->GetPath()->GetVertexList()->GetElement(2) != p3,
                               "Test failed:vertex1->GetPath()->GetVertexList()->GetElement(2)!=p3");
  otbControlConditionTestMacro(vertex1->GetCompacity() != compacity,
                               "Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex");
  // attributes vector set test
  AttributesMapType attr1 = vertex1->GetAttributesMap();

  std::ostringstream oss;
  oss << p1[0];
  otbControlConditionTestMacro(attr1["P0x"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P0x\"].compare(std::string(p1[0]))!=0");
  oss.str("");
  oss << p1[1];
  otbControlConditionTestMacro(attr1["P0y"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P0y\"].compare(std::string(p1[1]))!=0");
  oss.str("");
  oss << p2[0];
  otbControlConditionTestMacro(attr1["P1x"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P1x\"].compare(std::string(p2[0]))!=0");
  oss.str("");
  oss << p2[1];
  otbControlConditionTestMacro(attr1["P1y"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P1y\"].compare(std::string(p2[1]))!=0");
  oss.str("");
  oss << p3[0];
  otbControlConditionTestMacro(attr1["P2x"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P2x\"].compare(std::string(p3[0]))!=0");
  oss.str("");
  oss << p3[1];
  otbControlConditionTestMacro(attr1["P2y"].compare(oss.str()) != 0,
                               "Test failed:attr1[\"P2y\"].compare(std::string(p3[1]))!=0");

  otbControlConditionTestMacro(attr1["Compacity"].compare(std::string(argv[3])) != 0,
                               "Test failed: vertex1->GetAttributesMap()[\"Compacity\"]!=std::string(argv[2])");
  // attributes vector get test
  RCC8VertexType::Pointer vertex2 = RCC8VertexType::New();
  vertex2->SetAttributesMap(attr1);
  otbControlConditionTestMacro(vertex1->GetSegmentationLevel() != vertex2->GetSegmentationLevel(),
                               "Test failed: vertex1->GetSegmentationLevel()!=vertex2->GetSegmentationLevel()");
  otbControlConditionTestMacro(vertex1->GetSegmentationType() != vertex2->GetSegmentationType(),
                               "Test failed: vertex1->GetSegmentationType()!=vertex2->GetSegmentationType()");

  otbControlConditionTestMacro(vertex2->GetPath()->GetVertexList()->GetElement(0) != p1,
                               "Test failed:vertex2->GetPath()->GetVertexList()->GetElement(0)!=p1");
  otbControlConditionTestMacro(vertex2->GetPath()->GetVertexList()->GetElement(1) != p2,
                               "Test failed:vertex2->GetPath()->GetVertexList()->GetElement(1)!=p2");
  otbControlConditionTestMacro(vertex2->GetPath()->GetVertexList()->GetElement(2) != p3,
                               "Test failed:vertex2->GetPath()->GetVertexList()->GetElement(2)!=p3");
  otbControlConditionTestMacro(vertex1->GetCompacity() != vertex2->GetCompacity(),
                               "Test failed: vertex1->GetCompacity()!=vertex2->GetCompacity()");

  return EXIT_SUCCESS;
}
