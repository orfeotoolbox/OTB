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
   m_LatFit = LSQREstimatorType::New();
   m_LonFit = LSQREstimatorType::New();
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
   m_LatFit = LSQREstimatorType::New();
   m_LonFit = LSQREstimatorType::New();
   m_XFit = LSQREstimatorType::New();
   m_YFit = LSQREstimatorType::New(); 

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

   computeLS();
}

itk::Point<double, 2> BilinearProjection::worldToLineSample(const Point3DType& worldPoint) const
{
   // convert Point3D to vector double to use
   // LeastSquareBilinearTransformEstimator::lsFitValue
   itk::Vector<double,2>  worldptMatrix;
   worldptMatrix[0] = worldPoint[0];
   worldptMatrix[1] = worldPoint[1];

   double x,y;

   m_XFit->lsFitValue(worldptMatrix, x);
   m_YFit->lsFitValue(worldptMatrix, y);

   Point2DType lineSampPt;
   lineSampPt[0] = x;
   lineSampPt[1] = y;
   return lineSampPt;
}

BilinearProjection::Point3DType BilinearProjection::lineSampleToWorld(Point2DType lineSampPt) const
{
  return lineSampleHeightToWorld(lineSampPt, 0.0);
}

BilinearProjection::Point3DType BilinearProjection::lineSampleHeightToWorld(
                                                Point2DType lineSampPt,
                                                const double& heightAboveEllipsoid) const
{
   itk::Vector<double,2> lineSampMatrix;
   lineSampMatrix[0] = lineSampPt[0];
   lineSampMatrix[1] = lineSampPt[1];
   double lon, lat;
   m_LonFit->lsFitValue(lineSampMatrix, lon);
   m_LatFit->lsFitValue(lineSampMatrix, lat);

   Point3DType worldPt;
   worldPt[0] = lon;
   worldPt[1] = lat;
   worldPt[2] = heightAboveEllipsoid;

   return worldPt;
}

const std::vector<itk::Point<double, 2>>& BilinearProjection::getLineSamplePoints() const
{
   return m_LineSamplePoints;
}

void BilinearProjection::setLineSamplePoints(const std::vector<Point2DType>& lsPts)
{
   m_LineSamplePoints = lsPts;
}

const std::vector<BilinearProjection::Point3DType>& BilinearProjection::getWorldPoints() const
{
   return m_worldPoints;
}

void BilinearProjection::setWorldPoints(const std::vector<Point3DType>& wPts)
{
   m_worldPoints = wPts;
}

void BilinearProjection::computeLS()
{

   if (m_LineSamplePoints.size() != m_worldPoints.size())
   {
      otbGenericExceptionMacro(itk::ExceptionObject, <<"Difference between the number of linesample points and world points");
   }
   
   for (uint64_t i = 0; i < m_LineSamplePoints.size(); ++i)
   {
      Point2DType currentWorldPoint;
      currentWorldPoint[0] = m_worldPoints[i][0];
      currentWorldPoint[1] = m_worldPoints[i][1];
      m_LatFit->AddTiePoints(m_LineSamplePoints[i], currentWorldPoint[1]);
      m_LonFit->AddTiePoints(m_LineSamplePoints[i], currentWorldPoint[0]);
      m_XFit->AddTiePoints(currentWorldPoint, m_LineSamplePoints[i][0]);
      m_YFit->AddTiePoints(currentWorldPoint, m_LineSamplePoints[i][1]);
   }

   m_LatFit->Compute();
   m_LonFit->Compute();
   m_XFit->Compute();
   m_YFit->Compute();      

}

bool BilinearProjection::imgPointsHaveNan()
{
   for(auto const& currentPoint : m_LineSamplePoints)
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
   for(auto const& currentPoint : m_worldPoints)
   {
      if ( std::isnan(currentPoint[0]) || std::isnan(currentPoint[1]) || std::isnan(currentPoint[2]) )
      {
         return true;
      }
   }
   return false;
}
}
