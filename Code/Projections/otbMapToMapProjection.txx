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
#ifndef __otbMapToMapProjection_txx
#define __otbMapToMapProjection_txx

#include "otbMapToMapProjection.h"

namespace otb
{
  
template<class TInputMapProjection, 
         class TOutputMapProjection, 
         class TScalarType, 
         unsigned int NInputDimensions, 
         unsigned int NOutputDimensions>
MapToMapProjection<TInputMapProjection, 
                   TOutputMapProjection, 
                   TScalarType, 
                   NInputDimensions, 
                   NOutputDimensions>
::MapToMapProjection()
  : Superclass(SpaceDimension,ParametersDimension)
{
}
  
template<class TInputMapProjection, 
         class TOutputMapProjection, 
         class TScalarType, 
         unsigned int NInputDimensions, 
         unsigned int NOutputDimensions>
MapToMapProjection<TInputMapProjection, 
                   TOutputMapProjection, 
                   TScalarType, 
                   NInputDimensions, 
                   NOutputDimensions>
::~MapToMapProjection()
{
}
  
  ///Pass from projection 1 to projection 2
/*  template<class TInputMapProjection, 
	   class TOutputMapProjection, 
	   class TScalarType, 
	   unsigned int NInputDimensions, 
	   unsigned int NOutputDimensions> 
  typename MapToMapProjection<TInputMapProjection, 
			      TOutputMapProjection, 
			      TScalarType, 
			      NInputDimensions, 
			      NOutputDimensions>::OutputPointType 
  MapToMapProjection<TInputMapProjection, 
		     TOutputMapProjection, 
		     TScalarType, 
		     NInputDimensions, 
		     NOutputDimensions>::ComputeProjection1ToProjection2(const InputPointType &point1)
  {
    InputPointType geopoint;
    OutputPointType point2;	
    
    //(lat,lon) projection 
    geopoint=m_InputMapProjection->Inverse(point1); 
    //map projection
    point2=m_OutputMapProjection->Forward(geopoint); 
    
    return point2;
  }

  ///Pass from projection 2 to projection 1
  template<class TInputMapProjection, 
	   class TOutputMapProjection, 
	   class TScalarType, 
	   unsigned int NInputDimensions, 
	   unsigned int NOutputDimensions> 
  typename MapToMapProjection<TInputMapProjection, 
			      TOutputMapProjection, 
			      TScalarType, 
			      NInputDimensions, 
			      NOutputDimensions>::InputPointType 
  MapToMapProjection<TInputMapProjection, 
		     TOutputMapProjection, 
		     TScalarType, 
		     NInputDimensions, 
		     NOutputDimensions>::ComputeProjection2ToProjection1(const OutputPointType &point2)
  {
    
    OutputPointType geopoint;
    InputPointType point1;
    
    //(lat,lon) projection
    geopoint=m_OutputMapProjection->Inverse(point2);
    //map projection
    point1=m_InputMapProjection->Forward(geopoint);
    
    return point1;
  }*/

template<class TInputMapProjection, 
         class TOutputMapProjection, 
         class TScalarType, 
         unsigned int NInputDimensions, 
         unsigned int NOutputDimensions>	
typename MapToMapProjection<TInputMapProjection, 
                   TOutputMapProjection, 
                   TScalarType, 
                   NInputDimensions, 
                   NOutputDimensions>
::OutputPointType 
MapToMapProjection<TInputMapProjection, 
                   TOutputMapProjection, 
                   TScalarType, 
                   NInputDimensions, 
                   NOutputDimensions>::
TransformPoint(const InputPointType &point) const
{
    OutputPointOfInputMapProjectionType geopoint;
    OutputPointType point2;	
    
    //(lat,lon) projection 
    geopoint=m_InputMapProjection->TransformPoint(point); 
    
		//map projection
    point2=m_OutputMapProjection->TransformPoint(geopoint); 
    
    return point2;
}
  
} // namespace otb

#endif

