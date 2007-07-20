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
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::~InverseSensorModel()
{
}


///Méthode InverseTransformPoint:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::TransformPoint(const InputPointType &point) const
{

	//On transforme le type "itk::point" en type "ossim::ossimGpt" 
  ossimGpt ossimGPoint(point[0], point[1]);
  
  //On calcule 
  ossimDpt ossimDPoint;

  if( this->m_Model == NULL)
  {
  	itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
  }
  
	this->m_Model->worldToLineSample(ossimGPoint, ossimDPoint);
  
  //On stocke le resultat dans un "OutputPointType"  
  OutputPointType outputPoint;
  outputPoint[0]=ossimDPoint.x;
  outputPoint[1]=ossimDPoint.y;
  
	return outputPoint;
}
  
///Méthode InverseTransformPoint en tenant compte de l'altitude:
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType 
InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::TransformPoint(const InputPointType &point, double height) const
{
	//On transforme le type "itk::point" en type "ossim::ossimGpt" 
  ossimGpt ossimGPoint(point[0], point[1], height);
  
  //On calcule 
  ossimDpt ossimDPoint;

  if( this->m_Model == NULL)
  {
  	itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
  }
  this->m_Model->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" appelle la méthode "lineSampleHeightToWorld" pour prendre en compte l'élévation. 
  
  //On stocke le resultat dans un "OutputPointType"  
  OutputPointType outputPoint;
  outputPoint[0]=ossimDPoint.x;
  outputPoint[1]=ossimDPoint.y;
  
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

