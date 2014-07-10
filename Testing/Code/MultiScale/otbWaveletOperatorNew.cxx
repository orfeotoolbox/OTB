/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbWaveletOperator.h"

int otbWaveletOperatorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const otb::Wavelet::WaveletDirection directionID = otb::Wavelet::INVERSE;
  const otb::Wavelet::Wavelet          wvltID = otb::Wavelet::SPLINE_BIORTHOGONAL_4_4;

  const int Dimension = 2;
  typedef double PixelType;

  typedef otb::WaveletOperator<wvltID, directionID, PixelType, Dimension>
  WaveletOperator;

  typedef WaveletOperator::LowPassOperator LowPassOperatorType;
  LowPassOperatorType h_filter;

  typedef WaveletOperator::HighPassOperator HighPassOperatorType;
  HighPassOperatorType g_filter;

  return EXIT_SUCCESS;
}
