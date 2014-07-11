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

#include "otbVectorDataSource.h"
#include "otbVectorData.h"

int otbVectorDataSourceNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::VectorData<>                     VectorDataType;
  typedef otb::VectorDataSource<VectorDataType> VectorDataSourceType;
  VectorDataSourceType::Pointer object = VectorDataSourceType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
