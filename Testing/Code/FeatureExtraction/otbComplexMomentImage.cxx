/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   10 mars 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbComplexMomentImageFunction.h"

int otbComplexMomentImage( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        unsigned int  p((unsigned int)::atoi(argv[2]));
        unsigned int  q((unsigned int)::atoi(argv[3]));
       
        typedef unsigned char                                   InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  

	typedef otb::ComplexMomentImageFunction<InputImageType>   CMType;

        ReaderType::Pointer reader         = ReaderType::New();
	
        reader->SetFileName( inputFilename  );
	
	InputImageType::ConstPointer image = reader->GetOutput();
		
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

