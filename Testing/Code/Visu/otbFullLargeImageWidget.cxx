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
#include "itkNonThreadedShrinkImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "itkVectorRescaleIntensityImageFilter.h"
#include <FL/Fl.H>


int otbFullLargeImageWidget( int argc, char * argv[] )
{
  try 
    { 
      char * filename = argv[1];      
      typedef float PixelType;
      typedef otb::FixedSizeFullImageWidget<PixelType> WidgetType;
      typedef WidgetType::ImageType ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef itk::NonThreadedShrinkImageFilter<ImageType,ImageType> ShrinkFilterType;
      typedef itk::StreamingImageFilter<ImageType,ImageType> StreamingFilterType;
      typedef itk::VectorRescaleIntensityImageFilter<ImageType,ImageType> RescalerType;


      ReaderType::Pointer reader = ReaderType::New();
      ImageType::SizeType size;
      ImageType::IndexType index;
      ImageType::RegionType region;

      
      reader->SetFileName(filename);
      reader->GenerateOutputInformation();
      ShrinkFilterType::Pointer shrink = ShrinkFilterType::New();
      shrink->SetInput(reader->GetOutput());
      shrink->SetShrinkFactors(10);
      shrink->GenerateOutputInformation();
      
      // index.Fill(0);
//       size.Fill(60);
//       region.SetIndex(index);
//       region.SetSize(size);
//       shrink->GetOutput()->SetRequestedRegion(region);
//       shrink->GenerateInputRequestedRegion();
//       std::cout<<"Taille en sortie: "<<shrink->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;  
//       std::cout<<"Taille en entrée: "<<shrink->GetInput()->GetRequestedRegion().GetSize()<<std::endl;
//       shrink->Update();
      
       StreamingFilterType::Pointer streaming = StreamingFilterType::New();
       streaming->SetNumberOfStreamDivisions(size[0]/2);
       streaming->SetInput(shrink->GetOutput());
       
       RescalerType::Pointer rescaler = RescalerType::New();
       rescaler->SetInput(streaming->GetOutput());
       rescaler->SetOutputMaximumMagnitude(255);

       size = shrink->GetOutput()->GetLargestPossibleRegion().GetSize();
       Fl_Window window(size[0],size[1]);

       WidgetType::Pointer widget = WidgetType::New();   
       window.resizable(widget.GetPointer());
       widget->SetInput(rescaler->GetOutput());
       // if(shrink->GetOutput()->GetNumberOfComponentsPerPixel()>=3)
//  	widget->SetViewModelToRGB();
//        else
       widget->SetViewModelToGrayscale();
       widget->Init(0,0,size[0],size[1],"Test Full Image Widget");
       widget->redraw();
       window.end();
       window.show();
       window.show();
       widget->Show();
       widget->redraw();
       Fl::run();
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
