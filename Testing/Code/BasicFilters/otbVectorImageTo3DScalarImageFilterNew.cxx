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
#include "otbVectorImage.h"
#include "otbImage.h"

int otbVectorImageTo3DScalarImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int BiDimension  = 2;
  const unsigned int TriDimension  = 3;

  typedef double PixelType;

  typedef otb::VectorImage<PixelType, BiDimension> VectorImageType;
  typedef otb::Image<PixelType, TriDimension>      ImageType;

  typedef otb::VectorImageTo3DScalarImageFilter<VectorImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
