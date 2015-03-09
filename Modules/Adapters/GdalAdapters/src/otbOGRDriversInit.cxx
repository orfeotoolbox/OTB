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

#include "otbOGRDriversInit.h"
#include "ogr_api.h"

/*static*/
otb::ogr::Drivers & otb::ogr::Drivers::Init()
{
  static Drivers theInstance;
  return theInstance;
}

otb::ogr::Drivers::Drivers()
{
  OGRRegisterAll();
}

otb::ogr::Drivers::~Drivers()
{
  OGRCleanupAll();
}

