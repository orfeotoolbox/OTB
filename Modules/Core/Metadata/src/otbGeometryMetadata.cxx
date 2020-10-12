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

#include "otbGeometryMetadata.h"

#include <iostream>

namespace otb
{

GCP::GCP(std::string id, std::string info, double col, double row, double px, double py, double pz)
  : m_Id(move(id)), m_Info(move(info)), m_GCPCol(col), m_GCPRow(row), m_GCPX(px), m_GCPY(py), m_GCPZ(pz)
{
}

void GCP::Print(std::ostream& os) const
{
  os << "   GCP Id = " << this->m_Id << std::endl;
  os << "   GCP Info =  " << this->m_Info << std::endl;
  os << "   GCP (Row, Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
  os << "   GCP (X, Y, Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
}

std::string GCP::ToJSON(bool multiline) const
{
  std::ostringstream oss;
  std::string sep;
  if (multiline)
  {
    sep = "\n";
  }
  oss << "{"
      << "\"GCP_Id\": \"" << this->m_Id << "\", " << sep
      << "\"GCP_Info\": \"" << this->m_Info << "\", " << sep
      << "\"GCP_Row\": \"" << this->m_GCPRow << "\", " << sep
      << "\"GCP_Col\": \"" << this->m_GCPCol << "\", " << sep
      << "\"GCP_X\": \"" << this->m_GCPX << "\", " << sep
      << "\"GCP_Y\": \"" << this->m_GCPY << "\", " << sep
      << "\"GCP_Z\": \"" << this->m_GCPZ << "\", " << sep
      << "}";
  return oss.str();
}

namespace Projection
{
std::string GCPParam::ToJSON(bool multiline) const
{
  std::ostringstream oss;
  std::string sep;
  if (multiline)
  {
    sep = "\n";
  }
  oss << "{"
      << "\"Projection\": \"" << GCPProjection << "\", " << sep
      << "[";
  for (const auto& gcp : GCPs)
    oss << gcp.ToJSON() << ", " << sep;
  oss << "]}";
  return oss.str();
}

std::string RPCParam::ToJSON(bool multiline) const
{
  std::ostringstream oss;
  std::string sep;
  if (multiline)
  {
    sep = "\n";
  }
  oss << "{"
      << "\"LineOffset\": \""   << LineOffset   << "\", " << sep
	  << "\"SampleOffset\": \"" << SampleOffset << "\", " << sep
	  << "\"LatOffset\": \""    << LatOffset    << "\", " << sep
	  << "\"LonOffset\": \""    << LonOffset    << "\", " << sep
	  << "\"HeightOffset\": \"" << HeightOffset << "\", " << sep

	  << "\"LineScale\": \""   << LineScale   << "\", " << sep
	  << "\"SampleScale\": \"" << SampleScale << "\", " << sep
	  << "\"LatScale\": \""    << LatScale    << "\", " << sep
	  << "\"LonScale\": \""    << LonScale    << "\", " << sep
	  << "\"HeightScale\": \"" << HeightScale << "\", " << sep

	  << "\"LineNum\": "   << doubleArrayToString(LineNum)   << ", " << sep
	  << "\"LineDen\": "   << doubleArrayToString(LineDen)   << ", " << sep
	  << "\"SampleNum\": " << doubleArrayToString(SampleNum) << ", " << sep
	  << "\"SampleDen\": " << doubleArrayToString(SampleDen) << ", " << sep

      << "}";
  return oss.str();
}

bool operator==(const RPCParam & lhs, const RPCParam & rhs)
{
  return lhs.LineOffset == rhs.LineOffset
      && lhs.SampleOffset == rhs.SampleOffset
      && lhs.LatOffset == rhs.LatOffset
      && lhs.LonOffset == rhs.LonOffset
      && lhs.HeightOffset == rhs.HeightOffset
      && lhs.LineScale == rhs.LineScale
      && lhs.SampleScale == rhs.SampleScale
      && lhs.LatScale == rhs.LatScale
      && lhs.LonScale == rhs.LonScale
      && lhs.HeightScale == rhs.HeightScale
      && std::equal(lhs.LineNum, lhs.LineNum+20, rhs.LineNum )
      && std::equal(lhs.LineDen, lhs.LineDen+20, rhs.LineDen )
      && std::equal(lhs.SampleNum, lhs.SampleNum+20, rhs.SampleNum )
      && std::equal(lhs.SampleDen, lhs.SampleDen+20, rhs.SampleDen );
}

} // end namespace Projection
} // end namespace otb
