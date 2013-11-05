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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbContinuousMinimumMaximumImageCalculatorNew);
  REGISTER_TEST(otbContinuousMinimumMaximumImageCalculatorTest);
  REGISTER_TEST(otbMeanShiftImageFilterNew);
  REGISTER_TEST(otbMeanShiftImageFilter);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterNew);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilter);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterSpatialStability);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterThreading);
  REGISTER_TEST(otbMeanShiftVectorImageFilterNew);
  REGISTER_TEST(otbMeanShiftVectorImageFilter);
  REGISTER_TEST(otbLabelImageRegionMergingFilter);
  REGISTER_TEST(otbLabelImageRegionPruningFilter);
  REGISTER_TEST(otbFunctionToImageFilterNew);
  REGISTER_TEST(otbFunctionToImageFilter);
  REGISTER_TEST(otbBinaryImageDensityFunctionNew);
  REGISTER_TEST(otbBinaryImageDensityFunction);
  REGISTER_TEST(otbBinaryImageToDensityImageFilterNew);
  REGISTER_TEST(otbBinaryImageToDensityImageFilter);
  REGISTER_TEST(otbEdgeDensityImageFilterNew);
  REGISTER_TEST(otbEdgeDensityImageFilter);
}
