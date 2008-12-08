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

#include "otbMSTARImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbMSTARImageIOTestCanRead(int argc, char* argv[])
{
  otb::MSTARImageIO::Pointer lMSTARImageIO = otb::MSTARImageIO::New();
  bool lCanRead = lMSTARImageIO->CanReadFile(argv[1]);
  if ( lCanRead == false)
    {
      std::cerr << "Erreur otb::MSTARImageIO : impossible d'ouvrir l'image MSTAR "<<argv[1]<<"."<<std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
