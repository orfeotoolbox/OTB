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
#include "otbSRTMHeightAboveMSLFunction.h"
#include "otbImage.h"

int otbSRTMHeightAboveMSLFunction(int argc, char* argv[])
{
  typedef otb::Image<float,2>                          ImageType;
  typedef otb::SRTMHeightAboveMSLFunction<ImageType>   SrtmFunctionType;

  SrtmFunctionType::Pointer srtmDem;

  return EXIT_SUCCESS;

}
