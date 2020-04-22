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
  REGISTER_TEST(otbSensorModel);
  REGISTER_TEST(otbVectorDataProjectionFilterFromGeoToMap);
  REGISTER_TEST(otbGenericRSResampleImageFilter);
  REGISTER_TEST(otbGeometriesProjectionFilter);
  REGISTER_TEST(otbGenericRSTransformGenericTest);
  REGISTER_TEST(otbVectorDataTransformFilter);
  REGISTER_TEST(otbRationalTransformToDisplacementFieldSourceTest);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToSensor);
  REGISTER_TEST(otbRationalTransform);
  REGISTER_TEST(otbGeometriesProjectionFilterFromMapToSensor);
  REGISTER_TEST(otbImageToEnvelopeVectorDataFilter);
  REGISTER_TEST(otbImageToGenericRSOutputParameters);
  REGISTER_TEST(otbGeometriesProjectionFilterFromMapToGeo);
  REGISTER_TEST(otbROIdataConversion);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToGeo);
  REGISTER_TEST(otbPhysicalToRPCSensorModelImageFilter);
  REGISTER_TEST(otbGeometriesProjectionFilterFromGeoToMap);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToImage);
  REGISTER_TEST(otbGenericRSTransformFromImage);
  REGISTER_TEST(otbGenericRSTransformImageAndMNTToWGS84ConversionChecking);
  REGISTER_TEST(otbCompositeTransform);
  REGISTER_TEST(otbLeastSquareAffineTransformEstimator);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterCheckRpcModel);
  REGISTER_TEST(otbGeographicalDistance);
  REGISTER_TEST(otbTileImageFilterRSTransformTest);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterAndOrtho);
  REGISTER_TEST(otbVectorDataIntoImageProjectionFilterCompareImplTest);
  REGISTER_TEST(otbVectorDataExtractROIandProjection);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterWithoutDEM);
  REGISTER_TEST(otbGeometriesProjectionFilterFromMapToImage);
  REGISTER_TEST(otbGeometriesProjectionFilterFromMapToEPSG);
  REGISTER_TEST(otbVectorDataProjectionFilter);
  REGISTER_TEST(otbTileMapTransform);
  REGISTER_TEST(otbImportGeoInformationImageFilter);
}
