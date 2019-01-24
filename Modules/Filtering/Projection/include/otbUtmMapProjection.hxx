/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbUtmMapProjection_hxx
#define otbUtmMapProjection_hxx

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
  //If this happens, we have to better control the instantiation of
  //the projection.
  assert(this->GetParameter("Hemisphere").size() > 0);
  char hemisphere = this->GetParameter("Hemisphere")[0];

  return hemisphere;
}

}
#endif
