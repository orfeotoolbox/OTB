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
#include "otbImageListToRCC8GraphFilter.h"
#include "otbPolygon.h"

int otbImageListToRCC8GraphFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned short                        LabelPixelType;
  typedef otb::Polygon<>                        PathType;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::RCC8VertexBase<PathType>         VertexType;
  typedef otb::RCC8Graph<VertexType>            RCC8GraphType;
  typedef otb::ImageListToRCC8GraphFilter<LabelImageType, RCC8GraphType>
  ImageListToRCC8GraphFilterType;

  // Instantiation
  ImageListToRCC8GraphFilterType::Pointer filter = ImageListToRCC8GraphFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
