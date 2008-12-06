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
#include "otbVectorData.h"

int otbKMLVectorDataIONew(int argc, char * argv[])
{
  typedef otb::VectorData<> VectorDataType;
  typedef otb::KMLVectorDataIO<VectorDataType> KMLVectorDataIOType;
  KMLVectorDataIOType::Pointer object = KMLVectorDataIOType::New();

  return EXIT_SUCCESS;
}
