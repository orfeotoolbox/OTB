/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   09 mars 2006
  Version   :   
  Role      :   Test la classe du filtre de detection de contours 
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
#include "otbTouziEdgeDetector.h"


int otbTouziEdgeDetector( int argc, char* argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        const char * outputFilenameDir = argv[4];

        unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
        
        typedef unsigned char                                   InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::TouziEdgeDetector< InputImageType,OutputImageType >   FilterType;
	
	FilterType::SizeType Radius;
	Radius[0] = RadiusX;
	Radius[1] = RadiusX;
	
        FilterType::Pointer filtreTouzi = FilterType::New();
        
	filtreTouzi->SetRadius( Radius );
	
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
        WriterType::Pointer writer_dir = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filtreTouzi->SetInput( reader->GetOutput() );
        writer->SetInput( filtreTouzi->GetOutput() );
        
        writer->Update();
        
        writer_dir->SetFileName( outputFilenameDir );
        writer_dir->SetInput( filtreTouzi->GetOutputDirection() );
        writer_dir->Update(); 

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


