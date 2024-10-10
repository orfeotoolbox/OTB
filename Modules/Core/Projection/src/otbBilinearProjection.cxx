/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbBilinearProjection.h"

namespace otb
{
BilinearProjection::BilinearProjection()
{
    m_LonFit = LSQREstimatorType::New();
    m_LatFit = LSQREstimatorType::New();
    m_XFit = LSQREstimatorType::New();
    m_YFit = LSQREstimatorType::New();
}

BilinearProjection::BilinearProjection(const Point2DType& ul,
                           const Point2DType& ur,
                           const Point2DType& lr,
                           const Point2DType& ll,
                           const Point3DType& ulg,
                           const Point3DType& urg,
                           const Point3DType& lrg,
                           const Point3DType& llg)
{
    m_LonFit = LSQREstimatorType::New();
    m_LatFit = LSQREstimatorType::New();
    m_LineSamplePoints.reserve(4);
    m_worldPoints.reserve(4);
    m_LineSamplePoints[0] = ul;
    m_LineSamplePoints[1] = ur;
    m_LineSamplePoints[2] = lr;
    m_LineSamplePoints[3] = ll;

    m_worldPoints[0] = ulg;
    m_worldPoints[1] = urg;
    m_worldPoints[2] = lrg;
    m_worldPoints[3] = llg;

    initializeBilinear();
}

BilinearProjection::~BilinearProjection(){}

void BilinearProjection::worldToLineSample(const Point3DType& worldPoint,
                                  Point2DType&       lineSampPt) const
{
   itk::Vector<double,3>  worldptMatrix;
   worldptMatrix[0] = worldPoint[0];
   worldptMatrix[1] = worldPoint[1];
   worldptMatrix[2] = worldPoint[2];

   // Result is a 3DPoint (3,3 * 3,1 matrix) that we have to convert to 2D points after
   auto lineSpX = m_XFit->GetMatrix() * worldptMatrix;
   auto lineSpY = m_YFit->GetMatrix() * worldptMatrix;

   lineSampPt[0] = lineSpX[0];
   lineSampPt[1] = lineSpY[1];
}

void BilinearProjection::lineSampleToWorld(const Point2DType& lineSampPt,
                                  Point3DType&       worldPt) const
{
 lineSampleHeightToWorld(lineSampPt,
                           0.0,
                           worldPt);
}

void BilinearProjection::lineSampleHeightToWorld(const Point2DType& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        Point3DType&       worldPt) const
{
   itk::Vector<double,3> lineSampMatrix;
   lineSampMatrix[0] = lineSampPt[0];
   lineSampMatrix[1] = lineSampPt[1];
   lineSampMatrix[2] = heightAboveEllipsoid;

   auto worldLon = m_LonFit->GetMatrix() * lineSampMatrix;
   auto worldLat = m_LatFit->GetMatrix() * lineSampMatrix;
   worldPt[0] = worldLon[0];
   worldPt[1] = worldLat[1];
   worldPt[2] = heightAboveEllipsoid;
}

void BilinearProjection::setTiePoints(const std::vector<Point2DType>& lsPt, 
                                    const std::vector<Point3DType>& geoPt)
{
    m_LineSamplePoints = lsPt;
    m_worldPoints = geoPt;
}

void BilinearProjection::getTiePoints(std::vector<Point2DType>& lsPt, std::vector<Point3DType>& geoPt) const
{
    lsPt = m_LineSamplePoints;
    geoPt = m_worldPoints;
}

void BilinearProjection::initializeBilinear()
{
   bool theInterpolationPointsHaveNanFlag = imgPointsHaveNan()||worldPointsHaveNan();
   bool theInverseSupportedFlag = false;
   if(!theInverseSupportedFlag)
   {
    
      if (m_LineSamplePoints.size() != m_worldPoints.size())
      {
         return;
      }
      
      for (auto i = 0; i < m_LineSamplePoints.size(); ++i)
      {
         Point3DType currentLineSamplePoint;
         currentLineSamplePoint[0] = m_LineSamplePoints[i][0];
         currentLineSamplePoint[1] = m_LineSamplePoints[i][1];
         currentLineSamplePoint[2] = m_worldPoints[i][2];
         m_LatFit->AddTiePoints(currentLineSamplePoint,m_worldPoints[i]);
         m_LonFit->AddTiePoints(currentLineSamplePoint,m_worldPoints[i]);
         m_XFit->AddTiePoints(currentLineSamplePoint,m_worldPoints[i]);
         m_YFit->AddTiePoints(currentLineSamplePoint,m_worldPoints[i]);
      }
      
      m_LatFit->Compute();
      m_LonFit->Compute();
      m_XFit->Compute();
      m_YFit->Compute();
   }

   //TODO update boolean flags like ossim
}

bool BilinearProjection::imgPointsHaveNan()
{
   for(auto currentPoint : m_LineSamplePoints)
   {
      if (std::isnan(currentPoint[0]) || std::isnan(currentPoint[1]))
      {
         return true;
      }
   }

   return false;
}

bool BilinearProjection::worldPointsHaveNan()
{
   for(auto currentPoint : m_worldPoints)
   {
      if ( std::isnan(currentPoint[0]) || std::isnan(currentPoint[1]) || std::isnan(currentPoint[2]) )
      {
         return true;
      }
   }
   return false;
}

void BilinearProjection::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Bilinear Projection LatFit Matrix : " << m_LatFit->GetMatrix() << std::endl;
  os << indent << "Bilinear Projection LatFit AffineTransform : " << m_LatFit->GetAffineTransform() << std::endl;
  os << indent << "Bilinear Projection LonFit Matrix : " << m_LonFit->GetMatrix() << std::endl;
  os << indent << "Bilinear Projection LonFit AffineTransform : " << m_LonFit->GetAffineTransform() << std::endl;
}
                        
}