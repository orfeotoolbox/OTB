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
#include "otbGeodesicMorphologyDecompositionImageFilter.h"
#include "otbImage.h"
#include "itkBinaryBallStructuringElement.h"

int otbGeodesicMorphologyDecompositionImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2> ImageType;
  typedef itk::BinaryBallStructuringElement<double,
      2>                                                  StructuringElementType;
  typedef otb::GeodesicMorphologyDecompositionImageFilter<ImageType, ImageType,
      StructuringElementType>
  GeodesicMorphologyDecompositionImageFilterType;

  // Instantiating object
  GeodesicMorphologyDecompositionImageFilterType::Pointer filter = GeodesicMorphologyDecompositionImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
