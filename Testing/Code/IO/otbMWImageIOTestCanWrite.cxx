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

#include "otbMWImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbMWImageIOTestCanWrite(int argc, char* argv[])
{
  otb::MWImageIO::Pointer lMWImageIO = otb::MWImageIO::New();
  bool lCanRead = lMWImageIO->CanWriteFile(argv[1]);
  if ( lCanRead == false)
  {
    std::cerr << "Error  image "<<argv[1]<<" is not a MW image."<<std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
