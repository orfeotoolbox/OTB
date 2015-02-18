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
#ifndef __otbRectangle_txx
#define __otbRectangle_txx

#include "otbRectangle.h"

namespace otb
{

/**
 * Check wether point is strictly inside the polygon.
 * \param point The point to check.
 * \return True if the point is inside the polygon.
 */

template<class TValue>
bool
Rectangle<TValue>
::IsInside(VertexType point) const
{
  /*Iterate through the inputVertexList*/
  if (m_VertexList->Size() < 2)
    itkGenericExceptionMacro(
      << "Rectangle needs  TWO vertex, up-to-date the start and the end of the segments with AdDVertex Method ");

  VertexListConstIteratorType it = m_VertexList->Begin();

  VertexType p1 = it.Value();
  ++it;
  VertexType p2 = it.Value();

  /** Compute Length of the rectangle*/
  double lengthSeg = vcl_sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]));

  /** Orthogonal segment containing the middle of the segment */
  VertexType middleP;
  middleP[0] = (p1[0] + p2[0]) / 2.;
  middleP[1] = (p1[1] + p2[1]) / 2.;

  VertexType corner;
  corner[0] = middleP[0] + (m_Width / 2) * vcl_sin(m_Orientation);
  corner[1] = middleP[1] - (m_Width / 2) * vcl_cos(m_Orientation);

  /** Compute the distance to the orthogonal median of the rectangle*/
  if (this->ComputeEuclideanDistanceMetricToSegment(p1, p2, point) - (m_Width / 2.) < 1e-10
      && this->ComputeEuclideanDistanceMetricToSegment(middleP, corner, point) - (lengthSeg / 2.) < 1e-10) return true;
  else return false;
}

/**
 * Method to compute the distance between a point and a segment
 */
template<class TValue>
double
Rectangle<TValue>
::ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const
{
  double Xq1 = q1[0];
  double Yq1 = q1[1];
  double Xq2 = q2[0];
  double Yq2 = q2[1];
  double xp = p[0];
  double yp = p[1];

  double SegmentLength = vcl_sqrt((Xq1 - Xq2) * (Xq1 - Xq2) + (Yq1 - Yq2) * (Yq1 - Yq2));
  double CrossProduct  =   Xq1 * Yq2 - Xq2 * Yq1;
  double Num   = vcl_abs(xp * (Yq1 - Yq2) + yp * (Xq2 - Xq1) + CrossProduct);

  /** distance from Point P to Segment Q1Q2*/
  return  (Num / SegmentLength);
}

/**
 * Method to compute the Bounding region of a rectangle
 */

template<class TValue>
typename Rectangle<TValue>::
RegionType
Rectangle<TValue>
::GetBoundingRegion() const
{
  VertexListConstIteratorType it = m_VertexList->Begin();

  VertexType p1 = it.Value();
  ++it;
  VertexType p2 = it.Value();

  /** Compute the four corners of the recatangle*/
  double dx        = vcl_cos(m_Orientation);
  double dy        = vcl_sin(m_Orientation);
  double halfWidth = m_Width / 2;

  VertexListPointerType cornersVertex = VertexListType::New();
  VertexType            tempCorner;

  tempCorner[0] =  p1[0] + dy * halfWidth;
  tempCorner[1] =  p1[1] - dx * halfWidth;
  cornersVertex->InsertElement(cornersVertex->Size(), tempCorner);

  tempCorner[0] =  p1[0] - dy * halfWidth;
  tempCorner[1] =  p1[1] + dx * halfWidth;
  cornersVertex->InsertElement(cornersVertex->Size(), tempCorner);

  tempCorner[0] =  p2[0] + dy * halfWidth;
  tempCorner[1] =  p2[1] - dx * halfWidth;
  cornersVertex->InsertElement(cornersVertex->Size(), tempCorner);

  tempCorner[0] =  p2[0] - dy * halfWidth;
  tempCorner[1] =  p2[1] + dx * halfWidth;
  cornersVertex->InsertElement(cornersVertex->Size(), tempCorner);

  /** Compute the bounding region*/
  RegionType                     region;
  typename RegionType::SizeType  size;
  typename RegionType::IndexType index;
  typename RegionType::IndexType maxId;

  size.Fill(0);
  index.Fill(0);
  maxId.Fill(0);

  VertexListConstIteratorType itCorners = cornersVertex->Begin();

  long int x = 0, y = 0;

  if (cornersVertex->Size() > 0)
    {
    x = static_cast<long int>(itCorners.Value()[0]);
    y = static_cast<long int>(itCorners.Value()[1]);
    index[0] = x;
    index[1] = y;

    ++itCorners;
    while (itCorners != cornersVertex->End())
      {
      x = static_cast<long int>(itCorners.Value()[0]);
      y = static_cast<long int>(itCorners.Value()[1]);

      // Index search
      if (x < index[0])
        {
        index[0] = x;
        }
      if (y < index[1])
        {
        index[1] = y;
        }
      // Max Id search for size computation
      if (x > maxId[0])
        {
        maxId[0] = x;
        }
      if (y > maxId[1])
        {
        maxId[1] = y;
        }

      ++itCorners;
      }

    size[0] = maxId[0] - index[0];
    size[1] = maxId[1] - index[1];
    }
  region.SetSize(size);
  region.SetIndex(index);
  return region;

}

/*
 * Method add vertex
 *
 */
template<class TValue>
void
Rectangle<TValue>
::AddVertex(const ContinuousIndexType& vertex)
{
  if (m_VertexList->Size() > 1)
    itkGenericExceptionMacro(
      << "Rectangle needs only TWO vertex, a width and an orientation ");

  m_VertexList->InsertElement(m_VertexList->Size(), vertex);
}

/**
 * PrintSelf Method
 */
template<class TValue>
void
Rectangle<TValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Verticies:  " << m_VertexList << std::endl;
}

} // End namespace otb

#endif
