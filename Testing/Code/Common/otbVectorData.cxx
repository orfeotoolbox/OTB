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
