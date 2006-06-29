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

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAssociativeSymmetricalSumImageFilter.h"


int otbAssociativeSymmetricalSum( int argc, char* argv[] )
{
  try 
    { 
        const char * inputFilename1  = argv[1];
        const char * inputFilename2  = argv[2];
        const char * outputFilename = argv[3];

        
        typedef double		                                InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType1;
        typedef itk::Image< InputPixelType,  Dimension >        InputImageType2;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType1  >         ReaderType1;
        typedef otb::ImageFileReader< InputImageType2  >         ReaderType2;
        typedef otb::ImageFileWriter< OutputImageType >          WriterType;

        typedef otb::AssociativeSymmetricalSumImageFilter< InputImageType1, InputImageType2, OutputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
        ReaderType1::Pointer reader1 = ReaderType1::New();
        ReaderType2::Pointer reader2 = ReaderType2::New();
        WriterType::Pointer writer = WriterType::New();

        reader1->SetFileName( inputFilename1  );
        reader2->SetFileName( inputFilename2  );
        writer->SetFileName( outputFilename );
        
        filter->SetInput1( reader1->GetOutput() );
        filter->SetInput2( reader2->GetOutput() );
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
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


