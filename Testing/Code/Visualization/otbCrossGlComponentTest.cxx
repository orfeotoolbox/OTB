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

#include "otbCrossGlComponent.h"

int otbCrossGlComponentNew(int argc, char * argv[])
{
  otb::CrossGlComponent::Pointer component = otb::CrossGlComponent::New();

  return EXIT_SUCCESS;
}


