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

#ifndef __otbMollweidMapProjection_txx
#define __otbMollweidMapProjection_txx

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
