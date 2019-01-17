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
#include <fstream>
#include "otbVectorData.h"

int otbVectorData(int argc, char * argv[])
{

  typedef otb::VectorData<double, 2>   VectorDataType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef DataNodeType::PointType      PointType;
  typedef DataNodeType::LineType       LineType;
  typedef DataNodeType::PolygonType    PolygonType;

  //Instantiation
  VectorDataType::Pointer data = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();
  DataNodeType::Pointer point = DataNodeType::New();
  DataNodeType::Pointer line = DataNodeType::New();
  DataNodeType::Pointer polygon = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  point->SetNodeType(otb::FEATURE_POINT);
  line->SetNodeType(otb::FEATURE_LINE);
  polygon->SetNodeType(otb::FEATURE_POLYGON);

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

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder, document);
  data->GetDataTree()->Add(point, folder);
  data->GetDataTree()->Add(line, folder);
  data->GetDataTree()->Add(polygon, folder);

  if (argc < 2)
    {
    std::cout << data << std::endl;
    }
  else
    {
    const char *  outfile = argv[1];
    std::ofstream file;
    file.open(outfile);

    file << data << std::endl;
    file.close();

    }

  return EXIT_SUCCESS;
}
