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

int otbCAIImageIOTestCanWrite(int argc, char* argv[])
{
  otb::CAIImageIO::Pointer lCAIImageIO = otb::CAIImageIO::New();
  bool lCanRead = lCAIImageIO->CanWriteFile(argv[1]);
  if ( lCanRead == false)
    {
      std::cerr << "Erreur otb::CAIImageIO : impossible de creer l'image "<<argv[1]<<"."<<std::endl;
      return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
