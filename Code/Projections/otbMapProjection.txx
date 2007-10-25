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
#ifndef __otbMapProjection_txx
#define __otbMapProjection_txx

#include "otbMapProjection.h"
#include "otbMacro.h"

namespace otb
{

template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::MapProjection() : Superclass(SpaceDimension,ParametersDimension)
{
	m_MapProjection =  new OssimMapProjectionType();
}
  

template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::~MapProjection()
{
	delete m_MapProjection;
}


/// Method to set the projection ellipsoid
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetEllipsoid()
{
	ossimEllipsoid ellipsoid=ossimEllipsoid();
	m_MapProjection->setEllipsoid(ellipsoid);
}
/// Method to set the projection ellipsoid by copy
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetEllipsoid(const ossimEllipsoid &ellipsoid)
{
	m_MapProjection->setEllipsoid(ellipsoid);
}

///// Method to set the projection ellipsoid by knowing its code
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetEllipsoid(std::string code)
{
	const ossimEllipsoid *ellipsoid = ossimEllipsoidFactory::instance()->create(ossimString(code));
	double a=ellipsoid->getA();
	//std::cout <<a<< std::endl;
	double b=ellipsoid->getB();
	//std::cout <<b<< std::endl;
	//ossimString codes=ellipsoid->name();
	//std::cout <<codes<< std::endl;
	ossimEllipsoid ellipse(a,b);
	
	delete m_MapProjection;
	m_MapProjection= new OssimMapProjectionType(ellipse);
}

///// Method to set the projection ellipsoid by knowing its axis
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetEllipsoid(const double &major_axis, const double &minor_axis)
{
	ossimEllipsoid ellipse(major_axis,minor_axis);
	
	delete m_MapProjection;
	m_MapProjection= new OssimMapProjectionType(ellipse);
}

template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>::OutputPointType
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::TransformPoint(const InputPointType & point) const
{
	OutputPointType outputPoint;
	
	switch(DirectionOfMapping)
	  {
	case INVERSE:
	  {
	    otbGenericMsgDebugMacro(<< "Cartographic coordinates: (" << point[0] << "," << point[1] << ")");
			
	    //from "itk::point" to "ossim::ossimDpt" 
	    ossimDpt ossimDPoint(point[0], point[1]);
	    
	    //map projection
	    ossimGpt ossimGPoint;
	    ossimGPoint=m_MapProjection->inverse(ossimDPoint);
	    
	    outputPoint[0]=ossimGPoint.lon;
	    outputPoint[1]=ossimGPoint.lat;
			otbGenericMsgDebugMacro(<< "Geographic coordinates (long/lat) : (" << outputPoint[0] << "," << outputPoint[1] << ")");
	    break;
	  }
	  case FORWARD:
	{
		//from "itk::point" to "ossim::ossimGpt"
	  ossimGpt ossimGPoint(point[1], point[0]);
	  
	  //map projection
	  ossimDpt ossimDPoint;
	  ossimDPoint=m_MapProjection->forward(ossimGPoint);
	  
	  outputPoint[0]=ossimDPoint.x;
	  outputPoint[1]=ossimDPoint.y;
	  break;
	}
	  default:
	    {
	      itkExceptionMacro(<<"Model is INVERSE or FORWARD only !!");
	      break;
	    }	
	  }
  
	return outputPoint;
}


    
///\return The geographic point corresponding to (0,0)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>::InputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::Origin()
{
	ossimGpt ossimOrigin=m_MapProjection->origin();
	InputPointType otbOrigin;
	
	otbOrigin[0]= ossimOrigin.lat;
	otbOrigin[1]= ossimOrigin.lon;
	
	return otbOrigin;
}

///\return The False Northing(avoid negative coordinates)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetFalseNorthing() const
{
	double falseNorthing=m_MapProjection->getFalseNorthing();
	
	return falseNorthing;
}

///\return The FalseEasting(avoid negative coordinates)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetFalseEasting() const
{
	double falseEasting=m_MapProjection->getFalseEasting();
	
	return falseEasting;
}

///\return The StandardParallel1(depends on the projection type)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetStandardParallel1() const
{
	double standardParallel1=m_MapProjection->getStandardParallel1();
	
	return standardParallel1;
}

///\return The StandardParallel2(depends on the projection type)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetStandardParallel2() const
{
	double standardParallel2=m_MapProjection->getStandardParallel2();
	
	return standardParallel2;
}

///\return The projection name
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
std::string 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetProjectionName() const
{
	std::string projectionName;
	projectionName=m_MapProjection->getProjectionName();
	
	return projectionName;
}

///Check if the projection is geographic
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
bool 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::IsGeographic() const
{
	return (m_MapProjection->isGeographic());
}

///\return the major axis of the ellipsoid
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetA() const
{
	double majorAxis=m_MapProjection->getA();
	
	return majorAxis;
}

///\return the minor axis of the ellipsoid
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetB() const
{
	double minorAxis=m_MapProjection->getB();
	
	return minorAxis;
}

///\return the flatening of the ellipsoid
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetF() const
{
	double flattening=m_MapProjection->getF();
	
	return flattening;
}

///\return The resolution in meters
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>::OutputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetMetersPerPixel() const
{
	ossimDpt ossimMetersPerPixels=m_MapProjection->getMetersPerPixel();
  OutputPointType metersPerPixels;
  
	metersPerPixels[0]=ossimMetersPerPixels.x;
  metersPerPixels[1]=ossimMetersPerPixels.y;
  
	return metersPerPixels;
}

///\return The resolution in degree
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>::OutputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::GetDecimalDegreesPerPixel() const
{
	ossimDpt ossimDecimalDegreesPerPixels=m_MapProjection->getDecimalDegreesPerPixel();
  OutputPointType DecimalDegreesPerPixels;
  
	DecimalDegreesPerPixels[0]=ossimDecimalDegreesPerPixels.x;
  DecimalDegreesPerPixels[1]=ossimDecimalDegreesPerPixels.y;
  
	return DecimalDegreesPerPixels;
}

///Set the ellipsoid axis
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetAB(double a, double b) 
{
	m_MapProjection->setAB(a,b);
}

///Set the origin
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetOrigin(const InputPointType &origin) 
{
	ossimGpt ossimOrigin(origin[0], origin[1]);
	m_MapProjection->setOrigin(ossimOrigin);
}

///Set the map resolution in meters
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetMetersPerPixel(const OutputPointType &point) 
{ossimDpt ossimDPoint(point[0], point[1]);
m_MapProjection->setMetersPerPixel(ossimDPoint);
}

///Set the map resolution in degree
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::SetDecimalDegreesPerPixel(const OutputPointType &point) 
{
	ossimDpt ossimDPoint(point[0], point[1]);
	m_MapProjection->setDecimalDegreesPerPixel(ossimDPoint);
}

///\return an approximation of the resolution in degree
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::ComputeDegreesPerPixel(const InputPointType &ground, const OutputPointType &metersPerPixel, double &deltaLat, double &deltaLon) 
{
	ossimDpt ossimMetersPerPixel(metersPerPixel[0], metersPerPixel[1]);
	ossimGpt ossimGround(ground[0],ground[1]);
	
	m_MapProjection->computeDegreesPerPixel(ossimGround,ossimMetersPerPixel,deltaLat,deltaLon);
}

///\return an approximation of the resolution in meters
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
void
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::ComputeMetersPerPixel(const InputPointType &center, double deltaDegreesPerPixelLat, double deltaDegreesPerPixelLon, OutputPointType &metersPerPixel) 
{
	//Correction 
	ossimGpt ossimCenter(center[0],center[1]);
	ossimDpt ossimMetersPerPixel;
	m_MapProjection->computeMetersPerPixel(ossimCenter,deltaDegreesPerPixelLat, deltaDegreesPerPixelLon,ossimMetersPerPixel);
	metersPerPixel[0]=ossimMetersPerPixel.x;
	metersPerPixel[1]=ossimMetersPerPixel.y;
}

///\return an approximation of the resolution in meters
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions, InverseOrForwardTransformationEnum Transform> 
void 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions, Transform>
::ComputeMetersPerPixel(double deltaDegreesPerPixelLat, double deltaDegreesPerPixelLon, OutputPointType &metersPerPixel) 
{
	ossimDpt ossimMetersPerPixel;
	m_MapProjection->computeMetersPerPixel(deltaDegreesPerPixelLat, deltaDegreesPerPixelLon,ossimMetersPerPixel);
	metersPerPixel[0]=ossimMetersPerPixel.x;
	metersPerPixel[1]=ossimMetersPerPixel.y;
}
} // namespace otb

#endif
