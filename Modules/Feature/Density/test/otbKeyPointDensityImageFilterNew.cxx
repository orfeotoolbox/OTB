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

#include <stdio.h>

#include "otbKeyPointDensityImageFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"

int otbKeyPointDensityImageFilterNew(int, char*[])
{

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                           ImageType;
  typedef itk::VariableLengthVector<PixelType>                       RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> DetectorType;

  typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, DetectorType> FilterType;

  /**Instantiation of an object*/
  FilterType::Pointer filter =     FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
