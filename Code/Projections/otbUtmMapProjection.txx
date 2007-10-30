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
    m_utmprojection = new ossimUtmProjection();
  }
  
 	template <InverseOrForwardTransformationEnum transform>
  UtmMapProjection<transform>
  ::~UtmMapProjection()
  {
    delete m_utmprojection;
  }
  
  ///Set the zone
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZone(long zone)
  {
 //   delete m_utmprojection;
 //   m_utmprojection= new ossimUtmProjection(zone);
 		m_utmprojection->setZone(zone);
  }
  
  ///Set the zone
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZone(const InputPointType &ground) 
  {
    ossimGpt ossimGround;
    ossimGround.lon=ground[0];
    ossimGround.lat=ground[1];
    m_utmprojection->setZone(ossimGround);
  }
  
  ///Set the hemisphere
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetHemisphere(char hemisphere) 
  {
    m_utmprojection->setHemisphere(hemisphere);
  }
  
  ///\return the zone
  template <InverseOrForwardTransformationEnum transform>
	long UtmMapProjection<transform>
  ::GetZone()
  {
    long zone;
    zone=m_utmprojection->getZone();
    
    return zone;
  }
  
  ///\return the hemisphere
  template <InverseOrForwardTransformationEnum transform>
	const char UtmMapProjection<transform>
  ::GetHemisphere() const
  {
    char hemisphere=0;
    hemisphere=m_utmprojection->getHemisphere();
	
    return hemisphere;
  }
  
} // namespace otb

#endif
