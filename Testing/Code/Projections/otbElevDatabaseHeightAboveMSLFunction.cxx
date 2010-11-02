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

#include "otbMacro.h"
#include "otbElevDatabaseHeightAboveMSLFunction.h"
#include "otbImage.h"

int otbElevDatabaseHeightAboveMSLFunction(int argc, char* argv[])
{
  typedef otb::Image<float,2>                          ImageType;
  typedef otb::ElevDatabaseHeightAboveMSLFunction<ImageType>   SrtmFunctionType;

  SrtmFunctionType::Pointer srtmDem;

  return EXIT_SUCCESS;

}
