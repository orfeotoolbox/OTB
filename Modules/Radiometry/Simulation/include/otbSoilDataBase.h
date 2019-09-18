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


#ifndef otbSoilDataBase_h
#define otbSoilDataBase_h

#include "OTBSimulationExport.h"
#include "itkMacro.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace otb
{

class ITK_ABI_EXPORT SoilDataBase
{
public:
  // wavelength in nm
  using WavelenghtType = unsigned int;
  using SoilData       = std::unordered_map<WavelenghtType, double>;
  using SoilDataVector = std::vector<SoilData>;

  SoilDataBase(const std::string& SoilFileName, double wlfactor);
  const SoilDataVector& GetDB() const;
  double GetReflectance(size_t SoilIndex, WavelenghtType wl) const;

protected:
  size_t CountColumns(std::string fileName) const;
  void ParseSoilFile();

  std::string                 m_SoilFileName;
  double                      m_WlFactor;
  SoilDataVector              m_SoilDataVector;
  std::vector<WavelenghtType> m_Wavelengths;
};
}

#endif /* otbSoilDataBase_h */
