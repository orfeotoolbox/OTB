/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   29 mars 2006
  Version   :   
  Role      : Test du filtre composite AssymetricFusionOfLineDetector  
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkLinearInterpolateImageFunction.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAssymetricFusionOfLineDetectorImageFilter.h"

int otbAssymetricFusionOfLineDetector( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
	// Width of the linear feature = 2*WidthLine+1
        unsigned int  WidthLine((unsigned int)::atoi(argv[3]));
        // Length of the linear feature = 2*LengthLine+1
        unsigned int  LengthLine((unsigned int)::atoi(argv[4]));
        
        typedef unsigned char                                   InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef itk::LinearInterpolateImageFunction< InputImageType, double >	InterpolatorType;
	
        typedef otb::AssymetricFusionOfLineDetectorImageFilter< InputImageType, OutputImageType, InterpolatorType >   FilterType;
  
        FilterType::Pointer FilterAssSymSum = FilterType::New();
        
	FilterAssSymSum->SetWidthLine( WidthLine );
	FilterAssSymSum->SetLengthLine( LengthLine );
	
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        FilterAssSymSum->SetInput( reader->GetOutput() );
        writer->SetInput( FilterAssSymSum->GetOutput() );
        
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

