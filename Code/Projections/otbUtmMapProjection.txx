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

#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
UtmMapProjection<TTransform>
::UtmMapProjection()
{
  this->SetWkt("ossimUtmProjection");
}


///Set the zone
template <TransformDirection::TransformationDirection TTransform>
void UtmMapProjection<TTransform>
::SetZone(long zone)
{
  this->SetParameter("Zone", Utils::ConvertToString(zone));
  this->Modified();
}

///Set the hemisphere
template <TransformDirection::TransformationDirection TTransform>
void UtmMapProjection<TTransform>
::SetHemisphere(char hemisphere)
{
  this->SetParameter("Hemisphere", Utils::ConvertToString(hemisphere));
  this->Modified();
}

template <TransformDirection::TransformationDirection TTransform>
void UtmMapProjection<TTransform>
::SetZoneAndHemisphereFromGeoPoint(const InputPointType& geoPoint)
{
  if (geoPoint[1] > 0.) this->SetParameter("Hemisphere", "N");
  else this->SetParameter("Hemisphere", "S");

  int zone = Utils::GetZoneFromGeoPoint(geoPoint[0], geoPoint[1]);
  this->SetParameter("Zone", Utils::ConvertToString(zone));
}

///\return the zone
template <TransformDirection::TransformationDirection TTransform>
int UtmMapProjection<TTransform>
::GetZone() const
{
  int zone = atoi(this->GetParameter("Zone").c_str());
  return zone;
}

///\return the hemisphere
template <TransformDirection::TransformationDirection TTransform>
char UtmMapProjection<TTransform>
::GetHemisphere() const
{
  //If this happens, we have to better control the instanciation of
  //the projection.
  assert(this->GetParameter("Hemisphere").size() > 0);
  char hemisphere = this->GetParameter("Hemisphere")[0];

  return hemisphere;
}

}
#endif
