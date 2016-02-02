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

  typename TIterator::ImageType::PointType imgPoint;
  typename TIterator::IndexType imgIndex;
  OGRPoint tmpPoint(0.0,0.0,0.0);
  imgIt.GoToBegin();
  OGRGeometry * geom = featIt->ogr().GetGeometryRef();
  switch (geom->getGeometryType())
    {
    case wkbPoint:
    case wkbPoint25D:
      {
      OGRPoint* castPoint = dynamic_cast<OGRPoint*>(geom);
      if (castPoint == NULL)
        {
        // Wrong Type !
        break;
        }
      imgPoint[0] = castPoint->getX();
      imgPoint[1] = castPoint->getY();
      imgIt.GetImage()->TransformPhysicalPointToIndex(imgPoint,imgIndex);
      while (!imgIt.IsAtEnd())
        {
        if (imgIndex == imgIt.GetIndex())
          {
          m_NbPixelsGlobal++;
          m_ElmtsInClass[className]++;
          m_Polygon[featureId]++;
          break;
          }
        }
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      OGRPolygon tmpPolygon;
      tmpPolygon.getExteriorRing()->addPoint(0.0,0.0,0.0);
      tmpPolygon.getExteriorRing()->addPoint(1.0,0.0,0.0);
      tmpPolygon.getExteriorRing()->addPoint(1.0,1.0,0.0);
      tmpPolygon.getExteriorRing()->addPoint(0.0,1.0,0.0);
      tmpPolygon.getExteriorRing()->addPoint(0.0,0.0,0.0);
      typename TIterator::ImageType::SpacingType imgAbsSpacing;
      imgAbsSpacing = imgIt.GetImage()->GetSpacing();
      if (imgAbsSpacing[0] < 0) imgAbsSpacing[0] = -imgAbsSpacing[0];
      if (imgAbsSpacing[1] < 0) imgAbsSpacing[1] = -imgAbsSpacing[1];
      while (!imgIt.IsAtEnd())
        {
        imgIt.GetImage()->TransformIndexToPhysicalPoint(imgIt.GetIndex(),imgPoint);
        tmpPolygon.getExteriorRing()->setPoint(
          imgPoint[0]-0.5*imgAbsSpacing[0],
          imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(
          imgPoint[0]+0.5*imgAbsSpacing[0],
          imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(
          imgPoint[0]+0.5*imgAbsSpacing[0],
          imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(
          imgPoint[0]-0.5*imgAbsSpacing[0],
          imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(
          imgPoint[0]-0.5*imgAbsSpacing[0],
          imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        if (geom->Intersects(&tmpPolygon))
          {
          m_NbPixelsGlobal++;
          m_ElmtsInClass[className]++;
          m_Polygon[featureId]++;
          }
        ++imgIt;
        }
      break;
      }
    case wkbPolygon:
    case wkbPolygon25D:
      {
      while (!imgIt.IsAtEnd())
        {
        imgIt.GetImage()->TransformIndexToPhysicalPoint(imgIt.GetIndex(),imgPoint);
        tmpPoint.setX(imgPoint[0]);
        tmpPoint.setY(imgPoint[1]);
        if (geom->Contains(&tmpPoint))
          {
          m_NbPixelsGlobal++;
          m_ElmtsInClass[className]++;
          m_Polygon[featureId]++;
          }
        ++imgIt;
        }
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
