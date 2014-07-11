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
#include <cstdlib>

#include "otbPolyLineParametricPathWithValue.h"

int otbPolyLineParametricPathWithValueNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                                     ValueType;
  typedef otb::PolyLineParametricPathWithValue<ValueType, Dimension> PolyLineParametricPathWithValueType;

  // Instantiating object
  PolyLineParametricPathWithValueType::Pointer object = PolyLineParametricPathWithValueType::New();
  object->SetValue(0);

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
