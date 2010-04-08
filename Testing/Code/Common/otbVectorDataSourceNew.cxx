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
#include "itkExceptionObject.h"

#include "otbVectorDataSource.h"
#include "otbVectorData.h"

int otbVectorDataSourceNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                     InputPixelType;
  typedef otb::VectorData<InputPixelType, Dimension> InputVectorDataType;
  typedef otb::VectorDataSource<InputVectorDataType> VectorDataSourceType;

  // Instantiating VectorDataSource object
  VectorDataSourceType::Pointer VectorData = VectorDataSourceType::New();

  return EXIT_SUCCESS;
}
