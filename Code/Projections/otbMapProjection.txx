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

#include "projection/ossimMapProjection.h"
#include "base/ossimGpt.h"
#include "base/ossimDpt.h"
#include "projection/ossimProjection.h"
#include "base/ossimEllipsoid.h"
#include "base/ossimEllipsoidFactory.h"
#include "base/ossimString.h"
#include "gdal/ossimOgcWktTranslator.h"

namespace otb
{

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::MapProjection() : Superclass(SpaceDimension, ParametersDimension)
{
  m_MapProjection = NULL;
  m_ProjectionRefWkt.clear();
  this->InstanciateProjection();
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::~MapProjection()
{
  m_MapProjection = NULL;
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
const TOssimMapProjection*
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetMapProjection() const
{
  return this->m_MapProjection.get();
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
TOssimMapProjection*
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetMapProjection()
{
  return this->m_MapProjection.release();
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::InstanciateProjection()
{
  m_MapProjection =  new OssimMapProjectionType();
}

/// Method to set the projection ellipsoid
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid()
{
  ossimEllipsoid ellipsoid;
  m_MapProjection->setEllipsoid(ellipsoid);
}

/// Method to set the projection ellipsoid by copy
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid(const ossimEllipsoid& ellipsoid)
{
  m_MapProjection->setEllipsoid(ellipsoid);
}

///// Method to set the projection ellipsoid by knowing its code
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid(const std::string& code)
{
  const ossimEllipsoid ellipsoid = *(ossimEllipsoidFactory::instance()->create(ossimString(code)));
  m_MapProjection->setEllipsoid(ellipsoid);
}

///// Method to set the projection ellipsoid by knowing its axis
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetEllipsoid(const double& major_axis, const double& minor_axis)
{
  ossimEllipsoid ellipsoid(major_axis, minor_axis);
  m_MapProjection->setEllipsoid(ellipsoid);
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions,
    NOutputDimensions>::OutputPointType
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  if (DirectionOfMapping == TransformDirection::INVERSE)
    {
//     otbMsgDevMacro(<< "Cartographic coordinates: (" << point[0] << "," << point[1] << ")");

    //from "itk::point" to "ossim::ossimDpt"
    ossimDpt ossimDPoint(point[0], point[1]);

    //map projection
    ossimGpt ossimGPoint;
    ossimGPoint = m_MapProjection->inverse(ossimDPoint);
    ossimGPoint.changeDatum(ossimDatumFactory::instance()->wgs84());
//     otbGenericMsgDebugMacro(<< "Inverse : " << std::endl << m_MapProjection->print(std::cout));

    outputPoint[0] = ossimGPoint.lon;
    outputPoint[1] = ossimGPoint.lat;
//     otbMsgDevMacro(<< "Geographic coordinates (lon, lat) : (" << outputPoint[0] << "," << outputPoint[1] << ")");
    }
  if (DirectionOfMapping == TransformDirection::FORWARD)
    {
//     otbMsgDevMacro(<< "Geographic coordinates (lon, lat) : (" << point[1] << "," << point[0] << ")");
    //from "itk::point" to "ossim::ossimGpt"
    ossimGpt ossimGPoint(point[1], point[0]);

    //map projection
    ossimDpt ossimDPoint;
    ossimDPoint = m_MapProjection->forward(ossimGPoint);
//     otbGenericMsgDebugMacro(<< "Forward : ========================= " << std::endl << m_MapProjection->print(std::cout));
    outputPoint[0] = ossimDPoint.x;
    outputPoint[1] = ossimDPoint.y;

//     otbMsgDevMacro(<< "Cartographic coordinates: (" << outputPoint[0] << "," << outputPoint[1] << ")");
    }

  return outputPoint;
}

///\return The geographic point corresponding to (0, 0)
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions,
    NOutputDimensions>::InputPointType
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::Origin()
{
  ossimGpt       ossimOrigin = m_MapProjection->origin();
  InputPointType otbOrigin;
  otbOrigin[0] = ossimOrigin.lat;
  otbOrigin[1] = ossimOrigin.lon;

  return otbOrigin;
}

///\return The False Northing(avoid negative coordinates)
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetFalseNorthing() const
{
  double falseNorthing = m_MapProjection->getFalseNorthing();

  return falseNorthing;
}

///\return The FalseEasting(avoid negative coordinates)
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetFalseEasting() const
{
  double falseEasting = m_MapProjection->getFalseEasting();

  return falseEasting;
}

///\return The StandardParallel1(depends on the projection type)
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetStandardParallel1() const
{
  double standardParallel1 = m_MapProjection->getStandardParallel1();

  return standardParallel1;
}

///\return The StandardParallel2(depends on the projection type)
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetStandardParallel2() const
{
  double standardParallel2 = m_MapProjection->getStandardParallel2();

  return standardParallel2;
}

///\return The projection name
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
std::string
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetProjectionName() const
{
  std::string projectionName;
  projectionName = m_MapProjection->getProjectionName();

  return projectionName;
}

///Check if the projection is geographic
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::IsGeographic() const
{
  return (m_MapProjection->isGeographic());
}

///\return the major axis of the ellipsoid
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetA() const
{
  double majorAxis = m_MapProjection->getA();

  return majorAxis;
}

///\return the minor axis of the ellipsoid
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetB() const
{
  double minorAxis = m_MapProjection->getB();

  return minorAxis;
}

///\return the flatening of the ellipsoid
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
double
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetF() const
{
  double flattening = m_MapProjection->getF();

  return flattening;
}

///\return The resolution in meters
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions,
    NOutputDimensions>::OutputPointType
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetMetersPerPixel() const
{
  ossimDpt        ossimMetersPerPixels = m_MapProjection->getMetersPerPixel();
  OutputPointType metersPerPixels;

  metersPerPixels[0] = ossimMetersPerPixels.x;
  metersPerPixels[1] = ossimMetersPerPixels.y;

  return metersPerPixels;
}

///\return The resolution in degree
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions,
    NOutputDimensions>::OutputPointType
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetDecimalDegreesPerPixel() const
{
  ossimDpt        ossimDecimalDegreesPerPixels = m_MapProjection->getDecimalDegreesPerPixel();
  OutputPointType DecimalDegreesPerPixels;

  DecimalDegreesPerPixels[0] = ossimDecimalDegreesPerPixels.x;
  DecimalDegreesPerPixels[1] = ossimDecimalDegreesPerPixels.y;

  return DecimalDegreesPerPixels;
}

///Set the ellipsoid axis
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetAB(double a, double b)
{
  m_MapProjection->setAB(a, b);
}

///Set the origin
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetOrigin(const InputPointType& origin)
{
  ossimGpt ossimOrigin(origin[1], origin[0]);
  m_MapProjection->setOrigin(ossimOrigin);
}

///Set the origin in a given datum
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetOrigin(const InputPointType& origin, const std::string& datumCode)
{
  ossimGpt ossimOrigin(origin[1], origin[0], 0, ossimDatumFactory::instance()->create(datumCode));
  m_MapProjection->setOrigin(ossimOrigin);
}

///Set the map resolution in meters
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetMetersPerPixel(const OutputPointType& point)
{
  ossimDpt ossimDPoint(point[0], point[1]);
  m_MapProjection->setMetersPerPixel(ossimDPoint);
}

///Set the map resolution in degree
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetDecimalDegreesPerPixel(const OutputPointType& point)
{
  ossimDpt ossimDPoint(point[0], point[1]);
  m_MapProjection->setDecimalDegreesPerPixel(ossimDPoint);
}

///\return an approximation of the resolution in degree
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeDegreesPerPixel(const InputPointType& ground,
                         const OutputPointType& metersPerPixel,
                         double& deltaLat,
                         double& deltaLon)
{
  ossimDpt ossimMetersPerPixel(metersPerPixel[0], metersPerPixel[1]);
  ossimGpt ossimGround(ground[1], ground[0]);
  m_MapProjection->computeDegreesPerPixel(ossimGround, ossimMetersPerPixel, deltaLat, deltaLon);
}

///\return an approximation of the resolution in meters
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeMetersPerPixel(const InputPointType& center,
                        double deltaDegreesPerPixelLat,
                        double deltaDegreesPerPixelLon,
                        OutputPointType& metersPerPixel)
{
  //Correction
  ossimGpt ossimCenter(center[1], center[0]);
  ossimDpt ossimMetersPerPixel;
  m_MapProjection->computeMetersPerPixel(ossimCenter,
                                         deltaDegreesPerPixelLat,
                                         deltaDegreesPerPixelLon,
                                         ossimMetersPerPixel);
  metersPerPixel[0] = ossimMetersPerPixel.x;
  metersPerPixel[1] = ossimMetersPerPixel.y;
}

///\return an approximation of the resolution in meters
template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::ComputeMetersPerPixel(double deltaDegreesPerPixelLat, double deltaDegreesPerPixelLon, OutputPointType& metersPerPixel)
{
  ossimDpt ossimMetersPerPixel;
  m_MapProjection->computeMetersPerPixel(
    m_MapProjection->origin(), deltaDegreesPerPixelLat, deltaDegreesPerPixelLon, ossimMetersPerPixel);
  metersPerPixel[0] = ossimMetersPerPixel.x;
  metersPerPixel[1] = ossimMetersPerPixel.y;
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
std::string
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetWkt() const
{
  ossimKeywordlist kwl;
  m_MapProjection->saveState(kwl);
  ossimOgcWktTranslator wktTranslator;
  std::string           wkt;
  wkt = wktTranslator.fromOssimKwl(kwl);
  return wkt;
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetWkt(const std::string& projectionRefWkt)
{
  this->m_ProjectionRefWkt = projectionRefWkt;
  this->Modified();
}

template<class TOssimMapProjection, TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
MapProjection<TOssimMapProjection, TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::PrintMap() const
{
  std::cout << m_MapProjection->print(std::cout);
}

} // namespace otb

#endif
