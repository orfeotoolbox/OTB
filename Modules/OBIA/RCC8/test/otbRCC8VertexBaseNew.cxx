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
#include "otbRCC8VertexBase.h"
#include "otbPolygon.h"

int otbRCC8VertexBaseNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Polygon<>                PathType;
  typedef otb::RCC8VertexBase<PathType> RCC8VertexType;

  // Instantiation
  RCC8VertexType::Pointer rcc8Vertex = RCC8VertexType::New();

  std::cout << rcc8Vertex << std::endl;

  return EXIT_SUCCESS;
}
