/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
  REGISTER_TEST(otbReduceSpectralResponseSVMClassifier);
  REGISTER_TEST(otbProspectTransTest);
  REGISTER_TEST(otbReduceSpectralResponseClassifierRAndNIR);
  REGISTER_TEST(otbSatelliteRSR);
  REGISTER_TEST(otbSatelliteRSRCheckValue);
  REGISTER_TEST(otbSatelliteRSRSolarIrradianceCheckValue);
  REGISTER_TEST(otbSurfaceReflectanceToReflectanceFilterTest);
  REGISTER_TEST(otbImageSimulationMethodWithSpatialisationTest);
  REGISTER_TEST(otbImageSimulationMethodSVMClassif);
  REGISTER_TEST(otbReduceSpectralResponse);
  REGISTER_TEST(otbReduceSpectralResponseSimpleValues);
  REGISTER_TEST(otbAtmosphericEffects);
  REGISTER_TEST(otbProspectReflTest);
  REGISTER_TEST(otbLabelMapToSimulatedImageFilterTest);
  REGISTER_TEST(otbImageSimulationMethodKMeansClassif);
  REGISTER_TEST(otbSpectralResponse);
  REGISTER_TEST(otbAtmosphericCorrectionsRSRSVMClassifier);
  REGISTER_TEST(otbSpatialisationTest);
  REGISTER_TEST(otbImageSimulationMethodWithVectorDataTest);
  REGISTER_TEST(otbSailReflVTest);
  REGISTER_TEST(otbSailReflHTest);
  REGISTER_TEST(otbFilterFunctionValuesSpectralResponseTest);
  REGISTER_TEST(otbFilterFunctionValuesTest);
  REGISTER_TEST(otbSoilDataBaseParseFile);
}
