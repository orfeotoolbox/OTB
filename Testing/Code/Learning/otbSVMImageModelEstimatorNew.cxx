/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   27 April 2006
  Version   :   
  Role      :   Test for the SVMImageModelEstimator class (instanciation) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkVectorImage.h"
#include <iostream>

#include "otbSVMImageModelEstimator.h"
#include "otbSVMMembershipFunction.h"


int otbSVMImageModelEstimatorNew( int argc, char* argv[] )
{
  try 
    {         
        typedef double                               InputPixelType;
        const   unsigned int        	                    Dimension = 2;

        typedef itk::VectorImage< InputPixelType,  Dimension >        InputImageType;

	typedef itk::Image< InputPixelType,  Dimension >     TrainingImageType;

	typedef std::vector<double>   VectorType;

        typedef otb::SVMImageModelEstimator< InputImageType,
                                           TrainingImageType >   EstimatorType;
	
	
        EstimatorType::Pointer svmModel = EstimatorType::New();
        
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


