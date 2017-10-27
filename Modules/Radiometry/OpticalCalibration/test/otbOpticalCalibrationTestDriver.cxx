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
  REGISTER_TEST(otbSpectralSensitivityReaderNew);
  REGISTER_TEST(otbSpectralSensitivityReaderTest);
  REGISTER_TEST(otbSpectralSensitivityReaderGenericTest);
  REGISTER_TEST(otbReflectanceToImageImageFilter);
  REGISTER_TEST(otbReflectanceToRadianceImageFilterNew);
  REGISTER_TEST(otbRadianceToReflectanceImageFilterNew);
  REGISTER_TEST(otbRadianceToReflectanceImageFilterAuto);
  REGISTER_TEST(otbImageToRadianceImageFilterNew);
  REGISTER_TEST(otbRadiometryCorrectionParametersToAtmosphericRadiativeTermsNew);
  REGISTER_TEST(otbRadianceToImageImageFilterAuto);
  REGISTER_TEST(otbRadianceToImageImageFilterNew);
  REGISTER_TEST(otbReflectanceToRadianceImageFilterAuto);
  REGISTER_TEST(otbAeronetExtractDataBadData);
  REGISTER_TEST(otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter);
  REGISTER_TEST(otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms);
  REGISTER_TEST(otbSurfaceAdjacencyEffectCorrectionSchemeFilterNew);
  REGISTER_TEST(otbImageToReflectanceImageFilterAuto);
  REGISTER_TEST(otbAtmosphericCorrectionParametersNew);
  REGISTER_TEST(otbImageToReflectanceImageFilterNew);
  REGISTER_TEST(otbFilterFunctionValuesNew);
  REGISTER_TEST(otbWavelengthSpectralBandsNew);
  REGISTER_TEST(otbAtmosphericRadiativeTermsNew);
  REGISTER_TEST(otbAtmosphericRadiativeTermsSingleChannelNew);
  REGISTER_TEST(otbAtmosphericRadiativeTermsTest);
  REGISTER_TEST(otbImageToReflectanceImageFilter);
  REGISTER_TEST(otbRadianceToReflectanceImageFilter);
  REGISTER_TEST(otbReflectanceToImageImageFilterAuto);
  REGISTER_TEST(otbAeronetNew);
  REGISTER_TEST(otbAeronetExtractData);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterNew);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterTest);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterTest2);
  REGISTER_TEST(otbImageMetadataCorrectionParametersNew);
  REGISTER_TEST(otbImageToRadianceImageFilterAuto);
  REGISTER_TEST(otbAtmosphericCorrectionSequencementTest);
  REGISTER_TEST(otbSIXSTraitsTest);
  REGISTER_TEST(otbSIXSTraitsComputeAtmosphericParametersTest);
  REGISTER_TEST(otbReflectanceToImageImageFilterNew);
  REGISTER_TEST(otbSurfaceAdjacencyEffectCorrectionSchemeFilter);
  REGISTER_TEST(otbRadianceToImageImageFilter);
  REGISTER_TEST(otbReflectanceToRadianceImageFilter);
  REGISTER_TEST(otbImageToRadianceImageFilter);
}
