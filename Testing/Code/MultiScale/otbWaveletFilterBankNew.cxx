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

#include "otbImage.h"
#include "otbSplineBiOrthogonalOperator.h"
#include "otbWaveletFilterBank.h"

int otbWaveletFilterBankNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::LowPassSplineBiOrthogonalOperator< otb::FORWARD, PixelType, Dimension > LowPassOperator;
  typedef otb::HighPassSplineBiOrthogonalOperator< otb::FORWARD, PixelType, Dimension > HighPassOperator;

  typedef otb::WaveletFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator, otb::FORWARD >
    FilterType;
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
