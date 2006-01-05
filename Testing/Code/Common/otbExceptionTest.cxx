/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCommonPrintTest.cxx,v $
  Language:  C++
  Date:      $Date: 2004/01/02 19:14:46 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbExceptionObject.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbExceptionTest(int , char* [])
{
  try
  {
  	otb::ExceptionObject lotbException(__FILE__, __LINE__);
 	lotbException.SetDescription("Ceci est la description de l'erreur ;-)");
 	std::cout << "------------ test ExceptionObject OTB levee ..."<< std::endl;
  	throw lotbException;
  }
  catch( otb::ExceptionObject & err)
  {
    std::cerr << "ExceptionObject OTB attrappee !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_SUCCESS;	
  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
