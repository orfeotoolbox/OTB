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
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygonListToRCC8GraphFilter.h"
#include "otbRCC8GraphFileWriter.h"

// typedefs
typedef otb::Polygon<>                                                    PolygonType;
typedef otb::ObjectList<PolygonType>                                      PolygonListType;
typedef otb::RCC8VertexBase<PolygonType>                                  VertexType;
typedef otb::RCC8Graph<VertexType>                                        RCC8GraphType;
typedef otb::PolygonListToRCC8GraphFilter<PolygonListType, RCC8GraphType> RCC8GraphFilterType;
typedef otb::RCC8GraphFileWriter<RCC8GraphType>                           GraphWriterType;


PolygonType::VertexType Vertex(double i,double j)
{
  PolygonType::VertexType vertex;
  vertex[0] = i;
  vertex[1] = j;

  return vertex;
}


int otbPolygonListToRCC8GraphFilter(int itkNotUsed(argc), char* argv[])
{
  char * outfile = argv[1];

  PolygonListType::Pointer regions = PolygonListType::New();

  RCC8GraphFilterType::SegmentationRangesType ranges;

  ranges.push_back(0);

  PolygonType::Pointer polygon = PolygonType::New();
  polygon->AddVertex(Vertex(0,0));
  polygon->AddVertex(Vertex(2,0));
  polygon->AddVertex(Vertex(2,6));
  polygon->AddVertex(Vertex(0,6));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(1,1));
  polygon->AddVertex(Vertex(4,1));
  polygon->AddVertex(Vertex(4,5));
  polygon->AddVertex(Vertex(1,5));
  regions->PushBack(polygon);

  ranges.push_back(2);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(3,1));
  polygon->AddVertex(Vertex(4,1));
  polygon->AddVertex(Vertex(4,2));
  polygon->AddVertex(Vertex(3,2));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(5,1));
  polygon->AddVertex(Vertex(8,1));
  polygon->AddVertex(Vertex(8,5));
  polygon->AddVertex(Vertex(5,5));
  regions->PushBack(polygon);

  ranges.push_back(4);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(6,2));
  polygon->AddVertex(Vertex(7,2));
  polygon->AddVertex(Vertex(7,3));
  polygon->AddVertex(Vertex(6,3));
  regions->PushBack(polygon);

  polygon = PolygonType::New();
  polygon->AddVertex(Vertex(8,1));
  polygon->AddVertex(Vertex(9,1));
  polygon->AddVertex(Vertex(9,5));
  polygon->AddVertex(Vertex(8,5));
  regions->PushBack(polygon);

  // Filter instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  filter->SetInput(regions);
  filter->SetSegmentationRanges(ranges);

  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfile);
  writer->Update();

  return EXIT_SUCCESS;
}
