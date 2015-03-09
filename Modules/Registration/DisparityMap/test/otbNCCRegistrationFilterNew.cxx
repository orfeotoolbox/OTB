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
#include "otbNCCRegistrationFilter.h"
#include "otbImage.h"

int otbNCCRegistrationFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int ImageDimension = 2;

  typedef double                              PixelType;
  typedef itk::Vector<double, ImageDimension> DisplacementPixelType;

  //Allocate Images
  typedef otb::Image<PixelType, ImageDimension> MovingImageType;
  typedef otb::Image<PixelType, ImageDimension> FixedImageType;
  typedef otb::Image<DisplacementPixelType,
      ImageDimension>         DisplacementFieldType;

  //Create the filter
  typedef otb::NCCRegistrationFilter<FixedImageType,
      MovingImageType,
      DisplacementFieldType>
  RegistrationFilterType;

  // Instantiation
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
