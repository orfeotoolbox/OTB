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


#ifndef otbMollweidMapProjection_hxx
#define otbMollweidMapProjection_hxx

#include "otbMollweidMapProjection.h"
#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
MollweidMapProjection<TTransform>
::MollweidMapProjection()
{
  this->SetWkt("ossimMollweidProjection");
}

template <TransformDirection::TransformationDirection TTransform>
MollweidMapProjection<TTransform>
::~MollweidMapProjection()
{
}

///Set the false easting
template <TransformDirection::TransformationDirection TTransform>
void MollweidMapProjection<TTransform>
::SetFalseEasting(double falseEasting)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->Modified();
}

///Set the false Northing
template <TransformDirection::TransformationDirection TTransform>
void MollweidMapProjection<TTransform>
::SetFalseNorthing(double falseNorthing)
{
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

///\return the false northing (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double MollweidMapProjection<TTransform>
::GetFalseNorthing() const
{
  double falseNorthing = atof(this->GetParameter("FalseNorthing").c_str());
  return falseNorthing;
}

///\return the false easting (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double MollweidMapProjection<TTransform>
::GetFalseEasting() const
{
  double falseEasting = atof(this->GetParameter("FalseEasting").c_str());
  return falseEasting;
}

template <TransformDirection::TransformationDirection TTransform>
void MollweidMapProjection<TTransform>
::SetParameters(double falseEasting, double falseNorthing)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

} // end namespace otb

#endif
