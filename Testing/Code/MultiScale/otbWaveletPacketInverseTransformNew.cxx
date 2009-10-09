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

#include "otbImage.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletPacketTransform.h"
#include "otbWaveletPacketDecompositionCosts.h"

int otbWaveletPacketInverseTransformNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;

  /* Wavelet choice */
  //const otb::MotherWaveletOperatorEnum wvltID = otb::HAAR;
  const otb::MotherWaveletOperatorEnum wvltID = otb::DB8;

  /* Inverse Transformation */
  typedef otb::WaveletOperator< wvltID, otb::INVERSE, PixelType, Dimension >
    InverseWaveletOperator;
  typedef otb::WaveletFilterBank< ImageType, ImageType, InverseWaveletOperator, otb::INVERSE >
    InverseFilterBank;
  typedef otb::WaveletPacketTransform< ImageType, ImageType, InverseFilterBank, otb::INVERSE >
    InvFilterType;
  
  InvFilterType::Pointer invFilter = InvFilterType::New();

  return EXIT_SUCCESS;
}

