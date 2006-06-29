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


#include "itkExceptionObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

#include "otbExtractROI.h"

int otbExtractROI( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
        unsigned int  startX((unsigned int)::atoi(argv[3]));
        unsigned int  startY((unsigned int)::atoi(argv[4]));
        unsigned int  sizeX((unsigned int)::atoi(argv[5]));
        unsigned int  sizeY((unsigned int)::atoi(argv[6]));

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::ExtractROI< InputPixelType, 
                                            OutputPixelType >   FilterType;

        typedef FilterType::InputImageType        InputImageType;
        typedef FilterType::OutputImageType       OutputImageType;

        typedef itk::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
        FilterType::Pointer filter = FilterType::New();
        
//        filter->SetStartX( startX );
        filter->SetStartY( startY );
        filter->SetSizeX( sizeX );
        filter->SetSizeY( sizeY );

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filter->SetInput( reader->GetOutput() );
        writer->SetInput( filter->GetOutput() );
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


  return EXIT_SUCCESS;
}


