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

#include "itkExceptionObject.h"

#include "otbRAndBAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"

int otbRAndBAndNIRVegetationIndexImageFilterNew(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef unsigned char PixelType;
      typedef otb::Image<PixelType,Dimension> InputRImageType;
      typedef otb::Image<PixelType,Dimension> InputBImageType;
      typedef otb::Image<PixelType,Dimension> InputNIRImageType;
      typedef otb::Image<PixelType,Dimension> OutputImageType;

      typedef otb::RAndBAndNIRVegetationIndexImageFilter<InputRImageType,InputBImageType,InputNIRImageType,OutputImageType>  RAndBAndNIRVegetationIndexImageFilterType;

      // Instantiating object
      RAndBAndNIRVegetationIndexImageFilterType::Pointer object = RAndBAndNIRVegetationIndexImageFilterType::New();
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
