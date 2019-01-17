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


#ifndef otbEckert4MapProjection_hxx
#define otbEckert4MapProjection_hxx

#include "otbEckert4MapProjection.h"
#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
Eckert4MapProjection<TTransform>
::Eckert4MapProjection()
{
  this->SetWkt("ossimEckert4Projection");
}

template <TransformDirection::TransformationDirection TTransform>
Eckert4MapProjection<TTransform>
::~Eckert4MapProjection()
{
}

///False easting specification
template <TransformDirection::TransformationDirection TTransform>
void Eckert4MapProjection<TTransform>
::SetFalseEasting(double falseEasting)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->Modified();
}

///False Northing specification
template <TransformDirection::TransformationDirection TTransform>
void Eckert4MapProjection<TTransform>
::SetFalseNorthing(double falseNorthing)
{
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

///Get the false northing coordinates (avoid negative values)
template <TransformDirection::TransformationDirection TTransform>
double Eckert4MapProjection<TTransform>
::GetFalseNorthing() const
{
  double falseNorthing = atof(this->GetParameter("FalseNorthing").c_str());
  return falseNorthing;
}

///Get the false easting coordinates (avoid negative values)
template <TransformDirection::TransformationDirection TTransform>
double Eckert4MapProjection<TTransform>
::GetFalseEasting() const
{
  double falseEasting = atof(this->GetParameter("FalseEasting").c_str());
  return falseEasting;
}

template <TransformDirection::TransformationDirection TTransform>
void Eckert4MapProjection<TTransform>
::SetParameters(double falseEasting, double falseNorthing)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

} // namespace otb

#endif
