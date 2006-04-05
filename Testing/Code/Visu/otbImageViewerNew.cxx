/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   Test du ImageViewer
  Role      :   
  $Id$

=========================================================================*/
#include <stdlib.h>
#include "itkExceptionObject.h"

#include "otbImageViewer.h"

int otbImageViewerNew( int argc, char ** argv )
{
  try 
    { 

        typedef otb::ImageViewer<unsigned char,double>    ViewerType;
        ViewerType::Pointer  lViewer = ViewerType::New();

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


