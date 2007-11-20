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
	m_Epsilon = 0.0001;
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
	otbGenericMsgDebugMacro(<< "Point in sensor geometry: (" << point[0] << "," <<	point[1] << ")");
 
	// "itk::point" to "ossim::ossimDpt" transformation
  ossimDpt ossimPoint(point[0], point[1]);
  
  // Calculation 
  ossimGpt ossimGPoint;

  if( this->m_Model == NULL)
  {
    itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
  }
	
  this->m_Model->lineSampleToWorld(ossimPoint, ossimGPoint); 
	
	if (this->m_UseDEM)
	{
		ossimGpt ossimGPointTmp ;
		ossimGpt ossimTest;
		ossimGpt ossimGPointRef = ossimGPoint;
		double height;
		itk::Point<double, 2> point;
		int nbIter = 0;
	
		otbMsgDevMacro(<< "USING DEM ! ") ;

		while ((( fabs(ossimGPointRef.lat - ossimGPointTmp.lat) > m_Epsilon)
			||( fabs(ossimGPointRef.lon - ossimGPointTmp.lon) > m_Epsilon))
			&& (nbIter < 3))
		{
			otbGenericMsgDebugMacro(<< "Iter " << nbIter);
			
			if (nbIter != 0)
				ossimGPointRef = ossimGPointTmp;
				
			otbGenericMsgDebugMacro(<< "Point Before iter : (" << ossimGPointRef.lat << "," << ossimGPointRef.lon <<")");	
			
			point[0] = ossimGPointRef.lon;
			point[1] = ossimGPointRef.lat;
			
			height = this->m_DEMHandler->GetHeightAboveMSL(point);
			otbGenericMsgDebugMacro(<< "height : " << height) ;
			
			this->m_Model->lineSampleHeightToWorld(ossimPoint, height, ossimGPointTmp);	
			otbGenericMsgDebugMacro(<< "Point After iter :    (" << ossimGPointTmp.lat << "," << ossimGPointTmp.lon << ")");

			nbIter++;
		}
		
		ossimGPoint = ossimGPointRef;
	}
  
  // "OutputPointType" storage.
  OutputPointType outputPoint;
  outputPoint[0]=ossimGPoint.lon;
  outputPoint[1]=ossimGPoint.lat;
	
	otbGenericMsgDebugMacro(<< "Geographic point lon/lat : (" << outputPoint[1] << "," <<	outputPoint[0] << ")");
  
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

