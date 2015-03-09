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

#include "otbUtils.h"
#include <sstream>

namespace otb
{

namespace Utils
{

bool IsLonLatValid(double lon, double lat)
{
  if (lon < -180.0) return false;
  if (lon > 180.0) return false;
  if (lat < -90.0) return false;
  if (lat > 90.0) return false;
  return true;
}

}

}
