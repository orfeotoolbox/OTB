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
#include "otb_9_7_Operator.h"
#include "otbStationaryFilterBank.h"

int otbStationaryFilterBankNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::LowPass_9_7_Operator< PixelType, Dimension > LowPassOperator;
  typedef otb::HighPass_9_7_Operator< PixelType, Dimension > HighPassOperator;
  typedef otb::StationaryFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator > FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
