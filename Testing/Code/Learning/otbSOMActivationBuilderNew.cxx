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
#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"
#include "itkRGBPixel.h"
#include "itkEuclideanDistance.h"


int otbSOMActivationBuilderNew(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension =2;
    typedef float ComponentType;
    typedef unsigned char OutputPixelType;
    typedef itk::RGBPixel<ComponentType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;

    typedef otb::SOMMap<PixelType,DistanceType,Dimension> MapType;
    typedef otb::Image<PixelType,Dimension> InputImageType;
    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
    typedef otb::SOMActivationBuilder<InputImageType,MapType,OutputImageType> SOMActivationBuilderType;

    // Instantiation
    SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
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
