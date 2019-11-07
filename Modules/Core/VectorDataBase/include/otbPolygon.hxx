/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPolygon_hxx
#define otbPolygon_hxx

#include "otbPolygon.h"

namespace otb
{

template <class TValue>
void Polygon<TValue>::AddVertex(const ContinuousIndexType& vertex)
{
  Superclass::AddVertex(vertex);
  m_AreaIsValid = false;
}

/**
 * Check whether point is strictly inside the polygon.
 * \param point The point to check.
 * \return True if the point is inside the polygon.
 */

template <class TValue>
bool Polygon<TValue>::IsInside(VertexType point) const
{
  double                      x             = point[0];
  double                      y             = point[1];
  unsigned int                crossingCount = 0;
  VertexListConstIteratorType it            = this->GetVertexList()->Begin();
  double                      xa            = it.Value()[0];
  double                      ya            = it.Value()[1];
  ++it;
  while (it != this->GetVertexList()->End())
  {
    double xb = it.Value()[0];
    double yb = it.Value()[1];
    if (std::abs(xb - xa) < m_Epsilon)
    {
      if (ya > yb && xa > x && y >= yb && y < ya)
      {
        ++crossingCount;
      }
      else if (ya < yb && xa > x && y >= ya && y < yb)
      {
        ++crossingCount;
      }
    }
    else if (std::abs(yb - ya) >= m_Epsilon)
    {
      double xcross = xa + (xb - xa) * (y - ya) / (yb - ya);

      if (ya > yb && xcross > x && y >= yb && y < ya)
      {
        ++crossingCount;
      }
      else if (ya < yb && xcross > x && y >= ya && y < yb)
      {
        ++crossingCount;
      }
    }
    xa = xb;
    ya = yb;
    ++it;
  }
  double xb = this->GetVertexList()->Begin().Value()[0];
  double yb = this->GetVertexList()->Begin().Value()[1];
  if (std::abs(xb - xa) < m_Epsilon)
  {
    if (ya > yb && xa > x && y >= yb && y < ya)
    {
      ++crossingCount;
    }
    else if (ya < yb && xa > x && y >= ya && y < yb)
    {
      ++crossingCount;
    }
  }
  else if (std::abs(yb - ya) >= m_Epsilon)
  {
    double xcross = xa + (xb - xa) * (y - ya) / (yb - ya);

    if (ya > yb && xcross > x && y >= yb && y < ya)
    {
      ++crossingCount;
    }
    else if (ya < yb && xcross > x && y >= ya && y < yb)
    {
      ++crossingCount;
    }
  }
  // std::cout<<"Crossing count: "<<crossingCount<<std::endl;
  return (crossingCount % 2 == 1);
}

/**
 * Check whether point is strictly on the edge of the polygon.
 * \param point The point to check.
 * \return True if the point is on the edge of the polygon.
 */
template <class TValue>
bool Polygon<TValue>::IsOnEdge(VertexType point) const
{
  // std::cout<<"Checking point "<<point<<std::endl;
  bool                        resp = false;
  double                      x    = point[0];
  double                      y    = point[1];
  double                      xb, yb;
  VertexListConstIteratorType it     = this->GetVertexList()->Begin();
  double                      xa     = it.Value()[0];
  double                      ya     = it.Value()[1];
  double                      xbegin = xa;
  double                      ybegin = ya;
  ++it;
  while (!resp && it != this->GetVertexList()->End())
  {
    xb = it.Value()[0];
    yb = it.Value()[1];
    if (std::abs(xb - xa) >= m_Epsilon)
    {
      double cd   = (yb - ya) / (xb - xa);
      double oo   = (ya - cd * xa);
      double xmin = std::min(xa, xb);
      double xmax = std::max(xa, xb);
      if ((std::abs(y - cd * x - oo) < m_Epsilon) && (x <= xmax) && (x >= xmin))
      {
        // std::cout<<"Case 1: Point is on segment "<<xa<<", "<<ya<<" <-> "<<xb<<", "<<yb<<std::endl;
        resp = true;
      }
    }
    else
    {
      double ymin = std::min(ya, yb);
      double ymax = std::max(ya, yb);
      if ((std::abs(x - xa) < m_Epsilon) && (y <= ymax) && (y >= ymin))
      {
        resp = true;
        // std::cout<<"Case 2: Point is on segment "<<xa<<", "<<ya<<" <-> "<<xb<<", "<<yb<<std::endl;
      }
    }
    xa = xb;
    ya = yb;
    ++it;
  }
  xb = xbegin;
  yb = ybegin;
  if (std::abs(xb - xa) >= m_Epsilon)
  {
    double cd   = (yb - ya) / (xb - xa);
    double oo   = (ya - cd * xa);
    double xmin = std::min(xa, xb);
    double xmax = std::max(xa, xb);

    if ((std::abs(y - cd * x - oo) < m_Epsilon) && (x <= xmax) && (x >= xmin))
    {
      resp = true;
      // std::cout<<"Case 1: Point is on segment "<<xa<<", "<<ya<<" <-> "<<xb<<", "<<yb<<std::endl;
    }
  }
  else
  {
    double ymin = std::min(ya, yb);
    double ymax = std::max(ya, yb);
    if ((std::abs(x - xa) <= m_Epsilon) && (y <= ymax) && (y >= ymin))
    {
      resp = true;
      // std::cout<<"Case 2: Point is on segment "<<xa<<", "<<ya<<" <-> "<<xb<<", "<<yb<<std::endl;
    }
  }
  // std::cout<<"Result: "<<resp<<std::endl;
  return resp;
}
/**
 * Returns the number of crossings of the polygon with a given segment.
 * \param a First point of the segment,
 * \param b Second point of the segment,
 * \return the number of strict crossings of segment [ab] with the polygon.
 */
template <class TValue>
unsigned int Polygon<TValue>::NbCrossing(VertexType a, VertexType b) const
{
  unsigned int                resp    = 0;
  VertexListConstIteratorType it      = this->GetVertexList()->Begin();
  VertexListConstIteratorType it_end  = this->GetVertexList()->End();
  VertexType                  current = it.Value();
  VertexType                  first   = current;
  ++it;
  while (it != it_end)
  {
    // std::cout<<"Testing Is crossing "<<a<<" "<<b<<current<<it.Value()<<" = "<<IsCrossing(a, b, current, it.Value())<<std::endl;
    if (IsCrossing(a, b, current, it.Value()))
    {
      ++resp;
    }
    current = it.Value();
    ++it;
  }
  // std::cout<<"Testing Is crossing "<<a<<" "<<b<<current<<first<<" = "<<IsCrossing(a, b, current, first)<<std::endl;
  if (IsCrossing(a, b, current, first))
  {
    ++resp;
  }
  return resp;
}
/**
 * Returns the number of touchings without crossing of the polygon with a given segment.
 * \param a First point of the segment,
 * \param b Second point of the segment,
 * \return the number of touchings without crossing of segment [ab] with the polygon.
 */
template <class TValue>
unsigned int Polygon<TValue>::NbTouching(VertexType a, VertexType b) const
{
  unsigned int                resp    = 0;
  VertexListConstIteratorType it      = this->GetVertexList()->Begin();
  VertexListConstIteratorType it_end  = this->GetVertexList()->End();
  VertexType                  current = it.Value();
  VertexType                  first   = current;
  ++it;
  while (it != it_end)
  {
    // std::cout<<"IsTouching "<<a<<" "<<b<<", "<<current<<" "<<it.Value()<<" -> "<<IsTouching(a, b, current, it.Value())<<std::endl;
    if (IsTouching(a, b, current, it.Value()))
    {
      ++resp;
    }
    current = it.Value();
    ++it;
  }
  // std::cout<<"IsTouching "<<a<<" "<<b<<", "<<current<<" "<<first<<" -> "<<IsTouching(a, b, current, first)<<std::endl;
  if (IsTouching(a, b, current, first))
  {
    ++resp;
  }
  return resp;
}
/**
 * Check whether two segments [a1a2] and [b1b2] are strictly crossing.
 * \param a1 First point of the first segment,
 * \param a1 Second point of the first segment,
 * \param a1 First point of the second segment,
 * \param a1 Second point of the second segment.
 * \return True if the two segments are strictly crossing.
 */
template <class TValue>
bool Polygon<TValue>::IsCrossing(VertexType a1, VertexType a2, VertexType b1, VertexType b2) const
{
  bool   resp  = false;
  double xbmin = std::min(b1[0], b2[0]);
  double xbmax = std::max(b1[0], b2[0]);
  double ybmin = std::min(b1[1], b2[1]);
  double ybmax = std::max(b1[1], b2[1]);
  double xamin = std::min(a1[0], a2[0]);
  double xamax = std::max(a1[0], a2[0]);
  double yamin = std::min(a1[1], a2[1]);
  double yamax = std::max(a1[1], a2[1]);
  if (std::abs(a1[0] - a2[0]) < m_Epsilon && std::abs(b1[0] - b2[0]) < m_Epsilon)
  {
    resp = false;
  }
  else if (std::abs(a1[0] - a2[0]) < m_Epsilon)
  {
    double cd2    = (b2[1] - b1[1]) / (b2[0] - b1[0]);
    double oo2    = b1[1] - cd2 * b1[0];
    double ycross = cd2 * a1[0] + oo2;
    resp          = (xbmin < a1[0] && xbmax > a1[0] && yamin < ycross && yamax > ycross);
  }
  else if (std::abs(b1[0] - b2[0]) < m_Epsilon)
  {
    double cd1    = (a2[1] - a1[1]) / (a2[0] - a1[0]);
    double oo1    = a1[1] - cd1 * a1[0];
    double ycross = cd1 * b1[0] + oo1;
    resp          = (xamin < b1[0] && xamax > b1[0] && ybmin < ycross && ybmax > ycross);
  }
  else
  {
    double cd1 = (a2[1] - a1[1]) / (a2[0] - a1[0]);
    double oo1 = a1[1] - cd1 * a1[0];
    double cd2 = (b2[1] - b1[1]) / (b2[0] - b1[0]);
    double oo2 = b1[1] - cd2 * b1[0];
    if (cd1 != cd2)
    {
      double xcross = (oo2 - oo1) / (cd1 - cd2);
      resp          = (xamin < xcross && xbmin < xcross && xamax > xcross && xbmax > xcross);
    }
  }
  return resp;
}
/**
 * Check whether two segments[a1a2] and [b1b2] are touching without crossing.
 * \param a1 First point of the first segment,
 * \param a1 Second point of the first segment,
 * \param a1 First point of the second segment,
 * \param a1 Second point of the second segment.
 * \return True if the two segments are touching without crossing.
 */
template <class TValue>
bool Polygon<TValue>::IsTouching(VertexType a1, VertexType a2, VertexType b1, VertexType b2) const
{
  bool   resp  = false;
  double xbmin = std::min(b1[0], b2[0]);
  double xbmax = std::max(b1[0], b2[0]);
  double ybmin = std::min(b1[1], b2[1]);
  double ybmax = std::max(b1[1], b2[1]);
  double xamin = std::min(a1[0], a2[0]);
  double xamax = std::max(a1[0], a2[0]);
  double yamin = std::min(a1[1], a2[1]);
  double yamax = std::max(a1[1], a2[1]);
  if (std::abs(a1[0] - a2[0]) < m_Epsilon && std::abs(b1[0] - b2[0]) < m_Epsilon)
  {
    resp = (std::abs(a1[0] - b1[0]) < m_Epsilon) && ((a1[1] <= ybmax && a1[1] >= ybmin) || (a2[1] <= ybmax && a2[1] >= ybmin) ||
                                                     (b1[1] <= yamax && b1[1] >= yamin) || (b2[1] <= yamax && b2[1] >= yamin));
  }
  else if (std::abs(a1[0] - a2[0]) < m_Epsilon)
  {
    double cd2 = (b2[1] - b1[1]) / (b2[0] - b1[0]);
    double oo2 = b1[1] - cd2 * b1[0];

    if (std::abs(a1[1] - cd2 * a1[0] - oo2) < m_Epsilon)
    {
      resp = (a1[0] >= xbmin && a1[0] <= xbmax);
    }
    else if (std::abs(a2[1] - cd2 * a2[0] - oo2) < m_Epsilon)
    {
      resp = (a2[0] >= xbmin && a2[0] <= xbmax);
    }
  }
  else if (std::abs(b1[0] - b2[0]) < m_Epsilon)
  {
    double cd1 = (a2[1] - a1[1]) / (a2[0] - a1[0]);
    double oo1 = a1[1] - cd1 * a1[0];

    if (std::abs(b1[1] - cd1 * b1[0] - oo1) < m_Epsilon)
    {
      resp = (b1[0] >= xamin && b1[0] <= xamax);
    }
    else if (std::abs(b2[1] - cd1 * b2[0] - oo1) < m_Epsilon)
    {
      resp = (b2[0] >= xamin && b2[0] <= xamax);
    }
  }
  else
  {
    double cd1 = (a2[1] - a1[1]) / (a2[0] - a1[0]);
    double oo1 = a1[1] - cd1 * a1[0];
    double cd2 = (b2[1] - b1[1]) / (b2[0] - b1[0]);
    double oo2 = b1[1] - cd2 * b1[0];
    if (std::abs(cd1 - cd2) < m_Epsilon && std::abs(oo1 - oo2) < m_Epsilon)
    {
      resp = ((xamin <= xbmax && xamin >= xbmin) || (xamax <= xbmax && xamax >= xbmin) || (xbmin <= xamax && xbmin >= xamin) ||
              (xbmax <= xamax && xbmax >= xamin));
    }
    else
    {
      if (std::abs(a1[1] - cd2 * a1[0] - oo2) < m_Epsilon)
      {
        resp = (a1[0] >= xbmin && a1[0] <= xbmax);
      }
      else if (std::abs(a2[1] - cd2 * a2[0] - oo2) < m_Epsilon)
      {
        resp = (a2[0] >= xbmin && a2[0] <= xbmax);
      }
      if (std::abs(b1[1] - cd1 * b1[0] - oo1) < m_Epsilon)
      {
        resp = (b1[0] >= xamin && b1[0] <= xamax);
      }
      else if (std::abs(b2[1] - cd1 * b2[0] - oo1) < m_Epsilon)
      {
        resp = (b2[0] >= xamin && b2[0] <= xamax);
      }
    }
  }
  return resp;
}

/**
 * Area computation (for non convex polygons as well)
 */
template <class TValue>
void Polygon<TValue>::ComputeArea() const
{
  VertexListConstIteratorType it = this->GetVertexList()->Begin();

  if (this->GetVertexList()->Size() > 2)
  {
    double     area   = 0.0;
    VertexType origin = it.Value();
    ++it;
    VertexType pt1 = it.Value();
    VertexType pt2 = it.Value();

    while (it != this->GetVertexList()->End())
    {
      pt1 = pt2;
      pt2 = it.Value();

      double vector1x      = pt1[0] - origin[0];
      double vector1y      = pt1[1] - origin[1];
      double vector2x      = pt2[0] - origin[0];
      double vector2y      = pt2[1] - origin[1];
      double crossProdduct = vector1x * vector2y - vector2x * vector1y;
      area += crossProdduct;
      ++it;
    }

    m_Area = fabs(area / 2.0);
  }
  else // if there is strictly less than 3 points, surface is 0
  {
    m_Area = 0.0;
  }

  m_AreaIsValid = true;
}

/**
 * Get surface
 */
template <class TValue>
double Polygon<TValue>::GetArea() const
{
  if (!m_AreaIsValid)
  {
    ComputeArea();
  }
  return m_Area;
}

/**
 * Length computation (difference with path is in the last addition)
 */
template <class TValue>
double Polygon<TValue>::GetLength() const
{
  double                      length = 0.0;
  VertexListConstIteratorType it     = this->GetVertexList()->Begin();

  VertexType origin = it.Value();
  if (this->GetVertexList()->Size() > 1)
  {

    VertexType pt1 = it.Value(); // just init, won't be used like that
    VertexType pt2 = it.Value();

    ++it;
    while (it != this->GetVertexList()->End())
    {
      pt1          = pt2;
      pt2          = it.Value();
      double accum = 0.0;
      for (int i = 0; i < 2; ++i)
      {
        accum += (pt1[i] - pt2[i]) * (pt1[i] - pt2[i]);
      }
      length += std::sqrt(accum);
      ++it;
    }

    // Adding the last segment (between first and last point)
    double accum = 0.0;
    for (int i = 0; i < 2; ++i)
    {
      accum += (origin[i] - pt2[i]) * (origin[i] - pt2[i]);
    }
    length += std::sqrt(accum);
  }
  else // if there is strictly less than 2 points, length is 0
  {
    length = 0.0;
  }

  return length;
}

template <class TValue>
void Polygon<TValue>::Modified() const
{
  Superclass::Modified();
  m_AreaIsValid = false;
}

/**
 * PrintSelf Method
 */
template <class TValue>
void Polygon<TValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
