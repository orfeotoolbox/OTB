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
#include <FL/Fl.H>


int otbFullResolutionImageWidget( int argc, char * argv[] )
{
  char * filename = argv[1];      
  typedef float PixelType;
  typedef otb::FullResolutionImageWidget<PixelType> WidgetType;
  typedef WidgetType::ImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
      
  ReaderType::Pointer reader = ReaderType::New();
  ImageType::SizeType size;
  ImageType::IndexType index;
  ImageType::RegionType region;

  index[0]=0;
  index[1]=0;
  size[0]=1;
  size[1]=1;
  reader->SetFileName(filename);
  reader->Update();

  Fl_Window window(size[0],size[1]);

  WidgetType::Pointer widget = WidgetType::New();   
  window.resizable(widget.GetPointer());
  widget->SetInput(reader->GetOutput());
  if(reader->GetOutput()->GetNumberOfComponentsPerPixel()>=3)
    widget->SetViewModel(WidgetType::RGB);
  else
    widget->SetViewModel(WidgetType::GRAYSCALE);
  widget->SetUpperLeftCorner(index);
  widget->Init(0,0,size[0],size[1],"Test Full Resolution Widget");
  widget->redraw();
  window.end();
  window.show();
  window.show();
  widget->Show();
  widget->redraw();
  Fl::check();
      
  int StepX =(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]-size[0])/100;
  int StepY =(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]-size[1])/100;
  for(int i = 0;i<=100;i+=5)
    {
      Fl::check();
      window.resize(0,0,size[0]+i*StepX,size[1]+i*StepY);
      Fl::wait(0.2);
      Fl::check();
    }
      
  for(int i = 100;i>=0;i-=5)
    {
      Fl::check();
      window.resize(0,0,size[0]+i*StepX,size[1]+i*StepY);
      Fl::wait(0.2);
      Fl::check();
    }
  // suppres child, without delete memory.
  // delete memory is ITK respoability, since WidgetType::New()
  window.remove(widget.GetPointer());

  
  
  return EXIT_SUCCESS;
}
