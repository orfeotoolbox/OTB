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

#include "otbKMLVectorDataIO.h"
#include <iostream>

int otbKMLVectorDataIOTestCanWrite(int itkNotUsed(argc), char* argv[])
{
  typedef otb::KMLVectorDataIO KMLVectorDataIOType;
  KMLVectorDataIOType::Pointer object = KMLVectorDataIOType::New();
  bool                         lCanWrite = object->CanWriteFile(argv[1]);
  if (lCanWrite == false)
    {
    std::cerr << "Erreur otb::KMLVectorDataIO : impossible to create the file " << argv[1] << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
