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
#include "itkExceptionObject.h"
#include <cstdlib>

#include "otbPolygon.h"

int otbPolygonNew(int argc, char * argv[])
{
  typedef otb::Polygon<> PolygonType;
  
  // Instantiating object
  PolygonType::Pointer polygon = PolygonType::New();
      
      
  return EXIT_SUCCESS;
}
