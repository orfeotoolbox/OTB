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

namespace otb
{

template <Transform::TransformationDirection transform>
MollweidMapProjection<transform>
::MollweidMapProjection()
{
}

template <Transform::TransformationDirection transform>
MollweidMapProjection<transform>
::~MollweidMapProjection()
{
}

///Set the false easting
template <Transform::TransformationDirection transform>
void MollweidMapProjection<transform>
::SetFalseEasting(double falseEasting)
{
  this->m_MapProjection->setFalseEasting(falseEasting);
}

///Set the false Northing
template <Transform::TransformationDirection transform>
void MollweidMapProjection<transform>
::SetFalseNorthing(double falseNorthing)
{
  this->m_MapProjection->setFalseNorthing(falseNorthing);
}

///Set the default parameters
template <Transform::TransformationDirection transform>
void MollweidMapProjection<transform>
::SetDefaults()
{
  this->m_MapProjection->setDefaults();
}

///\return the false northing (avoid negative coordinates)
template <Transform::TransformationDirection transform>
double MollweidMapProjection<transform>
::GetFalseNorthing() const
{
  double falseNorthing = this->m_MapProjection->getFalseNorthing();

  return falseNorthing;
}

///\return the false easting (avoid negative coordinates)
template <Transform::TransformationDirection transform>
double MollweidMapProjection<transform>
::GetFalseEasting() const
{
  double falseEasting = this->m_MapProjection->getFalseEasting();

  return falseEasting;
}

template <Transform::TransformationDirection transform>
void MollweidMapProjection<transform>
::SetParameters(double falseEasting, double falseNorthing)
{
  this->m_MapProjection->setFalseEastingNorthing(falseEasting, falseNorthing);
}

} // end namespace otb

#endif
