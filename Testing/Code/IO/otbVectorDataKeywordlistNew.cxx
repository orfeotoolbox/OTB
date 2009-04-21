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

#include "otbVectorDataKeywordlist.h"

int otbVectorDataKeywordlistNew(int argc, char * argv[])
{

  otb::VectorDataKeywordlist::Pointer object = otb::VectorDataKeywordlist::New();

  return EXIT_SUCCESS;
}