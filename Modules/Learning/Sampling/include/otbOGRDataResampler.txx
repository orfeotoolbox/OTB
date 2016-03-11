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
#ifndef __otbOGRDataResampler_txx
#define __otbOGRDataResampler_txx

#include "otbOGRDataResampler.h"
#include "otbMacro.h"

namespace otb
{

template <typename TIterator>
void
OGRDataResampler
::Add(otb::ogr::Layer::const_iterator& featIt,
      TIterator& imgIt)
{
  // Get class name
  std::string className(featIt->ogr().GetFieldAsString(this->m_FieldIndex));
  
  // Get Feature Id
  unsigned long featureId = featIt->ogr().GetFID();
  
  this->AddGeometry(featIt->ogr().GetGeometryRef(),
                    imgIt,
                    featureId,
                    className);
}

template <typename TIterator>
void
OGRDataResampler
::AddGeometry(OGRGeometry *geom,
              TIterator& imgIt,
              unsigned long &fId,
              std::string &className)
{


  typename TIterator::ImageType::PointType imgPoint;
  typename TIterator::IndexType imgIndex;
  OGRPoint tmpPoint(0.0,0.0,0.0);
  imgIt.GoToBegin();
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
      if (!imgIt.IsAtEnd())
        {
        if (imgIndex == imgIt.GetIndex())
         if (imgIt.Get()>0)
          if (TakeSample(className))
          {
              std::pair<double, double> phyPos = std::make_pair(imgPoint[0],imgPoint[1]);
              m_ClassToPhyPositions[className].push_back(phyPos);
          }
        } 
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      OGRPolygon tmpPolygon;
      OGRLinearRing ring;
      ring.addPoint(0.0,0.0,0.0);
      ring.addPoint(1.0,0.0,0.0);
      ring.addPoint(1.0,1.0,0.0);
      ring.addPoint(0.0,1.0,0.0);
      ring.addPoint(0.0,0.0,0.0);
      tmpPolygon.addRing(&ring);
      typename TIterator::ImageType::SpacingType imgAbsSpacing;
      imgAbsSpacing = imgIt.GetImage()->GetSpacing();
      if (imgAbsSpacing[0] < 0) imgAbsSpacing[0] = -imgAbsSpacing[0];
      if (imgAbsSpacing[1] < 0) imgAbsSpacing[1] = -imgAbsSpacing[1];
      while (!imgIt.IsAtEnd())
        {
        imgIt.GetImage()->TransformIndexToPhysicalPoint(imgIt.GetIndex(),imgPoint);
        tmpPolygon.getExteriorRing()->setPoint(0
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(1
          ,imgPoint[0]+0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(2
          ,imgPoint[0]+0.5*imgAbsSpacing[0]
          ,imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(3
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(4
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        if (geom->Intersects(&tmpPolygon)) 
         if (imgIt.Get()>0)
          if (TakeSample(className))
          {
              std::pair<double, double> phyPos = std::make_pair(imgPoint[0],imgPoint[1]);
              m_ClassToPhyPositions[className].push_back(phyPos);
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
         if (imgIt.Get()>0)
          if (TakeSample(className))
          {
              std::pair<double, double> phyPos = std::make_pair(imgPoint[0],imgPoint[1]);
              m_ClassToPhyPositions[className].push_back(phyPos);
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
      OGRGeometryCollection *geomCollection = dynamic_cast<OGRGeometryCollection*>(geom);
      if (geomCollection)
        {
        int nbGeom =  geomCollection->getNumGeometries();
        for ( int i=0 ; i < nbGeom ; ++i)
          {
          this->AddGeometry(geomCollection->getGeometryRef(i),
                            imgIt,
                            fId,
                            className);
          }
        }
      else
        {
        otbWarningMacro("Geometry not recognized as a collection : " << geom->getGeometryName());
        }
      break;
      }
    default:
      {
      otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
      break;
      }
    }
}

} // end of namespace otb

#endif
