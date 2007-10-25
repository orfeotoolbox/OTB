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
#ifndef __otbForwardSensorModel_txx
#define __otbForwardSensorModel_txx


#include "otbForwardSensorModel.h"
#include "itkExceptionObject.h"
#include "otbMacro.h"

namespace otb
{ 

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ForwardSensorModel< TScalarType,
                    NInputDimensions,
                    NOutputDimensions,
                    NParametersDimensions>
::ForwardSensorModel()
{
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ForwardSensorModel< TScalarType,
                    NInputDimensions,
                    NOutputDimensions,
                    NParametersDimensions>
::~ForwardSensorModel()
{
}


template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename ForwardSensorModel< TScalarType,
                             NInputDimensions,
                             NOutputDimensions,
                             NParametersDimensions>::OutputPointType 
ForwardSensorModel< TScalarType,
                    NInputDimensions,
                    NOutputDimensions,
                    NParametersDimensions>
::TransformPoint(const InputPointType &point) const
{
  // "itk::point" to "ossim::ossimDpt" transformation
  ossimDpt ossimPoint(point[0], point[1]);
  
  // Calculation 
  ossimGpt ossimGPoint;
  if( this->m_Model == NULL)
    {
      itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
    }
  
  // Projection using the method of the class ossimSensorModel : lineSampleToWorld()
  this->m_Model->lineSampleToWorld(ossimPoint, ossimGPoint); 
  
  // "OutputPointType" storage.
  OutputPointType outputPoint;
  outputPoint[0]=ossimGPoint.lat;
  outputPoint[1]=ossimGPoint.lon;
  
  return outputPoint;
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
ForwardSensorModel< TScalarType, 
                    NInputDimensions,
                    NOutputDimensions,
                    NParametersDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // namespace otb

#endif

