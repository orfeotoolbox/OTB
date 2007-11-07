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

#ifndef __otbUtmMapProjection_txx
#define __otbUtmMapProjection_txx

#include "otbUtmMapProjection.h"


namespace otb
{
 	template <InverseOrForwardTransformationEnum transform>
	UtmMapProjection<transform>
  ::UtmMapProjection()
  {
  }
  
 	template <InverseOrForwardTransformationEnum transform>
  UtmMapProjection<transform>
  ::~UtmMapProjection()
  {
  }
  
  ///Set the zone
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZone(long zone)
  {
		this->m_MapProjection->setZone(zone);
  }
  
  ///Set the zone
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZone(const InputPointType &ground) 
  {
    ossimGpt ossimGround;
    ossimGround.lon=ground[0];
    ossimGround.lat=ground[1];
    this->m_MapProjection->setZone(ossimGround);
  }
  
  ///Set the hemisphere
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetHemisphere(char hemisphere) 
  {
		this->m_MapProjection->setHemisphere(hemisphere);
  }
  
  ///\return the zone
  template <InverseOrForwardTransformationEnum transform>
	long UtmMapProjection<transform>
  ::GetZone()
  {
    long zone;
    zone=this->m_MapProjection->getZone();
    
    return zone;
  }
  
  ///\return the hemisphere
  template <InverseOrForwardTransformationEnum transform>
	const char UtmMapProjection<transform>
  ::GetHemisphere() const
  {
    char hemisphere=0;
    hemisphere=this->m_MapProjection->getHemisphere();
	
    return hemisphere;
  }
	

/*	template <InverseOrForwardTransformationEnum transform>
	void UtmMapProjection<transform>*/
	
	
  

}
#endif
