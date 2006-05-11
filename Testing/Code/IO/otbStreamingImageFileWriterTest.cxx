// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include <iostream>

#include "itkImage.h"
#include "itkStreamingImageFilter.h"
//#include "itkMeanImageFilter.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbStreamingImageFileWriterTest (int argc, char* argv[])
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
        typedef otb::StreamingImageFileWriter< OutputImageType >         WriterType;
        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
 
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
//        streamer->SetNumberOfStreamDivisions( 10 );
        
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
