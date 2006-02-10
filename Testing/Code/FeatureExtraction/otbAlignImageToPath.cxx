/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   06 fevrier 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageToPathAlignFilter.h"


int otbAlignImageToPath( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
	typedef float						PathType;
	typedef float						RealPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< RealPixelType,  Dimension >		RealImageType;


/*	typedef otb::ImageToPathAlignExtract<itk::Image< InputPixelType,  Dimension > ,PathType>  OutputPathType;
        typedef  OutputPathType::InputImageType  InputImageType;
*/


        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
	InputImageType::Pointer ImageIn = InputImageType::New();
	
	
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
	
        writer->SetInput( reader->GetOutput() );
        
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


