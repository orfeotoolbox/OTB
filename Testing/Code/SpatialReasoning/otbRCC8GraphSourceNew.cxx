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
#include "otbRCC8GraphSource.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygon.h"

int otbRCC8GraphSourceNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Polygon<>                      PathType;
  typedef otb::RCC8VertexBase<PathType>       VertexType;
  typedef otb::RCC8Graph<VertexType>          RCC8GraphType;
  typedef otb::RCC8GraphSource<RCC8GraphType> RCC8GraphSourceType;

  // Instantiation
  RCC8GraphSourceType::Pointer rcc8GraphSource = RCC8GraphSourceType::New();

  std::cout << rcc8GraphSource << std::endl;

  return EXIT_SUCCESS;
}
