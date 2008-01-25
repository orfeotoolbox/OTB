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
#include "otbFullResolutionImageWidget.h"
#include "otbImageFileReader.h"
#include "otbImageWidgetPolygonForm.h"
#include <FL/Fl.H>


int otbImageWidgetPolygonForm( int argc, char * argv[] )
{
  char * filename = argv[1];      
  typedef float PixelType;
  typedef otb::FullResolutionImageWidget<PixelType> WidgetType;
  //typedef otb::ZoomableImageWidget<PixelType> WidgetType;
  typedef WidgetType::ImageType                     ImageType;
  typedef otb::ImageFileReader<ImageType>           ReaderType; 
  typedef otb::ImageWidgetPolygonForm<>             WidgetPolyFormType;   
  typedef WidgetPolyFormType::PolygonType                   PolyType;
  typedef WidgetPolyFormType::VertexType                    VertexType;
  typedef WidgetPolyFormType::VertexListType                VertexListType;
  typedef WidgetPolyFormType::ContinuousIndexType ContinuousIndexType;

  ReaderType::Pointer         reader         = ReaderType::New();
  WidgetPolyFormType::Pointer wigdetpolyForm = WidgetPolyFormType::New();
  
  // Polygons instanciation
  // Reading vertices from command line
  int cpt = 2;
  
  while ( argv[cpt] != NULL && argv[cpt+1]!= NULL)
    {
      ContinuousIndexType newVertex; 
      newVertex[0]=atof(argv[cpt]);
      newVertex[1]=atof(argv[cpt+1]);
      wigdetpolyForm->GetPolygon()->AddVertex(newVertex);
      ++cpt;
      ++cpt;
    }
  
  ImageType::SizeType size;
  ImageType::IndexType index;
  ImageType::RegionType region;
  
  index[0]=0;
  index[1]=0;
  size[0]=200;
  size[1]=200;
  reader->SetFileName(filename);
  reader->Update();
  
  Fl_Window window(size[0],size[1]);
  
  WidgetType::Pointer         widget         = WidgetType::New();
  window.resizable(widget.GetPointer());
  widget->SetInput(reader->GetOutput());
  widget->GetFormList()->PushBack( wigdetpolyForm );
  widget->SetFormOverlayVisible(true);

  if(reader->GetOutput()->GetNumberOfComponentsPerPixel()>=3)
    {
      widget->SetViewModelToRGB();
    }
  else
    {
      widget->SetViewModelToGrayscale();
    }
  
  widget->SetUpperLeftCorner(index);
  widget->Init(0,0,size[0],size[1],"Test Full Resolution Widget");
  widget->redraw();
  window.end();
  window.show();
  window.show();
  widget->Show();
  widget->redraw();
  Fl::check();  


return EXIT_SUCCESS;
}
