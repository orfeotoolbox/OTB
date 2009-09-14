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

#include "otbTerraSarRadarBrightnessImageFilter.h"
#include "otbImage.h"

int otbTerraSarRadarBrightnessComplexImageFilterNew(int argc, char * argv[])
{
  typedef otb::Image< std::complex<double>, 2 >                                                     ComplexImageType;
  typedef otb::TerraSarRadarBrightnessComplexImageFunctor<ComplexImageType,ComplexImageType>        FunctorType;
  typedef otb::TerraSarRadarBrightnessImageFilter<ComplexImageType, ComplexImageType, FunctorType > FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
