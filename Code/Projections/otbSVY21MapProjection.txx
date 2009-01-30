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

#ifndef __otbSVY21MapProjection_txx
#define __otbSVY21MapProjection_txx

#include "otbSVY21MapProjection.h"

namespace otb
{

template <InverseOrForwardTransformationEnum transform>
SVY21MapProjection<transform>
::SVY21MapProjection()
{
  itk::Point<double,2> origin;
  origin[0]=103.83333333333333;
  origin[1]=1.3666666666666667;
  std::string datum = "WE"; //WGS84 datum


  double falseEasting=28001.642;
  double falseNorthing=38744.572;
  double scaleFactor=1.00000;
  std::string ellipsoid = "WE"; //GRS-80 ellipsoid


  //TODO: 29-02-2008 Emmanuel: when ossim version > 1.7.2 only
  // SetOrigin required (remove SetEllipsoid)
  this->SetOrigin(origin, datum);
  this->SetEllipsoid(ellipsoid);
  this->SetParameters(falseEasting, falseNorthing, scaleFactor);
}

template <InverseOrForwardTransformationEnum transform>
SVY21MapProjection<transform>
::~SVY21MapProjection()
{
}


} // namespace otb

#endif
