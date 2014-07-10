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

#include "otbImageFittingPolygonListFilter.h"
#include "otbPolygon.h"
#include "otbImage.h"
#include <cstdlib>

int otbImageFittingPolygonListFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef otb::Polygon<double>          PolygonType;
  typedef otb::Image<double, Dimension> ImageType;

  typedef otb::ImageFittingPolygonListFilter<PolygonType, ImageType> FittingPolygonType;

  // Instantiating object
  FittingPolygonType::Pointer filter = FittingPolygonType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
