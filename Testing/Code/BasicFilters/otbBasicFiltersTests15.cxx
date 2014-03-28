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


//
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbNDVIDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbNDVIDataNodeFeatureFunction);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunction);
  REGISTER_TEST(otbSpectralAngleDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbSpectralAngleDataNodeFeatureFunction);
  REGISTER_TEST(otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction);
  REGISTER_TEST(otbDBOverlapDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbDBOverlapDataNodeFeatureFunction);
  REGISTER_TEST(otbMaximumAutocorrelationFactorImageFilterNew);
  REGISTER_TEST(otbMaximumAutocorrelationFactorImageFilter);
  REGISTER_TEST(otbThresholdVectorImageFilterNew);
  REGISTER_TEST(otbThresholdVectorImageFilterTest);
  REGISTER_TEST(otbClampVectorImageFilterNew);
  REGISTER_TEST(otbClampVectorImageFilterTest);
  REGISTER_TEST(otbClampImageFilterNew);
  REGISTER_TEST(otbClampImageFilterTest);
  REGISTER_TEST(otbTileImageFilterNew);
  REGISTER_TEST(otbTileImageFilter);
  REGISTER_TEST(otbStreamingHistogramVectorImageFilterNew);
}
