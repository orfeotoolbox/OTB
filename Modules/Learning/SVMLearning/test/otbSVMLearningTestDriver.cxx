/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbSVMImageClassificationWithRuleFilter);
  REGISTER_TEST(otbLabelMapSVMClassifierNew);
  REGISTER_TEST(otbLabelMapSVMClassifier);
  REGISTER_TEST(otbSVMPointSetModelEstimatorTrain);
  REGISTER_TEST(otbExhaustiveExponentialOptimizerNew);
  REGISTER_TEST(otbSVMImageModelEstimatorModelAccessor);
  REGISTER_TEST(otbSVMImageClassificationFilter);
  REGISTER_TEST(otbSVMModelNew);
  REGISTER_TEST(otbSVMImageModelEstimatorNew);
  REGISTER_TEST(otbSVMImageModelEstimatorTrainOneClass);
  REGISTER_TEST(otbSVMCrossValidationCostFunctionNew);
  REGISTER_TEST(otbSVMModelLoadSave);
  REGISTER_TEST(otbSVMClassifierPointSet);
  REGISTER_TEST(otbSVMImageClassificationWithRuleFilterNew);
  REGISTER_TEST(otbSVMImageModelEstimatorTrain);
  REGISTER_TEST(otbObjectDetectionClassifierNew);
  REGISTER_TEST(otbObjectDetectionClassifier);
  REGISTER_TEST(otbSVMClassifierNew);
  REGISTER_TEST(otbSVMSampleListModelEstimatorNew);
  REGISTER_TEST(otbSVMSampleListModelEstimatorTest);
  REGISTER_TEST(otbSVMModelAccessor);
  REGISTER_TEST(otbSVMModelLoad);
  REGISTER_TEST(otbSVMImageClassificationFilterNew);
  REGISTER_TEST(otbSVMPointSetModelEstimatorNew);
  REGISTER_TEST(otbSVMMarginSamplerNew);
  REGISTER_TEST(otbExhaustiveExponentialOptimizerTest);
}
