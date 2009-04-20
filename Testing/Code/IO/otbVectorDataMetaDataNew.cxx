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

#include "otbVectorDataMetaData.h"

int otbVectorDataMetaDataNew(int argc, char * argv[])
{

  otb::VectorDataMetaData::Pointer object = otb::VectorDataMetaData::New();

  return EXIT_SUCCESS;
}