/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   27 April 2006
  Version   :   
  Role      :   Test for the SVMModel class (loading a model) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbSVMModel.h"


int otbSVMModelLoad( int argc, char* argv[] )
{
  try 
    {         
        typedef unsigned char                                   InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;

	typedef itk::Vector< float, 3 > MeasurementVectorType ;
        typedef otb::SVMModel< MeasurementVectorType >   ModelType;
	
	
        ModelType::Pointer svmModel = ModelType::New();

	svmModel->LoadModel(argv[1]);
        
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown exception !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


