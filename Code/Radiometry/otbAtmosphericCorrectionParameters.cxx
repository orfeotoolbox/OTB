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
  /**
   * Constructor
   */
const double AtmosphericCorrectionParameters::m_StepOfWavelenghtSpectralBandValues = 0.0025;



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
    os << indent << "Matrix of WavelenghtSpectralBandStep [x][y]: [" << m_WavelenghtSpectralBand.Cols() <<";"<<m_WavelenghtSpectralBand.Rows()<<"]" << std::endl;
    os << indent << "Step between each wavelenght spectral band values: " << m_StepOfWavelenghtSpectralBandValues << std::endl;
  }
} // end namespace otb

