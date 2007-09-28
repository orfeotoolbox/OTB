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
#include "itkImage.h"
#include <iostream>

#include "otbMapProjections.h"
#include "otbMapToMapProjection.h"

int otbProjectionBaseNew( int argc, char* argv[] )
{
  try 
    {        

        otb::AlbersProjection::Pointer lAlbersProjection = otb::AlbersProjection::New();
        otb::AzimEquDistProjection::Pointer lAzimEquDistProjection = otb::AzimEquDistProjection::New();
        typedef otb::MapToMapProjection<otb::AlbersProjection,otb::AzimEquDistProjection> MapToMapProjectionType;
        MapToMapProjectionType::Pointer lMapToMapProjection = MapToMapProjectionType::New();
        
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


