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
#ifndef __otbInverseSensorModel_txx
#define __otbInverseSensorModel_txx

#include "otbInverseSensorModel.h"
#include "itkExceptionObject.h"
#include "otbMacro.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::~InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
//     otbMsgDevMacro(<< "Geographic point lon/lat : (" << point[0] << "," <<  point[1] << ")");

  // Transformation of "itk::point" in "ossim::ossimGpt"
  ossimGpt ossimGPoint(point[1], point[0]);

  if (this->m_UseDEM)
    {
//       otbMsgDevMacro(<< "USING DEM ! ");
//       otbMsgDevMacro(<< "Point : (" << point[1] << "," << point[0] << ")");
    itk::Point<double, 2> currentPoint;
    currentPoint[0] = ossimGPoint.lon;
    currentPoint[1] = ossimGPoint.lat;
    double height = this->GetDEMFunction()->Evaluate(currentPoint); ;
//       otbMsgDevMacro(<< "height : " << height);
    ossimGPoint.height(height);
    }
  else if (InputPointType::PointDimension == 3)
    {
    ossimGPoint.height(point[2]);
    }
  else if (this->m_AverageElevation != -32768.0)
    {
    ossimGPoint.height(this->m_AverageElevation);
    }

  ossimDpt ossimDPoint;

  if (this->m_Model == NULL)
    {
    itkExceptionMacro(<< "TransformPoint(): Invalid Model pointer m_Model == NULL !");
    }

  this->m_Model->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" call "lineSampleHeightToWorld" method for take in care elevation information.

  OutputPointType outputPoint;

  outputPoint[0] = ossimDPoint.x;
  outputPoint[1] = ossimDPoint.y;
  if (OutputPointType::PointDimension == 3)
    {
    outputPoint[2] = ossimGPoint.height();
    }
//     otbMsgDevMacro(<< "Point in sensor geometry: (" << outputPoint[0] << "," <<  outputPoint[1] << ")");

  return outputPoint;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
