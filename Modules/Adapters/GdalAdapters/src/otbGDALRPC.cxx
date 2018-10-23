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

#include "otbGDALRPC.h"


#include <cstring>


namespace otb
{


namespace gdal
{


void
convert( GDALRPCInfo & to,
	 RPC const & from )
{
  to.dfLINE_OFF = from.m_LineOff;
  to.dfSAMP_OFF = from.m_SampOff;
  to.dfLAT_OFF = from.m_LatOff;
  to.dfLONG_OFF = from.m_LongOff;
  to.dfHEIGHT_OFF = from.m_HeightOff;
  to.dfLINE_SCALE = from.m_LineScale;
  to.dfSAMP_SCALE = from.m_SampScale;
  to.dfLAT_SCALE = from.m_LatScale;
  to.dfLONG_SCALE = from.m_LongScale;
  to.dfHEIGHT_SCALE = from.m_HeightScale;

  std::memcpy( to.adfLINE_NUM_COEFF, from.m_LineNum.data(), RPC::Size() );
  std::memcpy( to.adfLINE_DEN_COEFF, from.m_LineDen.data(), RPC::Size() );
  std::memcpy( to.adfSAMP_NUM_COEFF, from.m_SampNum.data(), RPC::Size() );
  std::memcpy( to.adfSAMP_DEN_COEFF, from.m_SampDen.data(), RPC::Size() );

  to.dfMIN_LONG = from.m_MinLong;
  to.dfMIN_LAT = from.m_MinLat;
  to.dfMAX_LONG = from.m_MaxLong;
  to.dfMAX_LAT = from.m_MaxLat;
}


} // End of namespace gdal.

} // End namespace otb
