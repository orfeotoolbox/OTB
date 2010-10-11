/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
