/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

#include "otbSVMImageModelEstimator.h"


int otbSVMImageModelEstimatorNew( int argc, char* argv[] )
{
  try 
    {         
        typedef double                               InputPixelType;
        const   unsigned int        	                    Dimension = 2;

        typedef otb::VectorImage< InputPixelType,  Dimension >        InputImageType;

	typedef otb::Image< InputPixelType,  Dimension >     TrainingImageType;

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


