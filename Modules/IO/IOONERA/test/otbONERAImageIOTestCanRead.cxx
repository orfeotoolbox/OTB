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



#include "otbONERAImageIO.h"
#include "itkMacro.h"
#include <iostream>

int otbONERAImageIOTestCanRead(int itkNotUsed(argc), char* argv[])
{
  otb::ONERAImageIO::Pointer lONERAImageIO = otb::ONERAImageIO::New();
  bool                       lCanRead = lONERAImageIO->CanReadFile(argv[1]);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::ONERAImageIO : impossible d'ouvrir l'image " << argv[1] << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
