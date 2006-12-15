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
#include "itkRGBPixel.h"
#include "itkEuclideanDistance.h"
#include "itkImageToListAdaptor.h"

int otbSOMNew(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension = 2;
    typedef float ComponentType;
    typedef itk::RGBPixel<ComponentType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;
    typedef otb::SOMMap<PixelType,DistanceType,Dimension> SOMMapType;
    typedef otb::SOM<SOMMapType,SOMMapType> SOMType;

    // Instantiation
    SOMType::Pointer som = SOMType::New();
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
