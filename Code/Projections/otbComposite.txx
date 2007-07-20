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
#ifndef __otbComposite_txx
#define __otbComposite_txx

#include "otbComposite.h"

namespace otb
{

template<class TInputMapProjection, class TOutputMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::Composite()
: Superclass(SpaceDimension,ParametersDimension)
{
}
  
template<class TInputMapProjection, class TOutputMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::~Composite()
{
}

///Méthode pour passer de la projection 1 à la projection 2
template<class TInputMapProjection, class TOutputMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
typename Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType 
Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::ComputeProjection1ToProjection2(const InputPointType &point1)
{
	InputPointType geopoint;
	OutputPointType point2;	
	
	//On projette le point en (lat,lon) 
	geopoint=m_InputMapProjection->Inverse(point1); 
	//Puis on le reprojette en coord carto.
	point2=m_OutputMapProjection->Forward(geopoint); 

	return point2;
}

///Méthode pour passer de la projection 2 à la projection 1
template<class TInputMapProjection, class TOutputMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
typename Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::InputPointType 
Composite<TInputMapProjection, TOutputMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::ComputeProjection2ToProjection1(const OutputPointType &point2)
{

	OutputPointType geopoint;
	InputPointType point1;

	//On projette le point en (lat,lon) 
	geopoint=m_OutputMapProjection->Inverse(point2);
	//Puis on le reprojette en coord carto.
	point1=m_InputMapProjection->Forward(geopoint);
	
	return point1;
}

} // namespace otb

#endif

