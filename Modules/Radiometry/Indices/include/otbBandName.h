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

#ifndef otbBandName_h
#define otbBandName_h

namespace otb
{
namespace BandName
{

/**
 * Provides a way to identify bands when passing the parameters
 * to the radiometric functors.*
 */
enum class CommonBandNames
{
  BLUE,
  GREEN,
  RED,
  NIR,
  MIR,
  MAX
};
enum LandsatTMBandNames
{
  TM1,
  TM2,
  TM3,
  TM4,
  TM5,
  TM60,
  TM61,
  TM62,
  TM7
};

// Note for landsat equivalence
// http://landsat.gsfc.nasa.gov/news/news-archive/sci_0017.html
// TM1 = Coastal
// TM2 = Blue
// TM3 = Green
// TM4 = Red
// TM5 = NIR
// TM6 = SWIR1 = MIR (SPOT5)
// TM7 = SWIR2
// TM8 = Panchro
// TM9 = Cirrus
//
// Spot at
// http://www.spotimage.com/web/en/233-resolution-and-spectral-bands.php

// Band Number        Wavelength Interval        Spectral Response
// 1                0.45-0.52 microm                Blue-Green
// 2                0.52-0.60 microm                Green
// 3                0.63-0.69 microm                Red
// 4                0.76-0.90 microm                Near IR
// 5                1.55-1.75 microm                Mid-IR
// 6              10.40-12.50 microm                Thermal IR
// 7                2.08-2.35 microm                Mid-IR

} // namespace BandName

} // namespace otb

#endif
