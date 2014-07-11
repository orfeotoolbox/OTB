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
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"

int otbWaveletFilterBankNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  /* Wavelet choice */
  //const otb::Wavelet::Wavelet wvltID = otb::Wavelet::HAAR;
  const otb::Wavelet::Wavelet wvltID = otb::Wavelet::DB4;
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::FORWARD, PixelType, Dimension> WaveletOperator;

  typedef otb::WaveletFilterBank<ImageType, ImageType, WaveletOperator, otb::Wavelet::FORWARD>
  FilterType;
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
