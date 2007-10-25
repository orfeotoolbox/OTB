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

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::InverseSensorModel()
{
	m_DEMHandler = DEMHandlerType::New();
	m_UseDEM = false;
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::~InverseSensorModel()
{
}



template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::TransformPoint(const InputPointType &point) const
{
	otbGenericMsgDebugMacro(<< "Geographic point lon/lat : (" << point[0] << "," <<	point[1] << ")");
	
	// Transformation of "itk::point" in "ossim::ossimGpt" 
	ossimGpt ossimGPoint(point[1], point[0]);
	
	if (m_UseDEM)
	{
		otbMsgDebugMacro(<< "USING DEM ! ") ;
		otbMsgDebugMacro(<< "Point : (" << point[1] << "," << point[0] << ")");
		double height = m_DEMHandler->GetHeightAboveMSL(point);
		otbMsgDebugMacro(<< "height : " << height) ;
		ossimGPoint.height(height);
	}
  
  ossimDpt ossimDPoint;

  if( this->m_Model == NULL)
  {
  	itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
  }
  this->m_Model->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" appelle la méthode "lineSampleHeightToWorld" pour prendre en compte l'élévation. 
  
  OutputPointType outputPoint;
	
//	std::cout << "m_UpperLeftCorner[1] : " << m_UpperLeftCorner[1] << std::endl;
//	std::cout << "ossimDPoint.x : " << ossimDPoint.x << std::endl;
//	std::cout << "ossimDPoint.y : " << ossimDPoint.y << std::endl;
	
	
  outputPoint[0]=ossimDPoint.x;
  outputPoint[1]=ossimDPoint.y;

	otbGenericMsgDebugMacro(<< "Point in sensor geometry: (" << outputPoint[0] << "," <<	outputPoint[1] << ")");

	return outputPoint;
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // namespace otb

#endif

