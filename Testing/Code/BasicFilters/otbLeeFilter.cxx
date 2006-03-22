/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   23 janvier 2005
  Version   :   
  Role      :   Test la classe qui filtre un image a partir de l'algorithme de J.S. LEE
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
#include "otbLeeImageFilter.h"


int otbLeeFilter( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       
        unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
        unsigned int  RadiusY((unsigned int)::atoi(argv[4]));
        double        NbLooks((double)::atof(argv[5]));

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::LeeImageFilter< InputImageType,OutputImageType >   FilterType;
	
	FilterType::SizeType Radius;
	Radius[0]= RadiusX;
	Radius[1]= RadiusY;

        FilterType::Pointer filtreLee = FilterType::New();
        
	filtreLee->SetRadius( Radius );
// OTB-FA-00018-CS
	filtreLee->SetNbLooks(NbLooks);

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filtreLee->SetInput( reader->GetOutput() );
        writer->SetInput( filtreLee->GetOutput() );
        
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


