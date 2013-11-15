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


#include <iostream>

#include "otbPolarimetricData.h"

int otbPolarimetricDataNew(int argc, char * argv[])
{
  otb::PolarimetricData::Pointer  data = otb::PolarimetricData::New();

  return EXIT_SUCCESS;
}



int otbPolarimetricDataTest(int argc, char * argv[])
{
  otb::PolarimetricData::Pointer  data = otb::PolarimetricData::New();

  data->SetArchitectureType(static_cast<otb::ArchitectureType>(5));

  data->DetermineArchitecture(3, true, false);

  bool tab[4];
  tab[0] = true;
  tab[1] = false;
  tab[2] = false;
  tab[3] = true;

  data->DetermineArchitecture(tab);

  return EXIT_SUCCESS;
}
