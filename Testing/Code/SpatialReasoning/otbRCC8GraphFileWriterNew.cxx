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
#include "otbRCC8GraphFileWriter.h"
#include "otbPolygon.h"

int otbRCC8GraphFileWriterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Polygon<>                          PathType;
  typedef otb::RCC8VertexBase<PathType>           VertexType;
  typedef otb::RCC8Graph<VertexType>              RCC8GraphType;
  typedef otb::RCC8GraphFileWriter<RCC8GraphType> RCC8GraphFileWriterType;

  // Instantiation
  RCC8GraphFileWriterType::Pointer rcc8GraphWriter = RCC8GraphFileWriterType::New();

  std::cout << rcc8GraphWriter << std::endl;

  return EXIT_SUCCESS;
}
