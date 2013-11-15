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

#include "otbCurlHelperInterface.h"

namespace otb {

bool
CurlHelperInterface::IsCurlAvailable()
{
  bool curlAvailable = false;

#ifdef OTB_USE_CURL
  curlAvailable = true;
#endif
  return curlAvailable;
}

bool
CurlHelperInterface::IsCurlMultiAvailable()
{
  bool curlMultiAvailable = false;

#ifdef OTB_CURL_MULTI_AVAILABLE
  curlMultiAvailable = true;
#endif
  return curlMultiAvailable;
}

}
