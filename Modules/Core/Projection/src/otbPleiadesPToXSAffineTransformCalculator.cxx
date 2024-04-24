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

#include "otbPleiadesPToXSAffineTransformCalculator.h"
#include "otbPleiadesImageMetadataInterface.h"
#include "itkMetaDataObject.h"

namespace otb
{

bool PleiadesPToXSAffineTransformCalculator::CanCompute(const ImageMetadata & panImd, const ImageMetadata & xsImd)
{
  const auto hasRequiredMetadata = [](const ImageMetadata & imd)
                                    {return imd.Has(MDStr::SensorID)
                                         && imd.Has(MDStr::GeometricLevel)
                                         && imd[MDStr::SensorID].find("PHR") != std::string::npos //PHR 1A or PHR 1B
                                         && imd[MDStr::GeometricLevel] == "SENSOR"
                                         && MetaData::PleiadesUtils::HasSensorModelCharacteristics(imd);};

  if (hasRequiredMetadata(panImd) && hasRequiredMetadata(xsImd))
  {
    std::string pid  = panImd[MetaData::PleiadesUtils::IMAGE_ID_KEY];
    std::string xsid = xsImd[MetaData::PleiadesUtils::IMAGE_ID_KEY];
    pid  = pid.substr(0, pid.size() - 4);
    xsid = xsid.substr(0, xsid.size() - 4);

    return pid == xsid;
  }
  return false;
}


PleiadesPToXSAffineTransformCalculator::OffsetType PleiadesPToXSAffineTransformCalculator::ComputeOffset(const ImageMetadata & panImd,
                                                                                                         const ImageMetadata & xsImd)
{
  const auto panSensorCharacteristics = MetaData::PleiadesUtils::GetSensorModelCharacteristics(panImd);
  const auto xsSensorCharacteristics = MetaData::PleiadesUtils::GetSensorModelCharacteristics(xsImd);

  double timeDelta =  (xsSensorCharacteristics.timeRangeStart - panSensorCharacteristics.timeRangeStart).TotalSeconds();

  /**
  This code compute the shift between PAN and XS image from a bundle
  product in SENSOR geometry level using acquisition start time of
  XS and PAN images, as well as PAN sampling rate to get the shift
  in lines and FIRST_COL metadata to get shift in columns. Here are
  two examples of how this computation is done:

  Product 6543
                            XS                            PA
    START                   17:16:57.2374640              17:16:57.2373170
    Te                      0.294                         0.0735
    FIRST_COL               3577                          14304

    deltaMS-PA =  0.000147
    deltaMS-PA_nb_lin = deltaMS-PA / (TePA / 1000) = 2 lines
    deltacolMS-PA = (FIRST_COL _MS -1 )*4 + 1 - FIRST_COL_PA = 1 column

  Product 0445
                            XS                            PA
    START                   17:20:45.2371140              17:20:45.2369670
    Te                      0.294                         0.0735
    FIRST_COL               526                           2101

    deltaMS-PA =  0.000147
    deltaMS-PA_nb_lin = deltaMS-PA / (TePA / 1000) = 2 lines
    deltacolMS-PA = (FIRST_COL _MS -1 )*4 + 1 - FIRST_COL_PA = 0 column

  In order to get a transform from PAN physical space to XS physical
  space, we have to take into account two additional effects:
  - The shifts above must be multiplied by -1 to get the correct
    transform direction,
  - In SENSOR geometry level, sampling grids of XS and PAN product
    are phased, which means that a shift of 1.5 pixels must be added
    in both directions in order to get the upper-left corners of XS
    and PAN pixels to match.

  This leads to the following formula:
   */
  double lineShift_MS_P = -std::floor((timeDelta / (panSensorCharacteristics.linePeriod / 1000) + 0.5)) + 1.5;
  double colShift_MS_P  = -((xsSensorCharacteristics.swathFirstCol - 1) * 4 - panSensorCharacteristics.swathFirstCol + 1) + 1.5;

  OffsetType offset;

  offset[0] = colShift_MS_P;
  offset[1] = lineShift_MS_P;

  return offset;
}


PleiadesPToXSAffineTransformCalculator::TransformType::Pointer PleiadesPToXSAffineTransformCalculator::Compute(const ImageMetadata & panchromaticImd,
                                                                                                               const ImageMetadata & xsImd)
{
  // Compute the offset
  OffsetType offset = ComputeOffset(panchromaticImd, xsImd);

  // Apply the scaling
  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  // Apply the offset
  transform->Translate(offset);

  // Apply the scaling
  transform->Scale(0.25);

  return transform;
}

} // End namespace otb
