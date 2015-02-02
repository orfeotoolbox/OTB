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
#ifndef __otbTransMercatorMapProjection_txx
#define __otbTransMercatorMapProjection_txx

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
