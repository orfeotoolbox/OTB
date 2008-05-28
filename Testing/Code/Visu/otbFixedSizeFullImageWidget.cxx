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
#include "otbFixedSizeFullImageWidget.h"
#include "otbImageFileReader.h"
#include <FL/Fl.H>


int otbFixedSizeFullImageWidget( int argc, char * argv[] )
{
  try 
    { 
      char * filename = argv[1];      
      typedef float PixelType;
      typedef otb::FixedSizeFullImageWidget<PixelType> WidgetType;
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
	{
	  widget->SetViewModel(WidgetType::RGB);
	}
      else
	{
	  widget->SetViewModel(WidgetType::GRAYSCALE);
	}
      widget->Init(0,0,size[0],size[1],"Test Full Image Widget");
      widget->redraw();
      window.end();
      window.show();
      window.show();
      widget->Show();
      widget->redraw();
      Fl::check();

      for(int i = 0;i<=400;i+=40)
	{
	  Fl::check();
	  window.resize(0,0,i,i);
	  Fl::wait(0.2);
	  Fl::check();
	}
      
      for(int i = 400;i>=0;i-=40)
	{
	  Fl::check();
	  window.resize(0,0,i,i);
	  Fl::wait(0.2);
	  Fl::check();
	}
      // suppres child, without delete memory.
      // delete memory is ITK respoability, since WidgetType::New()
      window.remove(widget.GetPointer());
    } 
  
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
      std::cout << "Exception levee inconnue !" << std::endl; 
      return EXIT_FAILURE;
    } 
  
  
  return EXIT_SUCCESS;
}
