/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Version   :   
  Role      :   Tester le portage de la fonction FLST de mégawave vers l'OTB
  $Id: $

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
 
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkExceptionObject.h"
#include "otbImageFileReader.h"
#include "otbTreeSource.h"
#include "otbImageToTreeFilter.h"

int otbFlstTest( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
//	typedef float						PathType;
	typedef float						RealPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< RealPixelType,  Dimension >		RealImageType;
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;

        ReaderType::Pointer reader = ReaderType::New();	
        reader->SetFileName( inputFilename  );
    
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
