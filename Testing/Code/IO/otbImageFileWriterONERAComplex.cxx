// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//#define MAIN


#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkExceptionObject.h"
#include <iostream>
#include "itkComplexToModulusImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageFileWriterONERAComplex(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

        typedef std::complex<float>  	                        InputPixelType;
        typedef float                             		OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< InputImageType >         WriterType;

        typedef itk::StreamingImageFilter< InputImageType, InputImageType >         StreamingType;
	
        StreamingType::Pointer streaming = StreamingType::New();
        ReaderType::Pointer complexReader = ReaderType::New();
 
	complexReader->SetFileName( inputFilename  );
	streaming->SetNumberOfStreamDivisions(100);
	streaming->SetInput(complexReader->GetOutput());
	streaming->Update();


	InputImageType::Pointer image = streaming->GetOutput();
	InputImageType::IndexType pixelIndex;
	InputImageType::PixelType      pixelValue;
	pixelIndex[0] = 0;      // x position of the pixel
	pixelIndex[1] = 0;      // y position of the pixel
        pixelValue = image->GetPixel( pixelIndex );
	std::cout << " PixelValue : ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< pixelValue << std::endl;

        WriterType::Pointer complexWriter = WriterType::New();
	complexWriter->SetFileName( outputFilename  );
	complexWriter->SetInput( streaming->GetOutput()  );
 	complexWriter->Update();

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

