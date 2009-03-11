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
#include "otbWaveletForwardTransform.h"
#include "otbHaarOperator.h"
#include "otbStationaryFilterBank.h"

int otbWaveletTransformNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::LowPassHaarOperator< PixelType, Dimension > LowPassOperator;
  typedef otb::HighPassHaarOperator< PixelType, Dimension > HighPassOperator;
  typedef otb::StationaryFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator > WaveletFilterType;
  typedef otb::WaveletForwardTransform< ImageType, ImageType, WaveletFilterType > FilterType;


  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
