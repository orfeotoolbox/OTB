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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"
#include "otbOrthoRectificationFilter.h"

int otbOrthoRectificationFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Image<double, 2>                                                     ImageType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType;

  OrthoRectifFilterType::Pointer filter = OrthoRectifFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
