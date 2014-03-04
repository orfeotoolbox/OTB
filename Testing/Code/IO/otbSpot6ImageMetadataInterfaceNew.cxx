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



#include "itkMacro.h"

#include <iostream>

#include "otbSpot6ImageMetadataInterface.h"

int otbSpot6ImageMetadataInterfaceNew(int argc, char* argv[])
{
  otb::Spot6ImageMetadataInterface::Pointer object = otb::Spot6ImageMetadataInterface::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
