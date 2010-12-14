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

#include "otbProSailParameters.h"


namespace otb
{


/**
 * Constructor
 */

ProSailParameters
::ProSailParameters()
{
   m_Cab=30.0;
   m_Car=10.0; 
   m_CBrown=0.0; 
   m_Cw=0.015; 
   m_Cm=0.009;
   m_N=1.2; 
   m_LAI=2;
   m_Angl=50; 
   m_PSoil=1;
   m_Skyl=70; 
   m_HSpot=0.2;
   m_TTS=30;
   m_TTO=0;
   m_PSI=0;
}



/**PrintSelf method */
void
ProSailParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   os << "Chlorophyll content       : " << m_Cab << std::endl;
   os << "Brown pigment content     : " << m_Car << std::endl;
   os << "EWT                       : " << m_Cw << std::endl;
   os << "LMA                       : " << m_Cm << std::endl;
   os << "structure coef            : " << m_N << std::endl;
   os << "leaf area index           : " << m_LAI << std::endl;
   os << "average leaf angle        : " << m_Angl << std::endl;
   os << "soil coefficient          : " << m_PSoil << std::endl;
   os << "diffuse/direct radiation  : " << m_Skyl << std::endl;
   os << "hot spot                  : " << m_HSpot << std::endl;
   os << "solar zenith angle        : " << m_TTS << std::endl;
   os << "observer zenith angle     : " << m_TTO << std::endl;
   os << "azimuth                   : " << m_PSI << std::endl;
}


} // end namespace otb
