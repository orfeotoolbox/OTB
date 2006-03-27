/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   27 mars 2006
  Version   :   
  Role      :   Test d'instanciation du filtre réalisant la somme 
  		associative assymétrique
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>


#include "otbAssociativeSymmetricalSumImageFilter.h"


int otbAssociativeSymmetricalSumNew( int argc, char* argv[] )
{
  try 
    { 
    
        typedef double		                                InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType1;
        typedef itk::Image< InputPixelType,  Dimension >        InputImageType2;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::AssociativeSymmetricalSumImageFilter< InputImageType1, InputImageType2, OutputImageType >   FilterType;
	
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


