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
  REGISTER_TEST(otbROIdataConversionNew);
  REGISTER_TEST(otbROIdataConversion);
  REGISTER_TEST(otbSVMCrossValidationCostFunctionNew);
  REGISTER_TEST(otbExhaustiveExponentialOptimizerNew);
  REGISTER_TEST(otbExhaustiveExponentialOptimizerTest);
  REGISTER_TEST(otbListSampleGeneratorNew);
  REGISTER_TEST(otbListSampleGenerator);
  REGISTER_TEST(otbConfusionMatrixCalculatorNew);
  REGISTER_TEST(otbConfusionMatrixCalculatorSetListSamples);
  REGISTER_TEST(otbConfusionMatrixCalculatorWrongSize);
  REGISTER_TEST(otbConfusionMatrixCalculatorUpdate);
  REGISTER_TEST(otbConfusionMatrixCalculatorUpdateWithBaseline);
  REGISTER_TEST(otbShiftScaleSampleListFilterNew);
  REGISTER_TEST(otbShiftScaleSampleListFilter);
  REGISTER_TEST(otbGaussianAdditiveNoiseSampleListFilterNew);
  REGISTER_TEST(otbGaussianAdditiveNoiseSampleListFilter);
  REGISTER_TEST(otbConcatenateSampleListFilterNew);
  REGISTER_TEST(otbConcatenateSampleListFilter);
  REGISTER_TEST(otbListSampleToBalancedListSampleFilterNew);
  REGISTER_TEST(otbListSampleToBalancedListSampleFilter);
}
