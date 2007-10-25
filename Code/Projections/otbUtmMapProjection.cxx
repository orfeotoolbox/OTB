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
#include "otbUtmMapProjection.h"

namespace otb
{ 

UtmMapProjection
::UtmMapProjection()
{
	m_utmprojection = new ossimUtmProjection();
}

UtmMapProjection
::~UtmMapProjection()
{
	delete m_utmprojection;
}

///Set the zone
void UtmMapProjection
::SetZone(long zone)
{
	delete m_utmprojection;
	m_utmprojection= new ossimUtmProjection(zone);
}

///Set the zone
void UtmMapProjection
::SetZone(const InputPointType &ground) 
{
	ossimGpt ossimGround;
	ossimGround.lon=ground[0];
	ossimGround.lat=ground[1];
	m_utmprojection->setZone(ossimGround);
}

///Set the hemisphere
void UtmMapProjection
::SetHemisphere(char hemisphere) 
{
	m_utmprojection->setHemisphere(hemisphere);
}

///\return the zone
long UtmMapProjection
::GetZone()
{
	long zone;
	zone=m_utmprojection->getZone();
	
	return zone;
}

///\return the hemisphere
const char UtmMapProjection
::GetHemisphere() const
{
	char hemisphere=0;
	hemisphere=m_utmprojection->getHemisphere();
	
	return hemisphere;
}

} // namespace otb

