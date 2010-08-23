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

///Set the zone
template <InverseOrForwardTransformationEnum TTransform>
void UtmMapProjection<TTransform>
::SetZone(long zone)
{
  this->m_MapProjection->setZone(zone);
  this->Modified();
}

///Set the hemisphere
template <InverseOrForwardTransformationEnum TTransform>
void UtmMapProjection<TTransform>
::SetHemisphere(char hemisphere)
{
  this->m_MapProjection->setHemisphere(hemisphere);
  this->Modified();
}

template <InverseOrForwardTransformationEnum TTransform>
void UtmMapProjection<TTransform>
::SetZoneAndHemisphereFromGeoPoint(const InputPointType& geoPoint)
{
  char hemisphere;

  if (geoPoint[1] > 0.) hemisphere = 'N';
  else hemisphere = 'S';
  this->SetHemisphere(hemisphere);

  int zone = this->GetZoneFromGeoPoint(geoPoint);
  this->SetZone(zone);
}

///\return the zone
template <InverseOrForwardTransformationEnum TTransform>
int UtmMapProjection<TTransform>
::GetZone() const
{
  int zone = this->m_MapProjection->getZone();

  return zone;
}

///\return the hemisphere
template <InverseOrForwardTransformationEnum TTransform>
const char UtmMapProjection<TTransform>
::GetHemisphere() const
{
  char hemisphere = this->m_MapProjection->getHemisphere();

  return hemisphere;
}

template <InverseOrForwardTransformationEnum TTransform>
int UtmMapProjection<TTransform>
::GetZoneFromGeoPoint(const InputPointType& geoPoint) const
{
  //use ossim to handle the special case of UTM
  ossimGpt point(geoPoint[1],  geoPoint[0]);
  int zone = this->m_MapProjection->computeZone(point);

  return zone;
}

}
#endif
