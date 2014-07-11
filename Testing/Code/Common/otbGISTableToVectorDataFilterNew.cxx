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

#include "otbVectorData.h"
#include <fstream>
#include <iostream>

#include "otbGISTableToVectorDataFilter.h"
#include "otbPostGISConnectionImplementation.h"

int otbGISTableToVectorDataFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef unsigned char                               PType;
  typedef otb::VectorData<>                           VectorDataType;
  typedef otb::PostGISConnectionImplementation        GISConnectionType;
  typedef otb::GISTable<GISConnectionType, double, 2> GISTableType;

  //Instantiation
  GISTableType::Pointer data = GISTableType::New();

  typedef otb::GISTableToVectorDataFilter<GISTableType, VectorDataType> GISTableToVectorDataFilter;

  GISTableToVectorDataFilter::Pointer filter = GISTableToVectorDataFilter::New();

  //data->itkGetObjectMacro ( )

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
