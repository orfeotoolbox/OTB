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
#include "otbImageToVectorImageCastFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbImageToVectorImageCastFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

  typedef otb::ImageToVectorImageCastFilter<ImageType, VectorImageType> VectorImageCastFilterType;

  // Instantiation
  VectorImageCastFilterType::Pointer caster = VectorImageCastFilterType::New();

  std::cout << caster << std::endl;

  return EXIT_SUCCESS;
}
