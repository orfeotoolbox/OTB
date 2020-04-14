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

OTB_GCP::OTB_GCP() : m_GCPCol(0), m_GCPRow(0), m_GCPX(0), m_GCPY(0), m_GCPZ(0)
{
}

OTB_GCP::~OTB_GCP()
{
}

void OTB_GCP::Print(std::ostream& os) const
{
  os << "   GCP Id = " << this->m_Id << std::endl;
  os << "   GCP Info =  " << this->m_Info << std::endl;
  os << "   GCP (Row, Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
  os << "   GCP (X, Y, Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
}

std::string OTB_GCP::ToJSON(bool multiline) const
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
    oss << gcp.ToJSON() << ", ";
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

} // end namespace Projection
} // end namespace otb
