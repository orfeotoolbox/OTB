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

#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"

int otbImageListToVectorImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> ImageListToVectorImageFilterType;

  // Instantiating object
  ImageListToVectorImageFilterType::Pointer filter = ImageListToVectorImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
