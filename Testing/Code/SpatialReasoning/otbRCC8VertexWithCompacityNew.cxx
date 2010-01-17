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
#include "itkExceptionObject.h"
#include "otbRCC8VertexWithCompacity.h"
#include "otbPolygon.h"

int otbRCC8VertexWithCompacityNew(int argc, char* argv[])
{
  typedef otb::Polygon<> PathType;
  typedef float PrecisionType;
  typedef otb::RCC8VertexWithCompacity<PathType,PrecisionType> RCC8VertexType;

  // Instantiation
  RCC8VertexType::Pointer rcc8Vertex= RCC8VertexType::New();

  return EXIT_SUCCESS;
}
