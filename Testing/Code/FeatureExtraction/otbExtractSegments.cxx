/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   19 avril 2006
  Version   :   
  Role      : Test du filtre composite ExtractSegmentsImageFilter  
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractSegmentsImageFilter.h"

int otbExtractSegments( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename1  = argv[1];
        const char * inputFilename2  = argv[2];
        const char * outputFilename = argv[3];
        
        unsigned int  PixelSuppressionRadiusX((unsigned int)::atoi(argv[4]));
	double	      PixelSuppressionAngularBeam((double)::atof(argv[5]));
	
	unsigned int  LocalHoughRadiusX((unsigned int)::atoi(argv[6]));
    	unsigned int  LocalHoughRadiusY((unsigned int)::atoi(argv[7]));
    	unsigned int  LocalHoughNumberOfLines((unsigned int)::atoi(argv[8]));
    	
        
        typedef double	                                        InputPixelType;
        typedef unsigned char	   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType1;
        typedef otb::ImageFileReader< InputImageType  >         ReaderType2;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

	
        typedef otb::ExtractSegmentsImageFilter< InputImageType, OutputImageType >   FilterType;
  
        FilterType::Pointer filter = FilterType::New();
        
        // filter parameters
        
        FilterType::SizeType PixelSuppressionRadius;
	PixelSuppressionRadius[0] = PixelSuppressionRadiusX;
	PixelSuppressionRadius[1] = PixelSuppressionRadiusX;
        
	filter->SetPixelSuppressionRadius( PixelSuppressionRadius );	
	filter->SetPixelSuppressionAngularBeam( PixelSuppressionAngularBeam );
	
	FilterType::SizeType LocalHoughRadius;
	LocalHoughRadius[0] = LocalHoughRadiusX;
	LocalHoughRadius[1] = LocalHoughRadiusY;

	filter->SetLocalHoughRadius( LocalHoughRadius );	
	filter->SetLocalHoughNumberOfLines( LocalHoughNumberOfLines );	
	
	// Reader / Writer
	
        ReaderType1::Pointer reader1 = ReaderType1::New();
        ReaderType2::Pointer reader2 = ReaderType2::New();
        WriterType::Pointer writer = WriterType::New();

        reader1->SetFileName( inputFilename1  );
        reader2->SetFileName( inputFilename2  );
        writer->SetFileName( outputFilename );
        
        filter->SetInputImage( reader1->GetOutput() );
        filter->SetInputImageDirection( reader2->GetOutput() );
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

