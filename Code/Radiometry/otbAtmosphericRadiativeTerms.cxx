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

#include "otbAtmosphericRadiativeTerms.h"

namespace otb 
{
  /**PrintSelf method */
  void 
  AtmosphericRadiativeTerms
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Intrinsic Atmospheric Reflectance: " << m_IntrinsicAtmosphericReflectance << std::endl;
    os << indent << "Shperical Albedo of the Atmosphere: " << m_SphericalAlbedo << std::endl;
    os << indent << "Ozone Transmission: " << m_OzoneTransmission << std::endl;
    os << indent << "OxygenTransmission: " << m_OxygenTransmission << std::endl;
    os << indent << "Water vapor Transmission: " << m_WaterVaporTransmission << std::endl;
    os << indent << "Downward Transmittance of the Atmospher: " << m_DownwardTransmittance << std::endl;
    os << indent << "Upward Transmittance of the Atmospher: " << m_UpwardTransmittance << std::endl;
   }

} // end namespace otb

