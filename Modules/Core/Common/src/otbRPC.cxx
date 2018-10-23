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

#include "otbRPC.h"


#include <cassert>
#include <cstring>


namespace otb
{


RPC
::RPC( double lineOff,
       double sampOff,
       double flatOff,
       double longOff,
       double heightOff,

       double lineScale,
       double sampScale,
       double flatScale,
       double longScale,
       double heightScale,

       DoubleArray lineNum,
       DoubleArray lineDen,
       DoubleArray sampNum,
       DoubleArray sampDen,

       double minLong,
       double minLat,
       double maxLong,
       double maxLat ) :
  m_LineOff( lineOff ),
  m_SampOff( sampOff ),
  m_LatOff( flatOff ),
  m_LongOff( longOff ),
  m_HeightOff( heightOff ),
  m_LineScale( lineScale ),
  m_SampScale( sampScale ),
  m_LatScale( flatScale ),
  m_LongScale( longScale ),
  m_HeightScale( heightScale ),
  m_LineNum(),
  m_LineDen(),
  m_SampNum(),
  m_SampDen(),
  m_MinLong( minLong ),
  m_MinLat( minLat ),
  m_MaxLong( maxLong ),
  m_MaxLat( maxLat )
{
  assert( Size()==m_LineNum.size() );
  assert( Size()==m_LineDen.size() );
  assert( Size()==m_SampNum.size() );
  assert( Size()==m_SampDen.size() );

  std::memcpy( m_LineNum.data(), lineNum, Size() );
  std::memcpy( m_LineDen.data(), lineDen, Size() );
  std::memcpy( m_SampNum.data(), sampNum, Size() );
  std::memcpy( m_SampDen.data(), sampDen, Size() );
}

} // End of namespace otb.
