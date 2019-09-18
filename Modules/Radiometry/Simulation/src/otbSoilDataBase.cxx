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


#include "otbSoilDataBase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "otb_boost_string_header.h"
#include "itkMacro.h"


namespace otb
{

SoilDataBase::SoilDataBase(const std::string& SoilFileName, double wlfactor) : m_SoilFileName(SoilFileName), m_WlFactor(wlfactor)
{
  ParseSoilFile();
};

const typename SoilDataBase::SoilDataVector& SoilDataBase::GetDB() const
{
  return m_SoilDataVector;
}

double SoilDataBase::GetReflectance(size_t SoilIndex, WavelenghtType wl) const
{
  if (SoilIndex >= m_SoilDataVector.size())
  {
    std::stringstream errmessg;
    errmessg << "Soil index must be >= 0 and < " << m_SoilDataVector.size() << ". " << SoilIndex << " was passed.\n";
    throw std::range_error(errmessg.str());
  }
  // wl not in the set of measured ones
  if (m_SoilDataVector[SoilIndex].find(wl) == m_SoilDataVector[SoilIndex].end())
  {
    const auto wlmin = m_Wavelengths[0];
    const auto wlmax = m_Wavelengths[m_Wavelengths.size() - 1];
    if (wl < wlmin)
      return (*m_SoilDataVector[SoilIndex].find(wlmin)).second;
    if (wl > wlmax)
      return (*m_SoilDataVector[SoilIndex].find(wlmax)).second;

    const auto p       = std::partition_point(m_Wavelengths.cbegin(), m_Wavelengths.cend(), [&](WavelenghtType w) { return w < wl; });
    const auto wlinf   = *(p - 1);
    const auto wlsup   = *p;
    const auto factinf = wl - wlinf;
    const auto factsup = wlsup - wl;
    const auto vinf    = (*m_SoilDataVector[SoilIndex].find(wlinf)).second;
    const auto vsup    = (*m_SoilDataVector[SoilIndex].find(wlsup)).second;
    return (vinf * factinf + vsup * factsup) / (factinf + factsup);
  }
  else
  {
    return (*m_SoilDataVector[SoilIndex].find(wl)).second;
  }
}

size_t SoilDataBase::CountColumns(std::string fileName) const
{
  std::ifstream ifile(fileName);
  std::string   line;
  if (ifile.is_open())
  {
    size_t nbSpaces = 0;
    if (!getline(ifile, line))
    {
      itkGenericExceptionMacro(<< "Error reading file " << fileName);
    }
    ifile.close();
    boost::trim(line);
    auto found = line.find(' ');
    while (found != std::string::npos)
    {
      ++nbSpaces;
      while (line[found + 1] == ' ')
        ++found;
      found = line.find(' ', found + 1);
    }
    return nbSpaces + 1;
  }
  else
  {
    itkGenericExceptionMacro(<< "Could not open file " << fileName);
  }
}
void SoilDataBase::ParseSoilFile()
{
  auto number_of_soils = CountColumns(m_SoilFileName) - 1;
  m_SoilDataVector.resize(number_of_soils);
  std::ifstream sdb(m_SoilFileName);
  if (!sdb.is_open())
  {
    itkGenericExceptionMacro(<< "Could not open file " << m_SoilFileName);
  }
  std::string line;
  while (std::getline(sdb, line))
  {
    if (line.size() > 3)
    {
      std::stringstream ss(line);
      double            tmpwl;
      ss >> tmpwl;
      WavelenghtType wl = static_cast<WavelenghtType>(m_WlFactor * tmpwl);
      m_Wavelengths.push_back(wl);
      for (size_t i = 0; i < number_of_soils; ++i)
      {
        double refl;
        ss >> refl;
        m_SoilDataVector[i][wl] = refl;
      }
    }
  }
  std::sort(m_Wavelengths.begin(), m_Wavelengths.end());
}
} // namespace otb
