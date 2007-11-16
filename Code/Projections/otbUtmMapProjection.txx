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
#include "otbMapProjections.h"


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
		this->Modified();
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
		this->Modified();
  }
  
  ///Set the hemisphere
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetHemisphere(char hemisphere) 
  {
		this->m_MapProjection->setHemisphere(hemisphere);
		this->Modified();
  }
	
	template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZoneAndHemisphereFromGeoPoint(const InputPointType &geoPoint)
	{
		double latitude = geoPoint[1];
		char hemisphere;
		int zone ;
		
		if (latitude > 0.)
			hemisphere = 'N';
		else
			hemisphere = 'S';
		this->SetHemisphere(hemisphere);

		zone = this->GetZoneFromGeoPoint(geoPoint);
		this->SetZone(zone);
	}
	
	/*template <InverseOrForwardTransformationEnum transform>
  void UtmMapProjection<transform>
  ::SetZoneAndHemisphereFromCartoPoint(const OutputPointType &cartoPoint)
	{
		InputPointType geoPoint;
		
		// TODO : Tester que la projection est bien inverse !!!
		geoPoint = this->TransformPoint(cartoPoint);
		this->SetZoneAndHemisphereFromGeoPoint(geoPoint);
	}	*/
	
  
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
	

	template <InverseOrForwardTransformationEnum transform>
	int UtmMapProjection<transform>
	::GetZoneFromGeoPoint(const InputPointType& geoPoint)
	{
		double longitude = geoPoint[0];
		//double latitude = geoPoint[1];
		int zone;
		
		// Each UTM zone is a narrow zone of 6 degrees in width
		// Zone 31 is between 0 and 6 degrees (lon)
		// There is 60 zones in each hemisphere
		zone = ((static_cast<int>(floor(longitude/6))+30)%60+60)%60+1 ;

		return zone;
	}
	
/*	template <InverseOrForwardTransformationEnum transform>
	void UtmMapProjection<transform>
	::Initialize(const InputPointType& middlePoint)
	{
		this->SetZoneAndHemisphereFromCartoPoint(middlePoint);
	}*/
	
	

}
#endif
