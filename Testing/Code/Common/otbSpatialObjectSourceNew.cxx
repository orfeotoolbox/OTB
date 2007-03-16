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

#include "otbSpatialObjectSource.h"
#include "itkGroupSpatialObject.h"

int otbSpatialObjectSourceNew(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension = 2;
    typedef itk::GroupSpatialObject<Dimension> SpatialObjectType;
    typedef otb::SpatialObjectSource<SpatialObjectType> SpatialObjectSourceType;

    // Instantiation
    SpatialObjectSourceType::Pointer source = SpatialObjectSourceType::New();
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
