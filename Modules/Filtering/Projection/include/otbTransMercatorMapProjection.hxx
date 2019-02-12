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

#ifndef otbTransMercatorMapProjection_hxx
#define otbTransMercatorMapProjection_hxx

#include "otbTransMercatorMapProjection.h"
#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
TransMercatorMapProjection<TTransform>
::TransMercatorMapProjection()
{
  this->SetWkt("ossimTransMercatorProjection");
  this->SetParameter("OriginX", "49.83"); //TODO check where this is coming from
  this->SetParameter("OriginY", "6.16");
// FIXME
//  ossimEllipsoid ellipsoid = *(ossimEllipsoidFactory::instance()->create("WE"));
//  ossimGpt origin(49.83, 6.16); //TODO check where this is coming from
//  this->m_MapProjection->setEllipsoid(ellipsoid);
//  this->m_MapProjection->setOrigin(origin);
}

/// Desctructor
template <TransformDirection::TransformationDirection TTransform>
TransMercatorMapProjection<TTransform>
::~TransMercatorMapProjection()
{
}

///Set the false Easting
template <TransformDirection::TransformationDirection TTransform>
void TransMercatorMapProjection<TTransform>
::SetFalseEasting(double falseEasting)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->Modified();
}

///Set the False Northing
template <TransformDirection::TransformationDirection TTransform>
void TransMercatorMapProjection<TTransform>
::SetFalseNorthing(double falseNorthing)
{
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

///Set the scale factor
template <TransformDirection::TransformationDirection TTransform>
void TransMercatorMapProjection<TTransform>
::SetScaleFactor(double scaleFactor)
{
  this->SetParameter("ScaleFactor", Utils::ConvertToString(scaleFactor));
  this->Modified();
}

///Set the parameters
template <TransformDirection::TransformationDirection TTransform>
void TransMercatorMapProjection<TTransform>
::SetParameters(double falseEasting, double falseNorthing, double scaleFactor)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->SetParameter("ScaleFactor", Utils::ConvertToString(scaleFactor));
  this->Modified();
}

///\return the scale factor
template <TransformDirection::TransformationDirection TTransform>
double TransMercatorMapProjection<TTransform>
::GetScaleFactor() const
{
  double scaleFactor = atof(this->GetParameter("ScaleFactor").c_str());
  return scaleFactor;
}

///\return the false northing (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double TransMercatorMapProjection<TTransform>
::GetFalseNorthing() const
{
  double falseNorthing = atof(this->GetParameter("FalseNorthing").c_str());
  return falseNorthing;
}

///\return the false easting (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double TransMercatorMapProjection<TTransform>
::GetFalseEasting() const
{
  double falseEasting = atof(this->GetParameter("FalseEasting").c_str());
  return falseEasting;
}

}
#endif
