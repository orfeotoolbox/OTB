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

namespace otb
{

template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::MapProjection() : Superclass(SpaceDimension,ParametersDimension)
{
	m_MapProjection =  new OssimMapProjectionType();
}
  

template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::~MapProjection()
{
	delete m_MapProjection;
}


///Méthode pour attribuer une ellipse à la projection(4 méthodes)
//Par defaut: 
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid()
{
	ossimEllipsoid ellipsoid=ossimEllipsoid();
	m_MapProjection->setEllipsoid(ellipsoid);
}

//Par copie:
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid(const ossimEllipsoid &ellipsoid)
{
	m_MapProjection->setEllipsoid(ellipsoid);
}

//En connaissant son "code"
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
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

//En connaissant ses axes
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid(const double &major_axis, const double &minor_axis)
{
	ossimEllipsoid ellipse(major_axis,minor_axis);
	
	delete m_MapProjection;
	m_MapProjection= new OssimMapProjectionType(ellipse);
}

///Méthode qui transforme un point géographique en un point cartographique:
//Encapsulation de la méthode "forward"
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::Forward(const InputPointType & point) const
{

	//On transforme le type "itk::point" en type "ossim::ossimGpt" 
  ossimGpt ossimGPoint(point[0], point[1]);
  
  //On le projète sur la carte 
  ossimDpt ossimDPoint;
  ossimDPoint=m_MapProjection->forward(ossimGPoint);

  OutputPointType outputPoint;
  outputPoint[0]=ossimDPoint.x;
  outputPoint[1]=ossimDPoint.y;
  
	return outputPoint;
}

///Méthode qui transforme un point cartographique en un point géographique:
//Encapsulation de la méthode "inverse"
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::InputPointType
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::Inverse(const OutputPointType & point) const
{
	//On transforme le type "itk::point" en type "ossim::ossimGpt" 
  ossimDpt ossimDPoint(point[0], point[1]);
  
  //On le projète sur la carte 
  ossimGpt ossimGPoint;
  ossimGPoint=m_MapProjection->inverse(ossimDPoint);

  InputPointType inputPoint;
  inputPoint[0]=ossimGPoint.lat;
  inputPoint[1]=ossimGPoint.lon;
  
	return inputPoint;
}
    
///Méthode qui retourne le point géographique correspondant à l'index (0;0)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::InputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::Origin()
{
	ossimGpt ossimOrigin=m_MapProjection->origin();
	InputPointType otbOrigin;
	
	otbOrigin[0]= ossimOrigin.lat;
	otbOrigin[1]= ossimOrigin.lon;
	
	return otbOrigin;
}

///Méthode pour récupérer le FalseNorthing(pour éviter les coordonnées négatives)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetFalseNorthing() const
{
	double falseNorthing=m_MapProjection->getFalseNorthing();
	
	return falseNorthing;
}

///Méthode pour récupérer le FalseEasting(pour éviter les coordonnées négatives)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetFalseEasting() const
{
	double falseEasting=m_MapProjection->getFalseEasting();
	
	return falseEasting;
}

///Méthode pour récupérer le StandardParallel1(# selon le type de projection)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetStandardParallel1() const
{
	double standardParallel1=m_MapProjection->getStandardParallel1();
	
	return standardParallel1;
}

///Méthode pour récupérer le StandardParallel2(# selon le type de projection)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetStandardParallel2() const
{
	double standardParallel2=m_MapProjection->getStandardParallel2();
	
	return standardParallel2;
}

///Méthode pour récupérer le nom de la projection
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
std::string 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetProjectionName() const
{
	std::string projectionName;
	projectionName=m_MapProjection->getProjectionName();
	
	return projectionName;
}

///Méthode pour vérifier si...
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::IsGeographic() const
{
	return (m_MapProjection->isGeographic());
}

///Méthode pour récupérer le "major axis" d'une ellipse
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetA() const
{
	double majorAxis=m_MapProjection->getA();
	
	return majorAxis;
}

///Méthode pour récupérer le "minor axis" d'une ellipse
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetB() const
{
	double minorAxis=m_MapProjection->getB();
	
	return minorAxis;
}

///Méthode pour récupérer le "Flattening" d'une ellipse
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
double 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetF() const
{
	double flattening=m_MapProjection->getF();
	
	return flattening;
}

///Méthode qui retourne la résolution (mètres/pixel)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetMetersPerPixel() const
{
	ossimDpt ossimMetersPerPixels=m_MapProjection->getMetersPerPixel();
  OutputPointType metersPerPixels;
  
	metersPerPixels[0]=ossimMetersPerPixels.x;
  metersPerPixels[1]=ossimMetersPerPixels.y;
  
	return metersPerPixels;
}

///Méthode qui retourne la résolution (degrés/pixel)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
typename MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::GetDecimalDegreesPerPixel() const
{
	ossimDpt ossimDecimalDegreesPerPixels=m_MapProjection->getDecimalDegreesPerPixel();
  OutputPointType DecimalDegreesPerPixels;
  
	DecimalDegreesPerPixels[0]=ossimDecimalDegreesPerPixels.x;
  DecimalDegreesPerPixels[1]=ossimDecimalDegreesPerPixels.y;
  
	return DecimalDegreesPerPixels;
}

///Méthode pour attribuer les axes à l'ellipse
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetAB(double a, double b) 
{
	m_MapProjection->setAB(a,b);
}

///Méthode pour attribuer l'origine
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetOrigin(const InputPointType &origin) 
{
	ossimGpt ossimOrigin(origin[0], origin[1]);
	m_MapProjection->setOrigin(ossimOrigin);
}

///Méthode pour fixer une résolution de la carte
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetMetersPerPixel(const OutputPointType &point) 
{ossimDpt ossimDPoint(point[0], point[1]);
m_MapProjection->setMetersPerPixel(ossimDPoint);
}

///Méthode pour fixer une résolution de la carte
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::SetDecimalDegreesPerPixel(const OutputPointType &point) 
{
	ossimDpt ossimDPoint(point[0], point[1]);
	m_MapProjection->setDecimalDegreesPerPixel(ossimDPoint);
}

///Méthode pour calculer approximativement la résolution(degré/pixel)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeDegreesPerPixel(const InputPointType &ground, const OutputPointType &metersPerPixel, double &deltaLat, double &deltaLon) 
{
	ossimDpt ossimMetersPerPixel(metersPerPixel[0], metersPerPixel[1]);
	ossimGpt ossimGround(ground[0],ground[1]);
	
	m_MapProjection->computeDegreesPerPixel(ossimGround,ossimMetersPerPixel,deltaLat,deltaLon);
}

///Méthode pour calculer approximativement la résolution(mètre/pixel)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
void
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeMetersPerPixel(const InputPointType &center, double deltaDegreesPerPixelLat, double deltaDegreesPerPixelLon, OutputPointType &metersPerPixel) 
{
	//Correction 
	ossimGpt ossimCenter(center[0],center[1]);
	ossimDpt ossimMetersPerPixel;
	m_MapProjection->computeMetersPerPixel(ossimCenter,deltaDegreesPerPixelLat, deltaDegreesPerPixelLon,ossimMetersPerPixel);
	metersPerPixel[0]=ossimMetersPerPixel.x;
	metersPerPixel[1]=ossimMetersPerPixel.y;
}

///Méthode pour calculer approximativement la résolution(mètre/pixel)
template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
void 
MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeMetersPerPixel(double deltaDegreesPerPixelLat, double deltaDegreesPerPixelLon, OutputPointType &metersPerPixel) 
{
	ossimDpt ossimMetersPerPixel;
	m_MapProjection->computeMetersPerPixel(deltaDegreesPerPixelLat, deltaDegreesPerPixelLon,ossimMetersPerPixel);
	metersPerPixel[0]=ossimMetersPerPixel.x;
	metersPerPixel[1]=ossimMetersPerPixel.y;
}

///Non implémenté dans ossim.
// template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::SetMatrix(double rotation,  const OutputPointType &scale, const OutputPointType &translation) 
// {
// ossimDpt ossimScale(scale[0], scale[1]);
// ossimDpt ossimTranslation(translation[0], translation[1]);
// m_MapProjection->setMatrix(rotation,ossimScale, ossimTranslation); 
// }

///Méthode pour attribuer un falseEasting
// template<class TOssimMapProjection, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> void MapProjection<TOssimMapProjection, TScalarType, NInputDimensions, NOutputDimensions>::SetFalseEasting(double falseEasting) 
// { std::string projectionName=m_MapProjection->getProjectionName();
// if (projectionName=="ossimUtmProjection")
// {m_MapProjection->setFalseEasting(falseEasting);
// }
// else {std::cout << "Erreur:Méthode non applicable à cette projection" << std::endl;}
// }

} // namespace otb

#endif
