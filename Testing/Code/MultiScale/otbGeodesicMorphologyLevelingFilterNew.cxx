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
#include "otbGeodesicMorphologyLevelingFilter.h"
#include "otbImage.h"

int otbGeodesicMorphologyLevelingFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2>                                                  ImageType;
  typedef otb::GeodesicMorphologyLevelingFilter<ImageType, ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
