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
#define OTB_DISABLE_GL_USE_ACCEL

#include "otbImageWidget.h"
#include "otbImageFileReader.h"
#include "otbVectorDataGlComponent.h"
#include "otbVectorData.h"

#include <FL/Fl.H>

int otbImageWidgetWithVectorDataGlComponent( int argc, char * argv[] )
{
  const char * infname   =      argv[1];
  const bool   useAccel  = atoi(argv[2]);

  // Widget typedefs
  typedef otb::ImageWidget<> WidgetType;
  typedef WidgetType::InputImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;


  // VectorData component typedef 
  typedef otb::VectorData<double,2> VectorDataType;
  typedef otb::VectorDataGlComponent<VectorDataType> VectorDataGlComponentType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef DataNodeType::PointType PointType;
  typedef DataNodeType::LineType LineType;
  typedef DataNodeType::PolygonType PolygonType;
  
  // Reading the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

  // Create a VectorData 
  VectorDataType::Pointer data = VectorDataType::New();
  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();
  DataNodeType::Pointer point = DataNodeType::New();
  DataNodeType::Pointer line = DataNodeType::New();
  DataNodeType::Pointer polygon = DataNodeType::New();

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");
  point->SetNodeId("FEATURE_POINT");
  line->SetNodeId("FEATURE_LINE");
  polygon->SetNodeId("FEATURE_POLYGON");
  

  // Create a point on the first quarter of the image
  PointType p;
  p[0] = region.GetIndex()[0]+region.GetSize()[0]/4;
  p[1] = region.GetIndex()[1]+region.GetSize()[1]/4;
  point->SetPoint(p);

  // Create a line on the second quarter of the image
  LineType::Pointer l = LineType::New();
  LineType::ContinuousIndexType cindex;
  cindex[0] = region.GetIndex()[0]+region.GetSize()[0]/2;
  cindex[1] = region.GetIndex()[1];
  l->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+region.GetSize()[0];
  cindex[1] = region.GetIndex()[1]+region.GetSize()[1]/2;
  l->AddVertex(cindex);
  line->SetLine(l);

  // Create a complex polygon shape on the remaning of the image
  
  // Outer ring
  PolygonType::Pointer outer = PolygonType::New();
  cindex[0] = region.GetIndex()[0]+region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+9*region.GetSize()[1]/16;
  outer->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+15*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+9*region.GetSize()[1]/16;
  outer->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+15*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+15*region.GetSize()[1]/16;
  outer->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+15*region.GetSize()[1]/16;
  outer->AddVertex(cindex);
  polygon->SetPolygonExteriorRing(outer);

  // Inner ring
  PolygonType::Pointer inner = PolygonType::New(); 
  cindex[0] = region.GetIndex()[0]+2*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+10*region.GetSize()[1]/16;
  inner->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+14*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+10*region.GetSize()[1]/16;
  inner->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+14*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+14*region.GetSize()[1]/16;
  inner->AddVertex(cindex);
  cindex[0] = region.GetIndex()[0]+2*region.GetSize()[0]/16;
  cindex[1] = region.GetIndex()[1]+14*region.GetSize()[1]/16;
  inner->AddVertex(cindex);
  polygon->GetPolygonInteriorRings()->PushBack(inner);

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document,root);
  data->GetDataTree()->Add(folder,document);
  data->GetDataTree()->Add(point,folder);
  data->GetDataTree()->Add(line,folder);
  data->GetDataTree()->Add(polygon,folder);

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  // Set the acceleration mode
  widget->SetUseGlAcceleration(useAccel);

  // Resize it
  widget->resize(0,0,region.GetSize()[0],region.GetSize()[1]);
  // Show it
  widget->show();
  // Refresh display
  Fl::check();
  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();
 
  // Create the VectorDataGlComponent
  VectorDataGlComponentType::Pointer vdgl = VectorDataGlComponentType::New();
  vdgl->SetVectorData(data);

  widget->AddGlComponent(vdgl);
  widget->redraw();
  Fl::run();

  

  return EXIT_SUCCESS;
}
