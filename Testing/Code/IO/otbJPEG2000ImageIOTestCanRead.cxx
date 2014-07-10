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



#include "otbJPEG2000ImageIO.h"
#include "itkMacro.h"
#include <iostream>

int otbJPEG2000ImageIOTestCanRead(int itkNotUsed(argc), char* argv[])
{
  otb::JPEG2000ImageIO::Pointer lJPEG2000ImageIO = otb::JPEG2000ImageIO::New();
  bool                          lCanRead = lJPEG2000ImageIO->CanReadFile(argv[1]);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::JPEG2000ImageIO : impossible to open the image file JPEG2000 " << argv[1] << "." <<
    std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
