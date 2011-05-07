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

#include "otbGamma.h"
#include "otbMacro.h"
#include <iostream>


int otbGammaTest(int argc, char * argv[])
{
  typedef otb::Gamma GammaFunctionType;

  //Instantiation
  GammaFunctionType * gam = new GammaFunctionType();
  const double epsilon = 0.0000000001;
  int output = EXIT_SUCCESS;

  if ( vcl_abs(gam->gamma(1) - 1) > epsilon ) output = EXIT_FAILURE;
  if ( vcl_abs(gam->gamma(0.5) - 1.77245385091) > epsilon ) output = EXIT_FAILURE;
  if ( vcl_abs(gam->gamma(4) - 6) > epsilon ) output = EXIT_FAILURE;

  delete gam;

  return output;
}
