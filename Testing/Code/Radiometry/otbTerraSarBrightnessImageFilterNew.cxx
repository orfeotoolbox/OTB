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
#include "itkExceptionObject.h"

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbImage.h"

int otbTerraSarBrightnessImageFilterNew(int argc, char * argv[])
{
  typedef otb::Image<double, 2>                                                  ImageType;
  typedef otb::Image<std::complex<double>, 2>                                    ComplexImageType; 

  typedef otb::TerraSarBrightnessImageFilter<ImageType, ImageType>               FilterType;
  typedef otb::TerraSarBrightnessImageFilter<ComplexImageType, ComplexImageType> ComplexFilterType;


  FilterType::Pointer filter = FilterType::New();
  ComplexFilterType::Pointer complexFilter = ComplexFilterType::New();

  return EXIT_SUCCESS;
}
