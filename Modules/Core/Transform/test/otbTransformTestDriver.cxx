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
  REGISTER_TEST(otbGenericRSTransformWithSRID);
  REGISTER_TEST(otbCreateInverseForwardSensorModel);
  REGISTER_TEST(otbGenericRSTransform);
  REGISTER_TEST(otbCreateProjectionWithOSSIM);
  REGISTER_TEST(otbLogPolarTransformResample);
  REGISTER_TEST(otbLogPolarTransform);
  REGISTER_TEST(otbGeocentricTransform);
  REGISTER_TEST(otbCreateProjectionWithOTB);
  REGISTER_TEST(otbGenericMapProjection);
  REGISTER_TEST(otbStreamingWarpImageFilter);
  REGISTER_TEST(otbInverseLogPolarTransform);
  REGISTER_TEST(otbInverseLogPolarTransformResample);
  REGISTER_TEST(otbStreamingResampleImageFilterWithAffineTransform);
}
