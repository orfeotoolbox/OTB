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
#include "itkExceptionObject.h"

#include "otbMultiChannelRAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

int otbMultiChannelRAndNIRVegetationIndexImageFilterNew(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef otb::VectorImage<unsigned char,Dimension> InputImageType;
      typedef otb::Image<float,Dimension> OutputImageType;
      typedef otb::MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType,OutputImageType> MultiChannelRAndNIRVegetationIndexImageFilterType;

      // Instantiating object
      MultiChannelRAndNIRVegetationIndexImageFilterType::Pointer object = MultiChannelRAndNIRVegetationIndexImageFilterType::New();
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
