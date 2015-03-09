/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGeographicalDistance_txx
#define __otbGeographicalDistance_txx

#include "otbGeographicalDistance.h"
#include "otbMath.h"

namespace otb
{

template <class TVector>
GeographicalDistance<TVector>
::GeographicalDistance() : m_EarthRadius(6371000)
{}


template <class TVector>
double
GeographicalDistance<TVector>
::Evaluate(const VectorType & x) const
{
  // First check if vector lenght is sufficient
  if(x.Size()<2)
    itkExceptionMacro(<<"Vector length must be at least 2 to compute geographical distance.");

  // Call evaluate implementation with the first point being the
  // origin
  VectorType origin(x);
  origin[0]=this->GetOrigin()[0];
  origin[1]=this->GetOrigin()[1];

  return this->Evaluate(origin, x);
}

template <class TVector>
double
GeographicalDistance<TVector>
::Evaluate(const VectorType & x, const VectorType & y) const
{
  // First check if vector lenght is sufficient
  if(x.Size()<2 || y.Size()<2)
    itkExceptionMacro(<<"Vector length must be at least 2 to compute geographical distance.");

  // Build some const variables
  const double One = itk::NumericTraits<double>::One;
  const double Two = One + One;
  const double Deg2Rad = CONST_PI/180.;

  // Compute lattitude and longitude differences
  double dLat = (vcl_fabs(x[1] - y[1])) * Deg2Rad;
  double dLon = (vcl_fabs(x[0] - y[0])) * Deg2Rad;

  // Compute dx in meters
  double a = vcl_sin(dLat / Two) * vcl_sin(dLat / Two) + vcl_cos(y[1] * Deg2Rad) * vcl_cos(
    x[1] * Deg2Rad) * vcl_sin(dLon / Two) * vcl_sin(dLon / Two);
  double c = Two * vcl_atan2(vcl_sqrt(a), vcl_sqrt(One - a));
  double d = m_EarthRadius * c;

  // Return result
  return d;
}

template <class TVector>
void
GeographicalDistance<TVector>
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Earth radius
  os<<indent<<"Earth radius: "<<m_EarthRadius<<std::endl;
}
} // End namespace otb

#endif
