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
#include "otbImageMultiSegmentationToRCC8GraphFilter.h"

int otbImageMultiSegmentationToRCC8GraphFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned short                        LabelPixelType;
  typedef otb::Polygon<>                        PolygonType;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::RCC8VertexBase<PolygonType>      VertexType;
  typedef otb::RCC8Graph<VertexType>            RCC8GraphType;
  typedef otb::ImageMultiSegmentationToRCC8GraphFilter<LabelImageType, RCC8GraphType>
  RCC8GraphFilterType;

  // Instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
