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
#include <iostream>

int otbKMLVectorDataIOTestFileReader(int argc, char* argv[])
{
  typedef otb::VectorData<>        VectorDataType;
  typedef otb::KMLVectorDataIO     KMLVectorDataIOType;
  typedef VectorDataType::Pointer  VectorDataPointerType;

  KMLVectorDataIOType::Pointer object = KMLVectorDataIOType::New();
  VectorDataPointerType        data = VectorDataType::New();

  bool lCanRead = object->CanReadFile(argv[1]);
  if (lCanRead == false)
    {
    return EXIT_FAILURE;
    }

  object->SetFileName(argv[1]);
  object->Read(data);

  VectorDataType::ConstPointer constData = static_cast<const VectorDataType *>(data);

  if (argc > 2)
    {
    bool lCanWrite = object->CanWriteFile(argv[2]);
    if (lCanWrite == false)
      {
      return EXIT_FAILURE;
      }
    object->SetFileName(argv[2]);
    object->Write(constData);
    }

  return EXIT_SUCCESS;
}
