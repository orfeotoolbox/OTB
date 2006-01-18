// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include <iostream>

#include "otbImageFileReader.h"

int otbImageFileReaderTest(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
 
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        writer->SetInput( reader->GetOutput() );
        writer->Update(); 
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

