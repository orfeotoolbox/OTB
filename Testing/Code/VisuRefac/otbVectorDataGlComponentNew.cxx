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
#include "otbVectorData.h"
#include "otbVectorDataGlComponent.h"

int otbVectorDataGlComponentNew(int argc, char * argv[])
{
  typedef otb::VectorData<double,2> VectorDataType;
  typedef otb::VectorDataGlComponent<VectorDataType> VectorDataGlComponentType;

  // Instanatiation
  VectorDataGlComponentType::Pointer comp = VectorDataGlComponentType::New();

  return EXIT_SUCCESS;
}
