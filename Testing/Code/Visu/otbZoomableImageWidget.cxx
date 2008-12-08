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
#include "otbZoomableImageWidget.h"
#include "otbImageFileReader.h"
#include <FL/Fl.H>


int otbZoomableImageWidget( int argc, char * argv[] )
{
  char * filename = argv[1];
  typedef float PixelType;
  typedef otb::ZoomableImageWidget<PixelType> WidgetType;
  typedef WidgetType::ImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  ImageType::SizeType size;
  ImageType::IndexType index;
  ImageType::RegionType region;
  reader->SetFileName(filename);
  reader->Update();
  size=reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  Fl_Window window(size[0],size[1]);

  WidgetType::Pointer widget = WidgetType::New();
  window.resizable(widget.GetPointer());
  widget->SetInput(reader->GetOutput());
  if(reader->GetOutput()->GetNumberOfComponentsPerPixel()>=3)
    widget->SetViewModel(WidgetType::RGB);
  else
    widget->SetViewModel(WidgetType::GRAYSCALE);
  widget->Init(0,0,size[0],size[1],"Test Full Image Widget");
  widget->redraw();
  window.end();
  window.show();
  window.show();
  widget->Show();
  widget->redraw();
  Fl::check();

  for(double zoom  = 1.0;zoom<10.;zoom++)
    {
      Fl::check();

      index[0]=size[0]/2-static_cast<int>(static_cast<double>(size[0]/2)/zoom+0.5);
      index[1]=size[1]/2-static_cast<int>(static_cast<double>(size[1]/2)/zoom+0.5);
      widget->SetZoomUpperLeftCorner(index);
      widget->SetZoomFactor(zoom);
      widget->redraw();
      Fl::wait(0.2);
      Fl::check();
    }

  for(double zoom=10.;zoom>=1.;zoom--)
    {
      Fl::check();
      index[0]=size[0]/2-static_cast<int>(static_cast<double>(size[0])/(2*zoom)+0.5);
      index[1]=size[1]/2-static_cast<int>(static_cast<double>(size[1])/(2*zoom)+0.5);
      widget->SetZoomUpperLeftCorner(index);
      widget->SetZoomFactor(zoom);
      widget->redraw();
      Fl::wait(0.2);
      Fl::check();
    }

  // suppres child, without delete memory.
  // delete memory is ITK respoability, since WidgetType::New()
  window.remove(widget.GetPointer());

  return EXIT_SUCCESS;
}
