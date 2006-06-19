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

int otbImageFileReaderERS(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

//        typedef std::complex<float>  	                        InputPixelType;
        typedef float  	                        InputPixelType;
//        typedef float                                           OutputPixelType;
        typedef float                                   OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::VectorImage< InputPixelType,  Dimension >        InputImageType;
        typedef itk::VectorImage< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef itk::StreamingImageFilter< InputImageType, OutputImageType >         StreamingType;
	
        StreamingType::Pointer streaming = StreamingType::New();
        ReaderType::Pointer complexReader = ReaderType::New();
 
	complexReader->SetFileName( inputFilename  );
	streaming->SetNumberOfStreamDivisions(100);
	streaming->SetInput(complexReader->GetOutput());
	streaming->Update();


	OutputImageType::Pointer image = streaming->GetOutput();
	OutputImageType::IndexType pixelIndex;
	OutputImageType::PixelType      pixelValue;
	  pixelIndex[0] = 0;      // x position of the pixel
	  pixelIndex[1] = 0;      // y position of the pixel
        pixelValue = image->GetPixel( pixelIndex );
	std::cout << "Number of Elements / Pixels = "<<pixelValue.GetNumberOfElements() <<std::endl;
	std::cout << " PixelValue : ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< pixelValue[0] <<";"<< pixelValue[1]<< std::endl;

	for(unsigned int x = 100 ; x < 200 ; x++ )
	{ 
	  for(unsigned int y = 100 ; y < 200 ; y++ )
	  {
	  pixelIndex[0] = x;      // x position of the pixel
	  pixelIndex[1] = y;      // y position of the pixel

	  pixelValue = image->GetPixel( pixelIndex );
	  std::cout << " PixelValue : ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< pixelValue[0] <<";"<< pixelValue[1]<< std::endl;
          }
	}
/*
  	typedef itk::ComplexToModulusImageFilter< 
                       InputImageType, OutputImageType > ModulusFilterType;

  	ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  	modulusFilter->SetInput( complexReader->GetOutput() );
	modulusFilter->Update(); 
        
	

        WriterType::Pointer writer = WriterType::New();
	
        writer->SetFileName( outputFilename );        
        writer->SetInput( modulusFilter->GetOutput() );
        writer->Update(); 
*/
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

