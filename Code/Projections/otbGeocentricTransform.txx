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
#ifndef __otbGeocentricTransform_txx
#define __otbGeocentricTransform_txx

#include "otbGeocentricTransform.h"

namespace otb
{

  template<InverseOrForwardTransformationEnum TransformDirection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      GeocentricTransform<TransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
  ::GeocentricTransform() : Superclass(SpaceDimension,ParametersDimension)
  {
    m_Ellipsoid = new ossimEllipsoid();
  }

  template<InverseOrForwardTransformationEnum TransformDirection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      GeocentricTransform<TransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
  ::~GeocentricTransform()
  {
    if (m_Ellipsoid != NULL)
    {
      delete m_Ellipsoid;
    }
  }

  template<InverseOrForwardTransformationEnum TransformDirection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      typename GeocentricTransform<TransformDirection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
          GeocentricTransform<TransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformPoint(const InputPointType & point) const
  {
    OutputPointType outputPoint;

    //To be completed
    return outputPoint;
  }


} // namespace otb

#endif
