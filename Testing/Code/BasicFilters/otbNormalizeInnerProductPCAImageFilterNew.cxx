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
#include "otbNormalizeInnerProductPCAImageFilter.h"
#include "otbVectorImage.h"

int otbNormalizeInnerProductPCAImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double PixelType;
  const unsigned int Dimension = 2;

  // Typedef
  typedef otb::VectorImage<PixelType, Dimension>                         ImageType;
  typedef otb::NormalizeInnerProductPCAImageFilter<ImageType, ImageType> FilterType;

  // Instantiation object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
