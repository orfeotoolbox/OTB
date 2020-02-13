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

#include "otbImageMetadata.h"
#include <iostream>

int otbImageMetadataTest(int, char*[])
{
  using namespace otb;
  
  ImageMetadata md;
  md[MDStr::SensorID] = "PHR";
  md.ProjectionRef = "UTM projRef";
  md.GeoTransform[0] = 10.0;
  md.GeoTransform[3] = 20.0;
  md.GeoTransform[5] = -1.0;

  BandMetadata bmd;
  bmd.Name = "B3";
  bmd[MDNum::PhysicalGain] = 2.0;
  bmd[MDNum::PhysicalBias] = 1.0;
  md.Bands.push_back(bmd);

  bmd.Name = "B2";
  bmd[MDNum::PhysicalGain] = 3.0;
  bmd[MDNum::PhysicalBias] = 2.0;
  md.Bands.push_back(bmd);

  bmd.Name = "B1";
  bmd.NoDataFlag = true;
  bmd.NoDataValue = -10000.0;
  bmd[MDNum::PhysicalGain] = 4.0;
  bmd[MDNum::PhysicalBias] = 3.0;
  md.Bands.push_back(bmd);

  ImageMetadata md2;

  md2 = md;

  md.ProjectionRef = "Lambert projRef";
  md.GeoTransform[5] = 1.0;

  std::cout << "md2: "<< md2 << "\n";
  
  return 0;
}
