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
#include <iostream>

#include "otbSVMModel.h"


int otbSVMModelNew( int argc, char* argv[] )
{
  try 
    {         
        typedef unsigned char                                   InputPixelType;
	typedef unsigned char                                   LabelPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;

        typedef otb::SVMModel< InputPixelType, LabelPixelType >   ModelType;
	
	
        ModelType::Pointer svmModel = ModelType::New();
        
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


