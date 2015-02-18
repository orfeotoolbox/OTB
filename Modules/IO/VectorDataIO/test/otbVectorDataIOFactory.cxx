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

int otbVectorDataIOFactory(int itkNotUsed(argc), char * argv[])
{
  typedef otb::VectorDataIOFactory VectorData1IOFactoryType;
  typedef VectorData1IOFactoryType::VectorDataIOBasePointerType VectorData1IOBasePointerType;

  VectorData1IOBasePointerType io1 = VectorData1IOFactoryType::CreateVectorDataIO(argv[1], VectorData1IOFactoryType::ReadMode);
  std::cout << io1 << std::endl;

  return EXIT_SUCCESS;
}
