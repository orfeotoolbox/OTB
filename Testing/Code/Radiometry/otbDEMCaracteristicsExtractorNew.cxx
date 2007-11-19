/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodOperatorImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007/08/10 14:34:02 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "otbDEMCaracteristicsExtractor.h"
#include "otbImage.h"


int otbDEMCaracteristicsExtractorNew(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::Image<PixelType, Dimension> InputImageType;
      typedef otb::DEMCaracteristicsExtractor< InputImageType, InputImageType > DEMCaracteristicsExtractorType;

      DEMCaracteristicsExtractorType::Pointer filter = DEMCaracteristicsExtractorType::New();
    }
  
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 
  
  catch( ... ) 
    { 
      std::cout << "Unknown exception thrown !" << std::endl; 
      return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
