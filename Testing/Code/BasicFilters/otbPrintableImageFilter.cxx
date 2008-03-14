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

#include "itkExceptionObject.h"
#include "otbStreamingImageFileWriter.h"
#include "otbVectorImage.h"

#include "otbPrintableImageFilter.h"

int otbPrintableImageFilter( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef double InputPixelType;
        const   unsigned int Dimension = 2;

        typedef otb::VectorImage< InputPixelType,  Dimension >        InputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;

        typedef otb::PrintableImageFilter< InputImageType>   FilterType;
        typedef FilterType::OutputImageType OutputImageType;
        
        typedef otb::StreamingImageFileWriter< OutputImageType >         WriterType;
	

        FilterType::Pointer printableImageFilter = FilterType::New();
        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        printableImageFilter->SetInput( reader->GetOutput() );
        printableImageFilter->SetChannel(3);
        printableImageFilter->SetChannel(2);
        printableImageFilter->SetChannel(1); 
        writer->SetInput( printableImageFilter->GetOutput() );
        
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


