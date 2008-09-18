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
#include <iostream>

int otbKMLVectorDataIOTestCanWrite(int argc, char* argv[])
{
  typedef otb::VectorData<> VectorDataType;
  typedef otb::KMLVectorDataIO<VectorDataType> KMLVectorDataIOType;
  KMLVectorDataIOType::Pointer object = KMLVectorDataIOType::New();
  bool lCanRead = object->CanWriteFile(argv[1]);
  if ( lCanRead == false)
    {
      std::cerr << "Erreur otb::KMLVectorDataIO : impossible to create the file "<<argv[1]<<"."<<std::endl;
      return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
