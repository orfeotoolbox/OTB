/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLogHelpers_h
#define otbLogHelpers_h

#include <ostream>

namespace otb
{

/** Helper class to log region in a more human readable way:
 * \tparam RegionType Applies to any type of regions, even unrelated
 * ones like `itk::ImageRegion` and `itk::ImageIORegion`.
 * e.g.
 * \code
 x ∈ [0..42[, y ∈ [12..24[, size=42x12 @(0, 12)
 * \endcode
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename RegionType>
struct NeatRegionLogger_CTADLess
{
  NeatRegionLogger_CTADLess(RegionType const& region) : m_region(region) {}

  friend std::ostream & operator<<(std::ostream & os, NeatRegionLogger_CTADLess const& r)
  {
    auto const& size = r.m_region.GetSize();
    auto const& idx  = r.m_region.GetIndex();
    auto const  idx_x1  = idx[0];
    int  const  idx_x2  = idx[0] + size[0];
    int  const  idx_y1  = idx[1];
    auto const  idx_y2  = idx[1] + size[1];
    os
      << "x ∈ ["<<idx_x1 << ".." << idx_x2 << '['
      << ", y ∈ ["<<idx_y1 << ".." << idx_y2 << '['
      << ", size=" << size[0]<<'x'<<size[1] << " @("<<idx_x1<<", "<<idx_y1<<')'
      ;
    return os ;
  }
  RegionType const& m_region;
};

template <typename RegionType>
auto NeatRegionLogger(RegionType const& region)
{ return NeatRegionLogger_CTADLess<RegionType>(region); }
} // otb namespace

#endif  // otbLogHelpers_h
