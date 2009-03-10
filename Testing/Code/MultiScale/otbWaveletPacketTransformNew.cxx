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
#include "otbHaarOperator.h"
#include "otbStationaryFilterBank.h"
#include "otbWaveletPacketForwardTransform.h"
#include "otbWPCost.h"

int otbWaveletPacketTransformNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::LowPassHaarOperator< PixelType, Dimension > LowPassOperator;
  typedef otb::HighPassHaarOperator< PixelType, Dimension > HighPassOperator;
  typedef otb::StationaryFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator > WaveletFilterType;
  typedef otb::FullyDecomposedWaveletPacketCost< ImageType > CostType;
  typedef otb::WaveletPacketForwardTransform< ImageType, ImageType, WaveletFilterType, CostType >
      FilterType;



  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
