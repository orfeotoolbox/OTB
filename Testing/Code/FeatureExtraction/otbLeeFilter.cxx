/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   23 janvier 2005
  Version   :   
  Role      :   Test la classe qui filtre un image a partir de l'algorithme de J.S. LEE
  $Id$

=========================================================================*/


#include "itkExceptionObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "otbLeeImageFilter.h"


int otbLeeFilter( int argc, char ** argv )
{
#if 0
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
        unsigned int  RadiusX(unsigned int)::atoi(argv[3]));
        unsigned int  RadiusY((unsigned int)::atoi(argv[4]));
        double        NbVues((double)::atoi(argv[5]));

        typedef float  	                                        InputPixelType;
        typedef float     	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::LeeImageFilter< InputImageType,OutputImageType >   FilterType;
	
	FilterType::SizeType Radius;
	Radius[0]= RadiusX;
	Radius[1]= RadiusY;

        FilterType::Pointer FiltreLee = FilterType::New();
        
        FiltreLee->SetRadius( Radius );
	FiltreLee->SetNbVues(NbVues);

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

#endif
  return EXIT_SUCCESS;
}


