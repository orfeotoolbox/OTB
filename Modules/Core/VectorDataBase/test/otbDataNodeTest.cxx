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
#include <cstdlib>

#include "otbDataNode.h"

int otbDataNodeTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef otb::DataNode<double, 2> DataNodeType;
  typedef DataNodeType::PointType   PointType;
  typedef DataNodeType::LineType    LineType;
  typedef DataNodeType::PolygonType PolygonType;

  DataNodeType::Pointer root     = DataNodeType::New();
  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder   = DataNodeType::New();
  DataNodeType::Pointer point    = DataNodeType::New();
  DataNodeType::Pointer line     = DataNodeType::New();
  DataNodeType::Pointer polygon  = DataNodeType::New();

  root->SetNodeType(otb::ROOT);
  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  point->SetNodeType(otb::FEATURE_POINT);
  line->SetNodeType(otb::FEATURE_LINE);
  polygon->SetNodeType(otb::FEATURE_POLYGON);

  root->SetNodeId("ROOT");
  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");
  point->SetNodeId("FEATURE_POINT");
  line->SetNodeId("FEATURE_LINE");
  polygon->SetNodeId("FEATURE_POLYGON");

  PointType p;
  p.Fill(5);

  point->SetPoint(p);

  LineType::Pointer l = LineType::New();
  line->SetLine(l);

  PolygonType::Pointer pol = PolygonType::New();
  polygon->SetPolygonExteriorRing(pol);

  otbControlConditionTestMacro(!root->IsRoot(), "Wrong IsRoot() method return.");
  otbControlConditionTestMacro(root->IsDocument(), "Wrong IsDocument() method return.");
  otbControlConditionTestMacro(root->IsFolder(), "Wrong IsFolder() method return.");
  otbControlConditionTestMacro(root->IsPointFeature(), "Wrong IsPointFeature() method return.");
  otbControlConditionTestMacro(root->IsLineFeature(), "Wrong IsLineFeature() method return.");
  otbControlConditionTestMacro(root->IsPolygonFeature(), "Wrong IsPolygonFeature() method return.");

  otbControlConditionTestMacro(document->IsRoot(), "Wrong IsRoot() method return.");
  otbControlConditionTestMacro(!document->IsDocument(), "Wrong IsDocument() method return.");
  otbControlConditionTestMacro(document->IsFolder(), "Wrong IsFolder() method return.");
  otbControlConditionTestMacro(document->IsPointFeature(), "Wrong IsPointFeature() method return.");
  otbControlConditionTestMacro(document->IsLineFeature(), "Wrong IsLineFeature() method return.");
  otbControlConditionTestMacro(document->IsPolygonFeature(), "Wrong IsPolygonFeature() method return.");

  otbControlConditionTestMacro(folder->IsRoot(), "Wrong IsRoot() method return.");
  otbControlConditionTestMacro(folder->IsDocument(), "Wrong IsDocument() method return.");
  otbControlConditionTestMacro(!folder->IsFolder(), "Wrong IsFolder() method return.");
  otbControlConditionTestMacro(folder->IsPointFeature(), "Wrong IsPointFeature() method return.");
  otbControlConditionTestMacro(folder->IsLineFeature(), "Wrong IsLineFeature() method return.");
  otbControlConditionTestMacro(folder->IsPolygonFeature(), "Wrong IsPolygonFeature() method return.");

  return EXIT_SUCCESS;
}
