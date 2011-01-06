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

#include "otbCanopyParameters.h"


namespace otb
{


/**
 * Constructor
 */

CanopyParameters
::CanopyParameters()
{
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
CanopyParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os,indent);
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


