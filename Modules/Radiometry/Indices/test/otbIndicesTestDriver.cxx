/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
  REGISTER_TEST(otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbNDVIDataNodeFeatureFunction);
  REGISTER_TEST(otbLandsatTMIndexNDSI);
  REGISTER_TEST(otbLandsatTMIndexBIO);
  REGISTER_TEST(otbLAIFromReflectancesLinear);
  REGISTER_TEST(otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbWDVIRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbGAndRIndexImageFilter);
  REGISTER_TEST(otbRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbIBMultiChannelGAndRAndNIRIndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexMIR2);
  REGISTER_TEST(otbLandsatTMIndexNDVI);
  REGISTER_TEST(otbISURAndNIRIndexImageFilter);
  REGISTER_TEST(otbLAIFromNDVILogarithmic);
  REGISTER_TEST(otbMultiChannelRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexVis);
  REGISTER_TEST(otbWaterSqrtSpectralAngleImageFilter);
  REGISTER_TEST(otbLandsatTMIndexBright);
  REGISTER_TEST(otbMultiChannelRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMBrightTest);
  REGISTER_TEST(otbLandsatTMThinCloudTest);
  REGISTER_TEST(otbISUMultiChannelRAndNIRIndexImageFilter);
  REGISTER_TEST(otbAVIRAndGAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbMultiChannelGAndRIndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexMIRTIR);
  REGISTER_TEST(otbLandsatTMIndexMIR1);
  REGISTER_TEST(otbRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMVegetationTest);
  REGISTER_TEST(otbEVIRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbNDBITM4AndTM5IndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexNDBBBI);
  REGISTER_TEST(otbIBGAndRAndNIRIndexImageFilter);
  REGISTER_TEST(otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbNDBIMultiChannelTM4AndTM5IndexImageFilter);
  REGISTER_TEST(otbNDWIWaterIndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexTIR);
  REGISTER_TEST(otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMLinguisticLabels);
  REGISTER_TEST(otbNDWIMultiChannelWaterIndexImageFilter);
  REGISTER_TEST(otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMIndexNDSIVis);
  REGISTER_TEST(otbLandsatTMIndexNIR);
  REGISTER_TEST(otbSetASetBRAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMKernelSpectralRules);
  REGISTER_TEST(otbLandsatTMKernelSpectralRulesWithImage);
  REGISTER_TEST(otbLandsatTMIndexNDBSI);
  REGISTER_TEST(otbTSARVIRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbLandsatTMThickCloudTest);
}
