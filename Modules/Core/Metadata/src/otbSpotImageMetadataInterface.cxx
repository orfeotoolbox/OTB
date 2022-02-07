/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSpotImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "otbMacro.h"
#include "itkMetaDataObject.h"

#include "otbXMLMetadataSupplier.h"

#include "otbDimapMetadataHelper.h"

namespace otb
{

unsigned int SpotImageMetadataInterface::BandIndexToWavelengthPosition(unsigned int i) const
{
  otbMsgDevMacro(<< "SPOT detected: band 0 and 2 inverted");
  if (i == 0)
    return 2;
  if (i == 2)
    return 0;
  return i;
}

void SpotImageMetadataInterface::FetchSpectralSensitivity(ImageMetadata& imd)
{
  // XS1 (Green), XS2 (Red), XS3 (near IR), SWIR 
  std::unordered_map<std::string, std::vector<double>> BandNameToSpectralSensitivityTable = {
    {"XS1", {
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0022771144f, 0.0045542287f, 0.021714223f,  0.038874217f,  0.088104725f,  0.13733523f,  0.21823871f,  0.29914218f,  0.38027802f,
        0.46141386f,  0.52832052f,   0.59522718f,   0.64936955f,   0.70351192f,   0.74529693f,   0.78708195f,  0.82499008f,  0.86289822f,  0.891466f,
        0.92003378f,  0.94254762f,   0.96506146f,   0.9776788f,    0.99029614f,   0.99514807f,   1.0f,         0.99612511f,  0.99225022f,  0.9804036f,
        0.96855699f,  0.94715939f,   0.9257618f,    0.90154813f,   0.87733446f,   0.84871261f,   0.82009075f,  0.78645969f,  0.75282863f,  0.71875799f,
        0.68468734f,  0.64818039f,   0.61167343f,   0.57338572f,   0.53509801f,   0.49156491f,   0.44803182f,  0.39732491f,  0.346618f,    0.29724838f,
        0.24787875f,  0.20190886f,   0.15593897f,   0.12403284f,   0.09212672f,   0.070920769f,  0.049714818f, 0.03798244f,  0.026250061f, 0.019816831f,
        0.013383602f, 0.010152718f,  0.006921834f,  0.0054235249f, 0.0039252158f, 0.0033455489f, 0.002765882f, 0.001382941f, 0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f,         0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,         0.0f,         0.0f,
        0.0f}},
    {"XS2", {
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0007673711f,
        0.0015347422f, 0.010419638f,  0.019304534f,  0.061542786f,  0.10378104f,   0.20079599f,   0.29781095f,   0.41207816f,   0.52634536f,  0.62039262f,
        0.71443988f,   0.78093055f,   0.84742121f,   0.88691213f,   0.92640305f,   0.95094047f,   0.9754779f,    0.98546823f,   0.99545857f,  0.99772928f,
        1.0f,          0.99374775f,   0.98749549f,   0.97497667f,   0.96245784f,   0.94318133f,   0.92390482f,   0.89924371f,   0.8745826f,   0.84464903f,
        0.81471546f,   0.76863444f,   0.72255341f,   0.65866954f,   0.59478567f,   0.5169305f,    0.43907532f,   0.36620824f,   0.29334116f,  0.23579319f,
        0.17824522f,   0.14140026f,   0.1045553f,    0.082135956f,  0.059716612f,  0.047238187f,  0.034759762f,  0.027242624f,  0.019725486f, 0.015547759f,
        0.011370033f,  0.0090187963f, 0.0066675595f, 0.0050977127f, 0.0035278658f, 0.0028316045f, 0.0021353432f, 0.0010676716f, 0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,         0.0f,
        0.0f}},
    {"XS3", {
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.00046485267f, 0.00092970533f, 0.0022703511f, 0.0036109969f, 0.0072833599f, 0.010955723f, 0.020381246f, 
      0.029806769f, 0.050425041f, 0.071043314f, 0.10698612f, 0.14292892f, 0.19323862f, 0.24354832f, 0.30664343f, 
      0.36973854f, 0.43590247f, 0.5020664f, 0.56601537f, 0.62996434f, 0.68195348f, 0.73394262f, 0.77728387f, 
      0.82062512f, 0.85237025f, 0.88411539f, 0.91130117f, 0.93848695f, 0.95316338f, 0.96783982f, 0.9764626f, 
      0.98508538f, 0.99215522f, 0.99922506f, 0.99961253f, 1.0f, 0.99705423f, 0.99410845f, 0.98330518f, 0.97250191f, 
      0.95825152f, 0.94400112f, 0.93075194f, 0.91750276f, 0.90228886f, 0.88707495f, 0.87152458f, 0.8559742f,
      0.82966368f, 0.80335316f, 0.7684215f, 0.73348985f, 0.68197206f, 0.63045427f, 0.56244955f, 0.49444484f, 
      0.43235415f, 0.37026347f, 0.30941829f, 0.24857311f, 0.20286212f, 0.15715112f, 0.13022816f, 0.1033052f, 
      0.084892217f, 0.06647923f, 0.053553967f, 0.040628703f, 0.03389025f, 0.027151797f, 0.022906605f, 
      0.018661412f, 0.015551925f, 0.012442438f, 0.010396472f, 0.0083505073f, 0.0070520644f, 0.0057536215f, 
      0.0049600816f, 0.0041665416f, 0.0035720051f, 0.0029774686f, 0.0029639347f, 0.0029504007f, 0.0029368668f, 
      0.0029233328f, 0.0029097989f, 0.0028962649f, 0.002882731f, 0.0028691971f, 0.0028556631f, 0.0028421292f, 
      0.0028285952f, 0.0028150613f, 0.0028015273f, 0.0027879934f, 0.0027744594f, 0.0027609255f, 0.0027473915f, 
      0.0027338576f, 0.0027203236f, 0.0027067897f, 0.0026932557f, 0.0026797218f, 0.0026661878f, 0.0026526539f, 
      0.0026391199f, 0.002625586f, 0.002612052f, 0.0025985181f, 0.0025849841f, 0.0025714502f, 0.0025579162f, 
      0.0025443823f, 0.0025308483f, 0.0025173144f, 0.0025037804f, 0.0024902465f, 0.0024767125f, 0.0024631786f, 
      0.0024496447f, 0.0024361107f, 0.0024225768f, 0.0024090428f, 0.0023955089f, 0.0023819749f, 0.002368441f, 
      0.002354907f, 0.0023413731f, 0.0023278391f, 0.0023143052f, 0.0023007712f, 0.0022872373f, 0.0022737033f, 
      0.0022601694f, 0.0022466354f, 0.0022331015f, 0.0022195675f, 0.0022060336f, 0.0021924996f, 0.0021789657f, 
      0.0021654317f, 0.0021518978f, 0.0021383638f, 0.0021248299f, 0.0021112959f, 0.002097762f, 0.002084228f, 
      0.0020706941f, 0.0020571601f, 0.0020436262f, 0.0020300923f, 0.0020165583f, 0.0020030244f, 0.0019894904f, 
      0.0019759565f, 0.0019624225f, 0.0019488886f, 0.0019353546f, 0.0019218207f, 0.0019082867f, 0.0018947528f, 
      0.0018812188f, 0.0018676849f, 0.0018541509f, 0.001840617f, 0.001827083f, 0.0018135491f, 0.0018000151f, 
      0.0017864812f, 0.0017729472f, 0.0017594133f, 0.0017458793f, 0.0017323454f, 0.0017188114f, 0.0017052775f, 
      0.0016917435f, 0.0016782096f, 0.0016646756f, 0.0016511417f, 0.0016376078f, 0.0016240738f, 0.0016105399f, 
      0.0015970059f, 0.001583472f, 0.001569938f, 0.0015564041f, 0.0015428701f, 0.0015293362f, 0.0015158022f, 
      0.0015022683f, 0.0014887343f, 0.0014752004f, 0.0014616664f, 0.0014481325f, 0.0014345985f, 0.0014210646f, 
      0.0014075306f, 0.0013939967f, 0.0013804627f, 0.0013669288f, 0.0013533948f, 0.0013398609f, 0.0013263269f, 
      0.001312793f, 0.001299259f, 0.0012857251f, 0.0012721911f, 0.0012586572f, 0.0012451232f, 0.0012315893f, 
      0.0012180554f, 0.0012045214f, 0.0011909875f, 0.0011774535f, 0.0011639196f, 0.0011503856f, 0.0011368517f, 
      0.0011233177f, 0.0011097838f, 0.0010962498f, 0.0010827159f, 0.0010691819f, 0.001055648f, 0.001042114f, 
      0.0010285801f, 0.0010150461f, 0.0010015122f, 0.00098797823f, 0.00097444428f, 0.00096091033f, 0.00094737638f, 
      0.00093384244f, 0.00092030849f, 0.00090677454f, 0.00089324059f, 0.00087970664f, 0.00086617269f, 0.00085263875f, 
      0.0008391048f, 0.00082557085f, 0.0008120369f, 0.00079850295f, 0.000784969f, 0.00077143506f, 0.00075790111f, 
      0.00074436716f, 0.00073083321f, 0.00071729926f, 0.00070376531f, 0.00069023137f, 0.00067669742f, 0.00066316347f, 
      0.00064962952f, 0.00063609557f, 0.00062256162f, 0.00060902768f, 0.00059549373f, 0.00058195978f, 0.00056842583f, 
      0.00055489188f, 0.00054135793f, 0.00052782399f, 0.00051429004f, 0.00050075609f, 0.00048722214f, 0.00047368819f, 
      0.00046015424f, 0.0004466203f, 0.00043308635f, 0.0004195524f, 0.00040601845f, 0.0003924845f, 0.00037895055f, 
      0.00036541661f, 0.00035188266f, 0.00033834871f, 0.00032481476f, 0.00031128081f, 0.00029774686f, 0.00028421292f, 
      0.00027067897f, 0.00025714502f, 0.00024361107f, 0.00023007712f, 0.00021654317f, 0.00020300923f, 0.00018947528f, 
      0.00017594133f, 0.00016240738f, 0.00014887343f, 0.00013533948f, 0.00012180554f, 0.00010827159f, 9.4737638e-05f, 
      8.120369e-05f, 6.7669742e-05f, 5.4135793e-05f, 4.0601845e-05f, 2.7067897e-05f, 1.3533948e-05f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
      0.0f, 0.0f, 0.0f, 0.0f}},
    {"SWIR", {
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,
        0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0088105211f, 0.017621042f,  0.020528086f,
        0.02343513f,   0.026392526f,  0.029349921f,  0.033664215f,  0.037978508f,  0.042245031f,  0.046511554f,  0.055648902f,  0.064786249f,  0.07395196f,
        0.08311767f,   0.10386151f,   0.12460535f,   0.14594447f,   0.16728359f,   0.20442288f,   0.24156217f,   0.27841272f,   0.31526328f,   0.35919335f,
        0.40312341f,   0.44621342f,   0.48930344f,   0.53207715f,   0.57485086f,   0.61905294f,   0.66325502f,   0.7114532f,    0.75965137f,   0.8076253f,
        0.85559924f,   0.89376042f,   0.93192161f,   0.9659608f,    1.0f,          0.99696894f,   0.99393787f,   0.99224825f,   0.99055862f,   0.96192825f,
        0.93329788f,   0.90629723f,   0.87929659f,   0.87156435f,   0.8638321f,    0.85303543f,   0.84223875f,   0.85502674f,   0.86781473f,   0.88056734f,
        0.89331995f,   0.89301593f,   0.89271191f,   0.86628263f,   0.83985335f,   0.80572933f,   0.77160531f,   0.73099612f,   0.69038694f,   0.64866608f,
        0.60694523f,   0.56782227f,   0.52869932f,   0.48719362f,   0.44568791f,   0.40599882f,   0.36630972f,   0.32807542f,   0.28984112f,   0.25834284f,
        0.22684456f,   0.19656397f,   0.16628339f,   0.14643708f,   0.12659078f,   0.106444f,     0.086297227f,  0.076081433f,  0.065865639f,  0.055683249f,
        0.045500859f,  0.040629869f,  0.03575888f,   0.031127116f,  0.026495353f,  0.024316707f,  0.022138061f,  0.019934653f,  0.017731245f,  0.01619062f,
        0.014649995f,  0.013104617f,  0.011559239f,  0.01030835f,   0.0090574603f, 0.0078583703f, 0.0066592804f, 0.0059099939f, 0.0051607074f, 0.0044159179f,
        0.0036711284f, 0.0033605972f, 0.0030500659f, 0.0027368724f, 0.0024236788f, 0.0021708901f, 0.0019181013f, 0.0016732152f, 0.001428329f,  0.001428329f,
        0.001428329f}}
  };

  otb::MetaData::LUT1D spectralSensitivity;

  spectralSensitivity.Axis[0].Origin = 0.45;
  spectralSensitivity.Axis[0].Spacing = 0.0025;
  spectralSensitivity.Axis[0].Size = 541;


  for (auto & band: imd.Bands)
  {
    auto SpectralSensitivityIt = BandNameToSpectralSensitivityTable.find(band[MDStr::BandName] );
    if (SpectralSensitivityIt != BandNameToSpectralSensitivityTable.end())
    {
      spectralSensitivity.Array = SpectralSensitivityIt->second;

      band.Add(MDL1D::SpectralSensitivity, spectralSensitivity);
    }
  }
}

void SpotImageMetadataInterface::Parse(ImageMetadata & imd)
{
  DimapMetadataHelper helper;

  // GDAL DIMAP metadata case
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "IMAGERY/SATELLITEID") == "SPOT 5")
  {
    //Find the METADATA.DIM file (Dimap V1 metadata file)
    auto resourceFiles = m_MetadataSupplierInterface->GetResourceFiles();
    
    std::string metadataFile;
    for (const auto & file: resourceFiles)
    {
      if (file.find("METADATA.DIM")!=std::string::npos)
      {
        metadataFile = file;
        break;
      }
    }

    if (metadataFile.empty())
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Cannot find the METADATA.DIM file")
    }

    XMLMetadataSupplier xmlMds(metadataFile);

    helper.ParseDimapV1(xmlMds, "Dimap_Document.");

  }
  // geom metadata case
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "support_data.sensorID" ) == "Spot 5")
  {
    helper.ParseGeom(*m_MetadataSupplierInterface);
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Not a Spot 5 product")
  }

  const auto & dimapData = helper.GetDimapData();
  
  imd.Add(MDStr::SensorID, "SPOT 5");
  imd.Add(MDStr::Mission, "SPOT 5");

  imd.Add(MDStr::Instrument, dimapData.Instrument);
  imd.Add(MDStr::InstrumentIndex, dimapData.InstrumentIndex);

  imd.Add(MDTime::ProductionDate,
    MetaData::ReadFormattedDate(dimapData.ProductionDate));
  imd.Add(MDTime::AcquisitionDate,
    MetaData::ReadFormattedDate(dimapData.AcquisitionDate));

  imd.Add(MDNum::SunAzimuth, dimapData.SunAzimuth[0]);
  imd.Add(MDNum::SunElevation, dimapData.SunElevation[0]);
  imd.Add(MDNum::SatElevation, 90. - dimapData.IncidenceAngle[0]);
  
  if (dimapData.StepCount < 48)
  {
    imd.Add(MDNum::SatAzimuth, dimapData.SceneOrientation[0] + 90);
  }
  else
  {
    imd.Add(MDNum::SatAzimuth, dimapData.SceneOrientation[0] - 90);
  }

  auto setDimapBandMetadata = [&imd](const MDNum & key, const std::vector<double> & input)
  {
    if (input.size() == 1)
    {
      // this is a PAN image
      imd.Bands[0].Add(key, input[0]);
    }
    else if (input.size() == 4)
    {
      // MS image, note the band ordering
      imd.Bands[0].Add(key, input[2]);
      imd.Bands[1].Add(key, input[1]);
      imd.Bands[2].Add(key, input[0]);
      imd.Bands[3].Add(key, input[3]);
    }
    else if (input.size() == 3)
    {
      imd.Bands[0].Add(key, input[2]);
      imd.Bands[1].Add(key, input[1]);
      imd.Bands[2].Add(key, input[0]);
    }
    else
    {
      otbGenericExceptionMacro(MissingMetadataException,
      << "The number of bands in image metadatas is incoherent with the DIMAP product")
    }
  };

  setDimapBandMetadata(MDNum::PhysicalBias, dimapData.PhysicalBias);
  setDimapBandMetadata(MDNum::PhysicalGain, dimapData.PhysicalGain);
  setDimapBandMetadata(MDNum::SolarIrradiance, dimapData.SolarIrradiance);

  auto nbBands = imd.Bands.size();

  // band IDs is not parsed in geom files. There are two possible cases: panchromatic and multispectral
  if (dimapData.BandIDs.empty())
  {
    if (imd.Bands.size() == 1)
    {
      imd.Bands[0].Add(MDStr::BandName, "P");
    }
    else if (imd.Bands.size() == 4)
    {
      imd.Bands[0].Add(MDStr::BandName, "XS1");
      imd.Bands[1].Add(MDStr::BandName, "XS2");
      imd.Bands[2].Add(MDStr::BandName, "XS3");
      imd.Bands[3].Add(MDStr::BandName, "SWIR");
    }
  }
  else if (dimapData.BandIDs.size() == nbBands )
  {
    auto bandId = dimapData.BandIDs.begin();
    
    for (auto & band: imd.Bands)
    {
      band.Add(MDStr::BandName, *bandId);
      bandId++;
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Invalid band number in the SPOT 5 product")
  }

  if (nbBands == 4)
  {
    FetchSpectralSensitivity(imd);
  }

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 1);
  imd.Add(MDNum::BlueDisplayChannel, 2);
}

} // end namespace otb
