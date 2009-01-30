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

#ifndef __otbLambert93Projection_txx
#define __otbLambert93Projection_txx

#include "otbLambert93Projection.h"

namespace otb
{

template <InverseOrForwardTransformationEnum transform>
Lambert93Projection<transform>
::Lambert93Projection()
{
  itk::Point<double,2> origin;
  origin[0]=3;
  origin[1]=46.5;
  std::string datum = "WE"; //WGS84 datum


  double parall1=44;
  double parall2=49;
  double falseEasting=700000;
  double falseNorthing=6600000;
  std::string ellipsoid = "RF"; //GRS-80 ellipsoid


  //TODO: 29-02-2008 Emmanuel: when ossim version > 1.7.2 only
  // SetOrigin required (remove SetEllipsoid)
  this->SetOrigin(origin, datum);
  this->SetEllipsoid(ellipsoid);
  this->SetParameters(parall1, parall2, falseEasting, falseNorthing);
}

template <InverseOrForwardTransformationEnum transform>
Lambert93Projection<transform>
::~Lambert93Projection()
{
}


} // namespace otb

#endif
