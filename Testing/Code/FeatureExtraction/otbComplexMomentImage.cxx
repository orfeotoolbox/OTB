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

        typedef itk::Image< InputPixelType,  Dimension >	              InputImageType;
        typedef otb::ImageFileReader< InputImageType  >                       ReaderType;
	  
        typedef std::complex<float>                                           ComplexType;
	typedef otb::ComplexMomentImageFunction<InputImageType,ComplexType>   CMType;
  

        ReaderType::Pointer reader         = ReaderType::New();
	CMType::Pointer function =CMType::New();
	
        reader->SetFileName( inputFilename  );

	reader->Update();
	function->SetInputImage( reader->GetOutput() );
	
	function->SetQ(q);
	function->SetP(p);
	
	InputImageType::IndexType index;
	index[0]=10;
	index[1]=10;
	
	ComplexType Result;
	
        Result = function->EvaluateAtIndex( index );
        std::cout << "function->EvaluateAtIndex( index ): " << Result << std::endl;

	std::cout << " Traitement local:" << std::endl;

	function->SetNeighborhoodRadius(3);
        Result = function->EvaluateAtIndex( index );
        std::cout << "function->EvaluateAtIndex( index ): " << Result << std::endl;

	
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

