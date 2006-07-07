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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageFileReader.h"
#include "otbFrostImageFilter.h"

#include "itkExceptionObject.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkMeanImageFilter.h"


int otbFrostFilter( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       
        unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
        unsigned int  RadiusY((unsigned int)::atoi(argv[4]));
        double        Deramp ((double)::atof(argv[5]));

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::FrostImageFilter< InputImageType,OutputImageType >   FilterType;
	
	FilterType::SizeType Radius;
	Radius[0]= RadiusX;
	Radius[1]= RadiusY;

        FilterType::Pointer filtreFrost = FilterType::New();
        
	filtreFrost->SetRadius( Radius );
	filtreFrost->SetDeramp( Deramp );

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filtreFrost->SetInput( reader->GetOutput() );
        writer->SetInput( filtreFrost->GetOutput() );
        
        writer->Update(); 

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
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


