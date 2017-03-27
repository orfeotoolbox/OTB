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


#ifndef otbSoilDataBase_h
#define otbSoilDataBase_h

#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "itkMacro.h"

namespace otb
{

class SoilDataBase
{
public:
  //wavelength in nm
  using WavelenghtType = unsigned int;
  using SoilData = std::unordered_map<WavelenghtType, double>;
  using SoilDataVector = std::vector<SoilData>;

  SoilDataBase(const std::string& SoilFileName, double wlfactor) : 
    m_SoilFileName(SoilFileName), m_WlFactor(wlfactor) 
  {
    ParseSoilFile();
  }; 

  const SoilDataVector& GetDB() const
  {
    return m_SoilDataVector;
  }

  double GetReflectance(size_t SoilIndex, WavelenghtType wl)
  {
    assert(SoilIndex<m_SoilDataVector.size());
    // wl not in the set of measured ones
    if(m_SoilDataVector[SoilIndex].find(wl)==m_SoilDataVector[SoilIndex].end())
      {
      const auto wlmin = m_Wavelengths[0];
      const auto wlmax = m_Wavelengths[m_Wavelengths.size()-1];
      if(wl<wlmin) return m_SoilDataVector[SoilIndex][wlmin];
      if(wl>wlmax) return m_SoilDataVector[SoilIndex][wlmax];

      const auto p = std::partition_point(m_Wavelengths.cbegin(), m_Wavelengths.cend(),
                                          [&](WavelenghtType w){ return w<wl;}
        );
      const auto wlinf = *(p-1);
      const auto wlsup = *p;
      const auto factinf = wl-wlinf;
      const auto factsup = wlsup-wl;
      return (m_SoilDataVector[SoilIndex][wlinf]*factinf
              +m_SoilDataVector[SoilIndex][wlsup]*factsup)/(factinf+factsup);
      }
    else
        {
        return m_SoilDataVector[SoilIndex][wl];
      }
  }

protected:
  size_t countColumns(std::string fileName)
  {
    std::ifstream ifile(fileName.c_str());
    std::string line;
    if (ifile.is_open())
      {
      size_t nbSpaces = 0;
      getline(ifile,line);
      ifile.close();
      boost::trim(line);
      auto found = line.find(' ');
      while(found!=std::string::npos)
        {
        ++nbSpaces;
        while(line[found+1] == ' ') ++found;
        found = line.find(' ', found+1);
        }
      return nbSpaces+1;
      }
    else
      {
      itkGenericExceptionMacro(<< "Could not open file " << fileName);
      }
  }
  void ParseSoilFile()
  {
    unsigned int number_of_soils = countColumns(m_SoilFileName) - 1;
    m_SoilDataVector.resize(number_of_soils);
    std::ifstream sdb(m_SoilFileName);
    std::string line;
    while(sdb.good())
      {
      std::getline(sdb, line);
      if(line.size() > 3)
        {
        std::stringstream ss(line);
        double tmpwl;
        ss >> tmpwl;
        WavelenghtType wl = static_cast<WavelenghtType>(m_WlFactor*tmpwl);
        for(size_t i=0; i< number_of_soils; ++i)
          {
          if(i==0)
            m_Wavelengths.push_back(wl);
          double refl;
          ss >> refl;
          m_SoilDataVector[i][wl] = refl;
          }
        }
      }
    std::sort(m_Wavelengths.begin(), m_Wavelengths.end());
  }

  std::string m_SoilFileName;
  double m_WlFactor;
  SoilDataVector m_SoilDataVector;
  std::vector<WavelenghtType> m_Wavelengths;
};
}

#endif /* otbSoilDataBase_h */
