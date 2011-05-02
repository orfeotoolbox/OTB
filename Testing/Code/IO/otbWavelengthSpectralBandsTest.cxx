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

#include "otbFilterFunctionValues.h"
#include "otbWavelengthSpectralBands.h"

using namespace otb;

int otbFilterFunctionValuesNew(int argc, char* argv[])
{

  FilterFunctionValues::Pointer spectSen = FilterFunctionValues::New();

  return EXIT_SUCCESS;
}

int otbWavelengthSpectralBandsNew(int argc, char* argv[])
{

  WavelengthSpectralBands::Pointer spectSen = WavelengthSpectralBands::New();

  return EXIT_SUCCESS;
}

