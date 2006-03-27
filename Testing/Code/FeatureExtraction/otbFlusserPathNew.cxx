/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   24 mars 2006
  Version   :   
  Role      :   
  $Id:$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbFlusserPathFunction.h"
#include "itkPolyLineParametricPath.h"

int otbFlusserPathNew( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        unsigned int  Number;
       
        typedef unsigned char     InputPixelType;
        const   unsigned int      Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >  InputImageType;
	typedef itk::PolyLineParametricPath< Dimension >	        PathType;
	typedef otb::FlusserPathFunction<InputImageType,PathType>       FunctionType;
	typedef FunctionType::RealType                                  RealType;
  

	FunctionType::Pointer function =FunctionType::New();

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

