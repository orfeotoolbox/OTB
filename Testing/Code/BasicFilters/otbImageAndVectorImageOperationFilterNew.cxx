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
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageAndVectorImageOperationFilter.h"

int otbImageAndVectorImageOperationFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double                                                                                     PixelType;
  typedef otb::Image<PixelType, 2>                                                                   ScalarImageType;
  typedef otb::VectorImage<PixelType, 2>                                                             VectorImageType;
  typedef otb::ImageAndVectorImageOperationFilter<ScalarImageType, VectorImageType, VectorImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
