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

#ifndef otbRPC_h
#define otbRPC_h

#include "OTBCommonExport.h"

#include <array>
#include <limits>


namespace otb
{

/** \class RPC
 *
 * \brief Interface class for image keyword-list adapters.
 *
 * \ingroup OTBGdalAdapters
 */
struct OTBCommon_EXPORT RPC
{
  using Self = RPC;

  static constexpr std::size_t SIZE = 20;

  using CoefArray = std::array< double, SIZE >;
  using DoubleArray = double[ SIZE ];

  static constexpr
    std::size_t
    Size()
  {
    return sizeof( CoefArray::value_type ) * SIZE;
  }


  RPC( double lineOff,
       double sampOff,
       double latOff,
       double longOff,
       double heightOff,

       double lineScale,
       double sampScale,
       double latScale,
       double longScale,
       double heightScale,

       DoubleArray lineNum,
       DoubleArray lineDen,
       DoubleArray sampNum,
       DoubleArray sampDen,

       double minLong = std::numeric_limits< double >::min(),
       double minLat = std::numeric_limits< double >::min(),
       double maxLong = std::numeric_limits< double >::max(),
       double maxLat = std::numeric_limits< double >::max() );

  double m_LineOff;
  double m_SampOff;
  double m_LatOff;
  double m_LongOff;
  double m_HeightOff;
  double m_LineScale;
  double m_SampScale;
  double m_LatScale;
  double m_LongScale;
  double m_HeightScale;
  CoefArray m_LineNum;
  CoefArray m_LineDen;
  CoefArray m_SampNum;
  CoefArray m_SampDen;
  double m_MinLong;
  double m_MinLat;
  double m_MaxLong;
  double m_MaxLat;
};

} // end of namespace otb.

#endif // otbGDALRPC_h
