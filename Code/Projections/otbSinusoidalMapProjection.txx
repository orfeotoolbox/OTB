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

#ifndef __otbSinusoidalMapProjection_txx
#define __otbSinusoidalMapProjection_txx

#include "otbSinusoidalMapProjection.h"
#include "otbUtils.h"

namespace otb
{

template <TransformDirection::TransformationDirection TTransform>
SinusoidalMapProjection<TTransform>
::SinusoidalMapProjection()
{
  this->SetWkt("ossimSinusoidalProjection");
}

template <TransformDirection::TransformationDirection TTransform>
SinusoidalMapProjection<TTransform>
::~SinusoidalMapProjection()
{
}

///Set the false Easting
template <TransformDirection::TransformationDirection TTransform>
void SinusoidalMapProjection<TTransform>
::SetFalseEasting(double falseEasting)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->Modified();
}

///Set the False Northing
template <TransformDirection::TransformationDirection TTransform>
void SinusoidalMapProjection<TTransform>
::SetFalseNorthing(double falseNorthing)
{
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

///\return the False Northing (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double SinusoidalMapProjection<TTransform>
::GetFalseNorthing() const
{
  double falseNorthing = atof(this->GetParameter("FalseNorthing").c_str());
  return falseNorthing;
}

///\return the False Easting (avoid negative coordinates)
template <TransformDirection::TransformationDirection TTransform>
double SinusoidalMapProjection<TTransform>
::GetFalseEasting() const
{
  double falseEasting = atof(this->GetParameter("FalseEasting").c_str());
  return falseEasting;
}

template <TransformDirection::TransformationDirection TTransform>
void SinusoidalMapProjection<TTransform>
::SetParameters(double falseEasting, double falseNorthing)
{
  this->SetParameter("FalseEasting", Utils::ConvertToString(falseEasting));
  this->SetParameter("FalseNorthing", Utils::ConvertToString(falseNorthing));
  this->Modified();
}

}
#endif
