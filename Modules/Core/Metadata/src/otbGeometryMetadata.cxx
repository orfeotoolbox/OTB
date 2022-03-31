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

#include "otbGeometryMetadata.h"

#include <iostream>
#include "otbMacro.h"


namespace
{
constexpr int STRING_PRECISION = 20;

// the precision of std::to_string is limited to 6 digits
template <typename T>
std::string to_string_with_precision(const T value)
{
    std::ostringstream out;
    out.precision(STRING_PRECISION);
    out << std::fixed << value;
    return out.str();
}

template<class T>
void KeywordlistToVector(std::vector<T> & vector,
                          const otb::MetaData::Keywordlist & kwl,
                          const std::string & prefix)
{
  vector.clear();

  const auto size = std::stoi(kwl.at(prefix + ".number"));
  for (int i = 0; i < size; i++)
  {
    auto t = T::FromKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
    vector.push_back(t);
  }
}


template <class T>
void VectorToKeywordList(otb::MetaData::Keywordlist & kwl,
                         const std::vector<T> & input,
                         const std::string & prefix)
{
  int i = 0;
  for (const auto & elem: input)
  {
    elem.ToKeywordlist(kwl, prefix + "_" + to_string_with_precision(i) + ".");
    i++;
  }
  kwl.insert({prefix + ".number" , to_string_with_precision(i)});
}

}


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

void GCP::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  kwl.insert({prefix + "Id", m_Id});
  kwl.insert({prefix + "Info", m_Info});
  kwl.insert({prefix + "Row", to_string_with_precision(m_GCPRow)});
  kwl.insert({prefix + "Col", to_string_with_precision(m_GCPCol)});
  kwl.insert({prefix + "X", to_string_with_precision(m_GCPX)});
  kwl.insert({prefix + "Y", to_string_with_precision(m_GCPY)});
  kwl.insert({prefix + "Z", to_string_with_precision(m_GCPZ)});
}

const std::string & Get(const std::unordered_map<std::string, std::string> & kwl, const std::string & key)
{
  try
  {
    return kwl.at(key);
  }
  catch (const std::exception & e)
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
             << "Unable to find " << key << "in the input keywordlist");
  }
}

GCP GCP::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  //Info is optional in GCPs, the key might not be in the keywordlist
  std::string info;
  auto infoFound = kwl.find(prefix + "Info");
  if (infoFound != kwl.end())
  {
    info = infoFound->second;
  }

  return GCP(Get(kwl, prefix + "Id"),
             info,
             std::stod(kwl.at(prefix + "Col")),
             std::stod(kwl.at(prefix + "Row")),
             std::stod(kwl.at(prefix + "X")),
             std::stod(kwl.at(prefix + "Y")),
             std::stod(kwl.at(prefix + "Z")));
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

void GCPParam::ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const
{
  kwl.insert({prefix + "GCPProjection", GCPProjection});
  VectorToKeywordList(kwl, GCPs, prefix + "GCP");
}

void GCPParam::FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix)
{
  //Handle the empty projection case
  std::string info;
  auto projectionFound = kwl.find(prefix + "GCPProjection");
  if (projectionFound != kwl.end())
  {
    GCPProjection = projectionFound->second;
  }
  KeywordlistToVector(GCPs, kwl, prefix + "GCP");
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
