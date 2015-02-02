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

#ifndef __otbLambertConformalConicMapProjection_txx
#define __otbLambertConformalConicMapProjection_txx

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
::SetParameters(double parallel1Degree, double parallel2Degree, double falseEasting, double falseNorthing)
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
