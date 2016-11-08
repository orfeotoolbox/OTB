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

#include "otbLineSpatialObject.h"

int otbLineSpatialObjectNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::LineSpatialObject<2> LineSpatialObjectType;

  // Instantiating object
  LineSpatialObjectType::Pointer object = LineSpatialObjectType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
