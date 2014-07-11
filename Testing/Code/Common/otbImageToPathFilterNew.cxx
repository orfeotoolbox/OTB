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
#include "itkPolyLineParametricPath.h"
#include "otbImageToPathFilter.h"
#include "otbImage.h"

int otbImageToPathFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                          PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageToPathFilter<ImageType, PathType> ImageToPathFilterType;

  ImageToPathFilterType::Pointer pathFilter = ImageToPathFilterType::New();

  std::cout << pathFilter << std::endl;

  return EXIT_SUCCESS;
}
