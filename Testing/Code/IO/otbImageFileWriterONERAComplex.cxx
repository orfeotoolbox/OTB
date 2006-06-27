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

#if 0
	InputImageType::Pointer image = streaming->GetOutput();
	InputImageType::IndexType pixelIndex;
	InputImageType::PixelType      pixelValue;
	pixelIndex[0] = 0;      // x position of the pixel
	pixelIndex[1] = 0;      // y position of the pixel
        pixelValue = image->GetPixel( pixelIndex );
	std::cout << " PixelValue : ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< pixelValue << std::endl;

	InputImageType::PixelType       complexValue;
	for(unsigned int x = 100 ; x < 110 ; x++ )
	{ 
	  for(unsigned int y = 100 ; y < 110 ; y++ )
	  {
	  pixelIndex[0] = x;      // x position of the pixel
	  pixelIndex[1] = y;      // y position of the pixel

	  complexValue = image->GetPixel( pixelIndex );
	  std::cout << " PixelValue : ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< complexValue << std::endl;
          }
	}

        WriterType::Pointer complexWriter = WriterType::New();
	complexWriter->SetFileName( outputFilename  );
	complexWriter->SetInput( streaming->GetOutput()  );
 	complexWriter->Update();

	std::cout << "Re-lecture " <<std::endl;
        ReaderType::Pointer complexReader2 = ReaderType::New();
        StreamingType::Pointer streaming2 = StreamingType::New();
	complexReader2->SetFileName( outputFilename  );
	streaming2->SetInput(complexReader2->GetOutput());
	streaming2->Update();

	InputImageType::Pointer image2 = streaming2->GetOutput();

	pixelIndex[0] = 0;      // x position of the pixel
	pixelIndex[1] = 0;      // y position of the pixel
        pixelValue = image->GetPixel( pixelIndex );
	std::cout << " PixelValue Write: ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< pixelValue << std::endl;
	for(unsigned int x = 100 ; x < 110 ; x++ )
	{ 
	  for(unsigned int y = 100 ; y < 110 ; y++ )
	  {
	  pixelIndex[0] = x;      // x position of the pixel
	  pixelIndex[1] = y;      // y position of the pixel

	  complexValue = image->GetPixel( pixelIndex );
	  std::cout << " PixelValue Write: ("<< pixelIndex[0]<<" , "<< pixelIndex[1]<<" ) = "<< complexValue << std::endl;
          }
	}

#endif

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

