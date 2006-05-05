/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   27 April 2006
  Version   :   
  Role      :   Test for the SVMPointSetModelEstimator class (instanciation) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkPointSet.h"
#include <iostream>

#include "otbSVMPointSetModelEstimator.h"
#include "otbSVMMembershipFunction.h"


int otbSVMPointSetModelEstimatorNew( int argc, char* argv[] )
{
  try 
    {         
    typedef std::vector<double>                             InputPixelType;
    typedef double                             LabelPixelType;
    const   unsigned int        	                    Dimension = 2;

    typedef itk::PointSet< InputPixelType,  Dimension >
                                                       MeasurePointSetType;

    typedef itk::PointSet< LabelPixelType,  Dimension >     LabelPointSetType;


    typedef otb::SVMPointSetModelEstimator< MeasurePointSetType,
                                           LabelPointSetType >   EstimatorType;
	
	
    EstimatorType::Pointer estimator = EstimatorType::New();
        
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


