/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   27 April 2006
  Version   :   
  Role      :   Test for the SVMMembershipFunction class (load a model) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbSVMMembershipFunction.h"


int otbSVMMembershipFunctionLoadModel( int argc, char* argv[] )
{
  try 
    {         
    typedef std::vector<double>   VectorType;

    typedef otb::SVMMembershipFunction< VectorType >   FunctionType;

    FunctionType::Pointer membershipFunction = FunctionType::New();

    typedef FunctionType::SVMModelType    ModelType;

    ModelType::Pointer model = ModelType::New();

    model->LoadModel( argv[1] );

    membershipFunction->SetModel( model );
        
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


