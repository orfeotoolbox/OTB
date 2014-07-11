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



#include "otbFlusserPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"

int otbFlusserPathNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::FlusserPathFunction<PathType>     FunctionType;

  FunctionType::Pointer function = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}
