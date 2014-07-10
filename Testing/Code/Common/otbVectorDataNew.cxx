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

#include "otbMacro.h"

#include "otbVectorData.h"

int otbVectorDataNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef otb::VectorData<double, 2> VectorDataType;

  //Instantiation
  VectorDataType::Pointer data = VectorDataType::New();

  std::cout << data << std::endl;

  return EXIT_SUCCESS;
}
