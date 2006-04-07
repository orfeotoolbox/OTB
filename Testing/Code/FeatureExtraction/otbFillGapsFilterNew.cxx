/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   7 avril 2006
  Version   :   
  Role      :   Test élémentaire de la classe otb::FillGapsFilter
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbFillGapsFilter.h"

int otbFillGapsFilterNew( int argc, char ** argv )
{
  try 
    { 
	typedef otb::FillGapsFilter FillGapsFilterType;
	FillGapsFilterType::Pointer fillgaps = FillGapsFilterType::New();
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

