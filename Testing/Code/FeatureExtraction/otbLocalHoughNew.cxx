/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   12 avril 2006
  Version   :   
  Role      :   Test d'instanciation du filtre local de Hough 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"


#include <iostream>

#include "otbLocalHoughFilter.h"


int otbLocalHoughNew( int argc, char* argv[] )
{
  try 
    {  
    	const char * inputFilename  = argv[1];
                       
        typedef double		                                InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;

        typedef otb::LocalHoughFilter< InputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
       
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


