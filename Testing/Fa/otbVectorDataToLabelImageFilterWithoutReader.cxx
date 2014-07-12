/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbVectorData.h"

#include "otbVectorDataToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

typedef otb::Image<unsigned int, 2>                           ImageType;
typedef otb::VectorData<>                                     VectorDataType;
typedef otb::ImageFileReader<ImageType>                       ReaderType;
typedef otb::ImageFileWriter<ImageType>              WriterType;
typedef otb::VectorDataToLabelImageFilter<VectorDataType,
                                          ImageType>          RasterizationFilterType;

typedef VectorDataType::DataNodeType              DataNodeType;

typedef DataNodeType::PointType                   PointType;
typedef DataNodeType::LineType                    LineType;
typedef DataNodeType::PolygonType                 PolygonType;
typedef LineType::VertexType                      VertexType;

int main(int itkNotUsed(argc), char* argv[])
{

  //OGRRegisterAll();

  // Create vector Data

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
   p1.Fill(1);

   VertexType p3;
   p3.Fill(11);

   VertexType p2;
   p2[0] = 1;
   p2[1] = 11;

   VertexType p4;
   p4[0] = 11;
   p4[1] = 1;

   PolygonType::Pointer poly1 = PolygonType::New();
   poly1->AddVertex(p1);
   poly1->AddVertex(p2);
   poly1->AddVertex(p3);
   poly1->AddVertex(p4);
   poly1->AddVertex(p1);
   polygon1->SetPolygonExteriorRing(poly1);
   polygon1->SetFieldAsInt("DN",32);


   p1.Fill(51);

   p3.Fill(61);

   p2[0] = 51;
   p2[1] = 61;

   p4[0] = 61;
   p4[1] = 51;


  PolygonType::Pointer poly2 = PolygonType::New();
  poly2->AddVertex(p1);
  poly2->AddVertex(p2);
  poly2->AddVertex(p3);
  poly2->AddVertex(p4);
  poly2->AddVertex(p1);
  polygon2->SetLine(poly2);
  polygon2->SetFieldAsInt("DN",64);




   DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

   data->GetDataTree()->Add(document, root);
   data->GetDataTree()->Add(folder, document);
   data->GetDataTree()->Add(polygon1, folder);
   data->GetDataTree()->Add(polygon2, folder);

   data->Update();

  // rasterize
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  rasterization->AddVectorData(data);



  rasterization->SetBurnAttribute("DN");


  // image infos
  ImageType::SizeType size;
  size[0] = 100;
  size[1] = 100;

  ImageType::PointType origin;
  origin[0]=0;
  origin[1]=0;

  ImageType::SpacingType spacing;
  spacing[0]=1;
  spacing[1]=1;

  rasterization->SetOutputSize(size);
  rasterization->SetOutputOrigin(origin);
  rasterization->SetOutputSpacing(spacing);

  WriterType::Pointer writer  = WriterType::New();
  writer->SetFileName(argv[1]);
  writer->SetInput(rasterization->GetOutput());
  writer->Update();

return EXIT_SUCCESS;
}
