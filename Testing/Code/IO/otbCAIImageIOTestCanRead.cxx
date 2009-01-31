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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbCAIImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbCAIImageIOTestCanRead(int argc, char* argv[])
{
  otb::CAIImageIO::Pointer lCAIImageIO = otb::CAIImageIO::New();
  bool lCanRead = lCAIImageIO->CanReadFile(argv[1]);
  if ( lCanRead == false)
  {
    std::cerr << "Erreur otb::CAIImageIO : impossible d'ouvrir l'image "<<argv[1]<<"."<<std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
