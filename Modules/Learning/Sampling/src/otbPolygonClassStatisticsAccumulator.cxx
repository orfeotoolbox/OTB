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

#include "otbPolygonClassStatisticsAccumulator.h"

namespace otb
{

template <typename TIterator>
void
PolygonClassStatisticsAccumulator
::Add(otb::ogr::Layer::const_iterator& featIt,
      TIterator& imgIt)
{
  // Get class name
  std::string className(featIt->ogr().GetFieldAsString(this->m_FieldIndex));
  // Get Feature Id
  unsigned long featureId = featIt->ogr().GetFID();

  if (m_ElmtsInClass.count(className) == 0)
    {
    m_ElmtsInClass[className] = 0UL;
    }
  if (m_Polygon.count(featureId) == 0)
    {
    m_Polygon[featureId] = 0UL;
    }

  OGRGeometry * geom = featIt->ogr().GetGeometryRef();
  switch (geom->getGeometryType())
    {
    case wkbPoint:
    case wkbPoint25D:
      {
      // TODO
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      // TODO
      break;
      }
    case wkbPolygon:
    case wkbPolygon25D:
      {
      // TODO
      break;
      }
    case wkbMultiPoint:
    case wkbMultiPoint25D:
    case wkbMultiLineString:
    case wkbMultiLineString25D:
    case wkbMultiPolygon:
    case wkbMultiPolygon25D:
    case wkbGeometryCollection:
    case wkbGeometryCollection25D:
      {
      //otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
      break;
      }
    default:
      {
      //otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
      break;
      }
    }

  // Count increments
  // m_NbPixelsGlobal++;
  // m_ElmtsInClass[className]++;
  // m_Polygon[featureId]++;

  //Generation of a random number for the sampling in a polygon where we only need one pixel, it's choosen randomly
  //elmtsInClass[className] = elmtsInClass[className] + nbOfPixelsInGeom;
  //OGRPolygon* inPolygon = dynamic_cast<OGRPolygon *>(geom);
  //OGRLinearRing* exteriorRing = inPolygon->getExteriorRing();
    //itk::Point<double, 2> point;
    //inputImage->TransformIndexToPhysicalPoint(it.GetIndex(), point);
    // ->Test if the current pixel is in a polygon hole
    // If point is in feature
    //if(exteriorRing->isPointInRing(&pointOGR, TRUE) && isNotInHole)
    //{
    //}

}

void
PolygonClassStatisticsAccumulator
::Reset()
{
  m_NbPixelsGlobal = 0UL;
  m_ElmtsInClass.clear();
  m_Polygon.clear();
}


} // end of namespace otb
