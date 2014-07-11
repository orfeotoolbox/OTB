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
#include "otbRCC8Edge.h"
#include <cstdlib>

int otbRCC8EdgeNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::RCC8Edge RCC8EdgeType;

  // Instantiation
  RCC8EdgeType::Pointer rcc8 = RCC8EdgeType::New();

  std::cout << rcc8 << std::endl;
  return EXIT_SUCCESS;
}
