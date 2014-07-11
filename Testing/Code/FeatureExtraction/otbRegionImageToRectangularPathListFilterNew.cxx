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
#include "otbRegionImageToRectangularPathListFilter.h"
#include "otbImage.h"
#include "itkPolyLineParametricPath.h"

int otbRegionImageToRectangularPathListFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned short InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::PolyLineParametricPathWithValue<double, Dimension> PathType;
  typedef otb::RegionImageToRectangularPathListFilter<InputImageType, PathType>
  RectangleListFilterType;

  RectangleListFilterType::Pointer filter = RectangleListFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
