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

int otbRCC8Edge(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::RCC8Edge               RCC8EdgeType;
  typedef RCC8EdgeType::RCC8ValueType RCC8ValueType;
  RCC8ValueType value = otb::OTB_RCC8_DC;

  // Instantiation
  RCC8EdgeType::Pointer edge = RCC8EdgeType::New();
  edge->SetValue(value);

  if (edge->GetValue() != value)
    {
    std::cout << "Test failed: edge->GetValue()!=value" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
