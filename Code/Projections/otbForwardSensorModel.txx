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

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::ForwardSensorModel()
{
  m_Epsilon = 0.0001;
  m_NbIter = 1;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::~ForwardSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
//   otbMsgDevMacro(<< "Point in sensor geometry: (" << point[0] << "," <<  point[1] << ")");

  // "itk::point" to "ossim::ossimDpt" transformation
  ossimDpt ossimPoint(point[0], point[1]);

  // Calculation
  ossimGpt ossimGPoint;

  if (this->m_Model == NULL)
    {
    itkExceptionMacro(<< "TransformPoint(): Invalid Model pointer m_Model == NULL !");
    }

  //Use of DEM: need iteration to reach the correct point
  if (this->m_UseDEM)
    {
    this->m_Model->lineSampleToWorld(ossimPoint, ossimGPoint);
    ossimGpt ossimGPointRef = ossimGPoint;
    double height(0.), heightTmp(0.);
    double                diffHeight = 100; // arbitrary value
    itk::Point<double, 2> currentPoint;
    int                   nbIter = 0;

    otbMsgDevMacro(<< "USING DEM ! ");

    while ((diffHeight > m_Epsilon)  && (nbIter < m_NbIter))
      {
      otbMsgDevMacro(<< "Iter " << nbIter);

      if (nbIter != 0) height = heightTmp;

//       otbMsgDevMacro(<< "PointG Before iter : (" << ossimGPointRef.lat << "," << ossimGPointRef.lon <<")");

      currentPoint[0] = ossimGPointRef.lon;
      currentPoint[1] = ossimGPointRef.lat;

//      otbMsgDevMacro(<< "PointP Before iter : (" << point[1] << "," << point[0] <<")");

      heightTmp = this->m_DEMHandler->GetHeightAboveMSL(currentPoint);

//       otbMsgDevMacro(<< "height : " << heightTmp);

      this->m_Model->lineSampleHeightToWorld(ossimPoint, heightTmp, ossimGPointRef);
//       otbMsgDevMacro(<< "Point After iter :    (" << ossimGPointRef.lat << "," << ossimGPointRef.lon << ")");

      diffHeight = fabs(heightTmp - height);

      ++nbIter;
      }
    ossimGPoint = ossimGPointRef;
    }
  //Altitude of the point is provided (in the sensor coordinate)
  else if (InputPointType::PointDimension == 3)
    {
    this->m_Model->lineSampleHeightToWorld(ossimPoint, point[2], ossimGPoint);
    }
  //Use of average elevation
  else if (this->m_AverageElevation != -32768.0)
    {
    this->m_Model->lineSampleHeightToWorld(ossimPoint, this->m_AverageElevation, ossimGPoint);
    }
  //Otherwise, just don't consider the altitude
  else
    {
    this->m_Model->lineSampleToWorld(ossimPoint, ossimGPoint);
    }

  // "OutputPointType" storage.
  OutputPointType outputPoint;
  outputPoint[0] = ossimGPoint.lon;
  outputPoint[1] = ossimGPoint.lat;

//   otbMsgDevMacro(<< "Geographic point lon/lat : (" << outputPoint[1] << "," <<  outputPoint[0] << ")");

  return outputPoint;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
