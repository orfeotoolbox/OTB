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
#include "otbRCC8Graph.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygon.h"

int otbRCC8GraphNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Polygon<>                PathType;
  typedef otb::RCC8VertexBase<PathType> VertexType;
  typedef otb::RCC8Graph<VertexType>    RCC8GraphType;

  // Instantiation
  RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();

  std::cout << rcc8Graph << std::endl;

  return EXIT_SUCCESS;
}
