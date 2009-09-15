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

#include "otbImage.h"
#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbRadarFunctors.h"

int otbTerraSarCalibrationCplxImageFilterNew(int argc, char * argv[])
{
  typedef std::complex<double>                                                             CplxType;
  typedef otb::Image<CplxType, 2>                                                          CplxImageType;
  typedef otb::Functor::TerraSarCalibrationComplexImageFunctor< CplxType, CplxType >       FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter< CplxImageType,CplxImageType, FunctorType> CplxFilterType;

  CplxFilterType::Pointer filterCplx = CplxFilterType::New();

  return EXIT_SUCCESS;
}
