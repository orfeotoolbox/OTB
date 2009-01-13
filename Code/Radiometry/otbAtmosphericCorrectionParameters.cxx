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

#include "otbAtmosphericCorrectionParameters.h"

namespace otb
{
  /***********************      FilterFunctionValues **************************/
  FilterFunctionValues
  ::FilterFunctionValues()
  {
     m_MinSpectralValue = 0;
     m_MaxSpectralValue = 0;
     m_FilterFunctionValues.clear();
  }

  /**PrintSelf method */
  void
  FilterFunctionValues
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Minimum spectral value: " << m_MinSpectralValue << std::endl;
    os << indent << "Maximum spectral value: " << m_MaxSpectralValue << std::endl;
    os << indent << "User Step between each wavelenght spectral band values: " << m_UserStep << std::endl;
    os << indent << "Filter function Vector Values: " << std::endl;
    for (unsigned int i=0; i<m_FilterFunctionValues.size(); i++)
      {
  os << indent << m_FilterFunctionValues[i] <<std::endl;
      }
    os << indent << "Filter function Vector Values 6S: " << std::endl;
    for (unsigned int i=0; i<m_FilterFunctionValues6S.size(); i++)
      {
  os << indent << m_FilterFunctionValues6S[i] <<std::endl;
      }
  }


  /***********************      AtmosphericCorrectionParameters **************************/
  /**
   * Constructor
   */

  AtmosphericCorrectionParameters
  ::AtmosphericCorrectionParameters()
  {
    m_AerosolModel = CONTINENTAL;
  }

  /**PrintSelf method */
  void
  AtmosphericCorrectionParameters
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Solar zenithal angle: " << m_SolarZenithalAngle << std::endl;
    os << indent << "Solar azimutal angle: " << m_SolarAzimutalAngle << std::endl;
    os << indent << "Viewing zenithal angle: " << m_ViewingZenithalAngle << std::endl;
    os << indent << "Viewing azimutal angle: " << m_ViewingAzimutalAngle << std::endl;
    os << indent << "Month: " << m_Month << std::endl;
    os << indent << "Day: " << m_Day << std::endl;
    os << indent << "Atmospheric pressure: " << m_AtmosphericPressure << std::endl;
    os << indent << "Water vapor amount: " << m_WaterVaporAmount << std::endl;
    os << indent << "Ozone amount: " << m_OzoneAmount << std::endl;
    os << indent << "Aerosol model: " << m_AerosolModel << std::endl;
    os << indent << "Aerosol optical : " << m_AerosolOptical << std::endl;

    // Function values print :
    os << indent << "Filter function Values: " << std::endl;
    for (unsigned int i=0; i<m_WavelenghtSpectralBand.size(); i++)
      {
  os << indent << "Channel : "<< i+1 <<" : " << std::endl;
  os << indent << m_WavelenghtSpectralBand[i]<< std::endl;
      }
    }
} // end namespace otb

