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

#ifndef __otbLambert2EtenduProjection_txx
#define __otbLambert2EtenduProjection_txx

#include "otbLambert2EtenduProjection.h"

namespace otb
{

  template <InverseOrForwardTransformationEnum transform>
      Lambert2EtenduProjection<transform>
  ::Lambert2EtenduProjection()
  {
    itk::Point<double,2> origin;
    origin[0]=2.33722778;
    origin[1]=46.8;
    std::string datum = "NTF";


    double parall1=45.89891944;
    double parall2=47.69601389;
    double falseEasting=600000;
    double falseNorthing=2200000;
    std::string ellipsoid = "CE";


    //TODO: 29-02-2008 Emmanuel: when ossim version > 1.7.2 only
    // SetOrigin required (remove SetEllipsoid)
    this->SetOrigin(origin, datum);
    this->SetEllipsoid(ellipsoid);
    this->SetParameters(parall1, parall2, falseEasting, falseNorthing);
  }

  template <InverseOrForwardTransformationEnum transform>
      Lambert2EtenduProjection<transform>
  ::~Lambert2EtenduProjection()
  {
  }


} // namespace otb

#endif
