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

#ifndef __otbEckert4MapProjection_txx
#define __otbEckert4MapProjection_txx

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
