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

#include "otbThresholdImageToPointSetFilter.h"
#include "itkPointSet.h"
#include "otbImage.h"

#include <iostream>

int otbThresholdImageToPointSetFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                       PixelType;
  typedef otb::Image<PixelType, Dimension>    ImageType;
  typedef itk::PointSet<PixelType, Dimension> PointSetType;

  typedef otb::ThresholdImageToPointSetFilter<ImageType, PointSetType> FilterThresholdType;

  FilterThresholdType::Pointer filter = FilterThresholdType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
