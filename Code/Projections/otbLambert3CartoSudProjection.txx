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

#ifndef __otbLambert3CartoSudProjection_txx
#define __otbLambert3CartoSudProjection_txx

#include "otbLambert3CartoSudProjection.h"

namespace otb
{

template <InverseOrForwardTransformationEnum transform>
Lambert3CartoSudProjection<transform>
::Lambert3CartoSudProjection()
{
  itk::Point<double,2> origin;
  origin[0]=3;
  origin[1]=46.5;
  std::string datum = "WE"; //WGS84 datum


  double parall1=43.1992913888888888888888888889;
  double parall2=44.9960938888888888888888888888;
  double falseEasting=600000;
  double falseNorthing=3200000;
  std::string ellipsoid = "CE";

  //TODO: 29-02-2008 Emmanuel: when ossim version > 1.7.2 only
  // SetOrigin required (remove SetEllipsoid)
  this->SetOrigin(origin, datum);
  this->SetEllipsoid(ellipsoid);
  this->SetParameters(parall1, parall2, falseEasting, falseNorthing);
}

template <InverseOrForwardTransformationEnum transform>
Lambert3CartoSudProjection<transform>
::~Lambert3CartoSudProjection()
{
}


} // namespace otb

#endif
