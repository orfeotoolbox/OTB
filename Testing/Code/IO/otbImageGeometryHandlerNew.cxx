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

#include "otbImageGeometryHandler.h"

int otbImageGeometryHandlerNew(int argc, char * argv[])
{
  // Instantiating object
  otb::ImageGeometryHandler::Pointer object = otb::ImageGeometryHandler::New();

  return EXIT_SUCCESS;
}
