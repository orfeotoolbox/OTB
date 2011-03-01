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

#include "otbVectorDataIOFactory.h"
#include "otbVectorData.h"

typedef short  Type1;
typedef double Type2;

typedef otb::VectorData<Type1>                VectorData1Type;
typedef otb::VectorData<Type2>                VectorData2Type;

typedef otb::VectorDataIOFactory<VectorData1Type> VectorData1IOFactoryType;
typedef otb::VectorDataIOFactory<VectorData2Type> VectorData2IOFactoryType;

typedef VectorData1IOFactoryType::VectorDataIOBasePointerType VectorData1IOBasePointerType;
typedef VectorData2IOFactoryType::VectorDataIOBasePointerType VectorData2IOBasePointerType;

int otbVectorDataIOFactory(int argc, char * argv[])
{
  VectorData1IOBasePointerType io1 = VectorData1IOFactoryType::CreateVectorDataIO(argv[1], VectorData1IOFactoryType::ReadMode);
  std::cout << io1 << std::endl;

  VectorData2IOBasePointerType io2 = VectorData2IOFactoryType::CreateVectorDataIO(argv[1], VectorData2IOFactoryType::ReadMode);
  std::cout << io2 << std::endl;

  return EXIT_SUCCESS;
}

