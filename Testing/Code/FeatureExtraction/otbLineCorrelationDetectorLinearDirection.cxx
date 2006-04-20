/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   5 avril 2006
  Version   :   
  Role      :   Test du filtre de detection de lignes par correlation
  $Id$

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
#include "otbLineCorrelationDetectorImageFilter.h"

#include "itkLinearInterpolateImageFunction.h"

int otbLineCorrelationDetectorLinearDirection( int argc, char* argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

	// Largeur de la ligne à detecter = 2*WidthLine+1
        unsigned int  WidthLine((unsigned int)::atoi(argv[3]));
        // Longueur de la ligne à detecter = 2*LengthLine+1
        unsigned int  LengthLine((unsigned int)::atoi(argv[4]));
        
        typedef unsigned char                                   InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef itk::LinearInterpolateImageFunction< InputImageType, double >	InterpolatorType;
        typedef otb::LineCorrelationDetectorImageFilter< InputImageType, OutputImageType, InterpolatorType >   FilterType;
	
        FilterType::Pointer FilterLineCorrelation = FilterType::New();
        
	FilterLineCorrelation->SetWidthLine( WidthLine );
	FilterLineCorrelation->SetLengthLine( LengthLine );
	
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        FilterLineCorrelation->SetInput( reader->GetOutput() );
        writer->SetInput( FilterLineCorrelation->GetOutputDirections() );
        
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


