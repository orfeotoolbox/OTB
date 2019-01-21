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

#include "otbPolarimetricData.h"

namespace otb
{
/**
 * Constructor
 */
PolarimetricData
::PolarimetricData()
{
  SetArchitectureType(UNKNOWN_ARCHITECTURE);
}

void
PolarimetricData
::DetermineArchitecture(bool *IsPresent)
{

  // With all the channels
  if (IsPresent[0] && IsPresent[1] && IsPresent[2] && IsPresent[3])
    {
    SetArchitectureType(HH_HV_VH_VV);
    }
  else
  // With 3 channels : HH HV VV
  if (IsPresent[0] && IsPresent[1] && !IsPresent[2] && IsPresent[3])
    {
    SetArchitectureType(HH_HV_VV);
    }
  else
  // With 3 channels : HH VH VV
  if (IsPresent[0] && !IsPresent[1] && IsPresent[2] && IsPresent[3])
    {
    SetArchitectureType(HH_VH_VV);
    }
  else
  // Only HH and HV are present
  if (IsPresent[0] && IsPresent[1] && !IsPresent[2] && !IsPresent[3])
    {
    SetArchitectureType(HH_HV);
    }
  else
  // Only VH and VV are present
  if (!IsPresent[0] && !IsPresent[1] && IsPresent[2] && IsPresent[3])
    {
    SetArchitectureType(VH_VV);
    }
  else
  // Only HH and VV are present
  if (IsPresent[0] && !IsPresent[1] && !IsPresent[2] && IsPresent[3])
    {
    SetArchitectureType(HH_VV);
    }
  else
    {
    SetArchitectureType(UNKNOWN_ARCHITECTURE);
    }
}

void
PolarimetricData
::DetermineArchitecture(int NumberOfImages, bool EmissionH, bool EmissionV)
{

  switch (NumberOfImages)
    {
    case 4:
      SetArchitectureType(HH_HV_VH_VV);
      break;

    case 3:
      SetArchitectureType(HH_HV_VV);
      break;

    case 2:

      if (EmissionH && !EmissionV)
        {
        SetArchitectureType(HH_HV);
        }
      else if (!EmissionH && EmissionV)
        {
        SetArchitectureType(VH_VV);
        }
      break;

    default:
      itkExceptionMacro("Unknown architecture !");
      return;
    }
}

/**PrintSelf method */
void
PolarimetricData
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "ArchitectureType " << m_ArchitectureType << " : " << std::endl;
}

} // end namespace otb
