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
#include "itkMacro.h"
#include "otbVectorDataToGISTableFilter.h"
#include "otbPostGISConnectionImplementation.h"
#include "otbGISTable.h"
#include "otbVectorData.h"

int otbVectorDataToGISTableFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::VectorData<PixelType> VectorDataType;

  typedef otb::PostGISConnectionImplementation              BdConnection;
  typedef otb::GISTable<BdConnection, PixelType, Dimension> GISTableType;

  typedef otb::VectorDataToGISTableFilter<VectorDataType, GISTableType> VectorDataToGISTableFilterType;

  VectorDataToGISTableFilterType::Pointer vectorData = VectorDataToGISTableFilterType::New();

  std::cout << vectorData << std::endl;

  return EXIT_SUCCESS;
}
