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


#ifndef otbLambertConformalConicMapProjection_hxx
#define otbLambertConformalConicMapProjection_hxx

#include "otbLambertConformalConicMapProjection.h"
#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
LambertConformalConicMapProjection<TTransform>
::LambertConformalConicMapProjection()
{
  this->SetWkt("ossimLambertConformalConicProjection");
}

/// Parallel1 specification
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetStandardParallel1(double degree)
{
  this->SetParameter("StandardParallel1", Utils::ConvertToString(degree));
  this->Modified();
}

/// Parallel2 specification
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetStandardParallel2(double degree)
{
  this->SetParameter("StandardParallel2", Utils::ConvertToString(degree));
  this->Modified();
}

/// Specification of the 2 parallels
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetStandardParallels(double parallel1Degree, double parallel2Degree)
{
  this->SetParameter("StandardParallel1", Utils::ConvertToString(parallel1Degree));
  this->SetParameter("StandardParallel2", Utils::ConvertToString(parallel2Degree));
  this->Modified();
}

/// False easting specification
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetFalseEasting(double falseEasting)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->Modified();
}

/// False Northing specification
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetFalseNorthing(double falseNorthing)
{
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

/// Set the parameters
template <TransformDirection::TransformationDirection TTransform>
void LambertConformalConicMapProjection<TTransform>
::SetAllParameters(double parallel1Degree, double parallel2Degree, double falseEasting, double falseNorthing)
{
  this->SetParameter("StandardParallel1", Utils::ConvertToString(parallel1Degree));
  this->SetParameter("StandardParallel2", Utils::ConvertToString(parallel2Degree));
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

/// Get the False Northing (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double LambertConformalConicMapProjection<TTransform>
::GetFalseNorthing() const
{
  double falseNorthing = atof(this->GetParameter("FalseNorthing").c_str());
  return falseNorthing;
}

///Get the False Easting (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double LambertConformalConicMapProjection<TTransform>
::GetFalseEasting() const
{
  double falseEasting = atof(this->GetParameter("FalseEasting").c_str());
  return falseEasting;
}

} // namespace otb

#endif
