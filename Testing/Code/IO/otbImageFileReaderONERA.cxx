// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//#define MAIN


#include "otbImage.h"
#include "itkVectorImage.h"
#include "itkExceptionObject.h"
#include <iostream>
#include "itkComplexToModulusImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

int otbImageFileReaderONERATest(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

        typedef float  	                                           InputPixelType;
        typedef float                                              OutputPixelType;
        const   unsigned int        	                           Dimension = 2;

        typedef otb::VectorImage< InputPixelType,  Dimension >     InputImageType;
        typedef otb::VectorImage< OutputPixelType, Dimension >     OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >            ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >            WriterType;

        typedef itk::StreamingImageFilter< InputImageType, 
					   OutputImageType >       StreamingType;
	
        StreamingType::Pointer streaming = StreamingType::New();
        ReaderType::Pointer complexReader = ReaderType::New();
 
	complexReader->SetFileName( inputFilename  );
	streaming->SetNumberOfStreamDivisions(100);
	streaming->SetInput(complexReader->GetOutput());
	streaming->Update();
        
        typedef otb::MultiChannelExtractROI< OutputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;

        ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

	extractROIFilter->SetStartX( 10 );
	extractROIFilter->SetStartY( 10 );
	extractROIFilter->SetSizeX( 100 );
	extractROIFilter->SetSizeY( 100 );
        extractROIFilter->SetInput( streaming->GetOutput() );        
	
        WriterType::Pointer writer = WriterType::New();
	
        writer->SetFileName( outputFilename );        
        writer->SetInput( extractROIFilter->GetOutput() );
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

