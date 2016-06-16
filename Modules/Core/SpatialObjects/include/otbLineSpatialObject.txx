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
#ifndef otbLineSpatialObject_txx
#define otbLineSpatialObject_txx


#include "otbLineSpatialObject.h"
#include "otbMacro.h"

namespace otb
{
/** Constructor */
template<unsigned int VDimension>
LineSpatialObject<VDimension>
::LineSpatialObject()
{
  this->SetDimension(VDimension);
  this->SetTypeName("LineSpatialObject");
  this->GetProperty()->SetRed(1);
  this->GetProperty()->SetGreen(0);
  this->GetProperty()->SetBlue(0);
  this->GetProperty()->SetAlpha(1);
  this->ComputeBoundingBox();
}

/** Destructor */
template<unsigned int VDimension>
LineSpatialObject<VDimension>
::~LineSpatialObject()
{}

/** Returns a reference to the list of the Line points.*/
template<unsigned int VDimension>
typename LineSpatialObject<VDimension>::PointListType&
LineSpatialObject<VDimension>
::GetPoints()
{
  // otbMsgDevMacro( "Getting LinePoint list" );
  return m_Points;
}

/** Set the list of Line points. */
template<unsigned int VDimension>
void
LineSpatialObject<VDimension>
::SetPoints(PointListType& points)
{
  // in this function, passing a null pointer as argument will
  // just clear the list...
  m_Points.clear();

  typename PointListType::iterator it, end;
  it = points.begin();
  end = points.end();
  while (it != end)
    {
    m_Points.push_back(*it);
    ++it;
    }

  this->ComputeBoundingBox();
  this->Modified();
}

/** Print the object. */
template<unsigned int VDimension>
void
LineSpatialObject<VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "LineSpatialObject(" << this << ")" << std::endl;
  os << indent << "ID: " << this->GetId() << std::endl;
  os << indent << "nb of points: " << static_cast<unsigned long>(m_Points.size()) << std::endl;
  Superclass::PrintSelf(os, indent);
}

/** Compute the boundaries of the line.*/
template<unsigned int VDimension>
bool
LineSpatialObject<VDimension>
::ComputeLocalBoundingBox() const
{
  // tbMsgDevMacro( "Computing tube bounding box" );
  if (this->GetBoundingBoxChildrenName().empty()
      || strstr(typeid(Self).name(), this->GetBoundingBoxChildrenName().c_str()))
    {
    typename PointListType::const_iterator it  = m_Points.begin();
    typename PointListType::const_iterator end = m_Points.end();

    if (it == end)
      {
      return false;
      }
    else
      {
      PointType pt = this->GetIndexToWorldTransform()->TransformPoint((*it).GetPosition());
      PointType ptmin;
      PointType ptmax;
      ptmin[0] = pt[0] - 1;
      ptmin[1] = pt[1] - 1;
      ptmax[0] = pt[0] + 1;
      ptmax[1] = pt[1] + 1;
      const_cast<BoundingBoxType *>(this->GetBounds())->SetMinimum(ptmin);
      const_cast<BoundingBoxType *>(this->GetBounds())->SetMaximum(ptmax);
      ++it;
      while (it != end)
        {

        PointType pt2 = this->GetIndexToWorldTransform()->TransformPoint((*it).GetPosition());
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pt2);
        ++it;
        }

      }
    }

  return true;
}

/** Test whether a point is inside or outside the object
*  For computational speed purposes, it is faster if the method does not
*  check the name of the class and the current depth */
template<unsigned int VDimension>
bool
LineSpatialObject<VDimension>
::IsInside(const PointType& point) const
{
  typename PointListType::const_iterator it1 = m_Points.begin();
  typename PointListType::const_iterator end = m_Points.end();
  typename PointListType::const_iterator it2 = it1 + 1;
  if (!this->GetIndexToWorldTransform()->GetInverse(const_cast<TransformType *>(this->GetInternalInverseTransform())))
    {
    return false;
    }

  PointType transformedPoint = this->GetInternalInverseTransform()->TransformPoint(point);

  if (this->GetBounds()->IsInside(transformedPoint))
    {
    while (it2 != end)
      {
      if ((*it1).GetPosition()[0] < (*it2).GetPosition()[0])
        {
        if (transformedPoint[1] ==
            (int) ((*it1).GetPosition()[1] +
                   (((*it2).GetPosition()[1] -
                     (*it1).GetPosition()[1]) /
                    ((*it2).GetPosition()[0] -
                     (*it1).GetPosition()[0])) * (transformedPoint[0] - (*it1).GetPosition()[0])) &&
            transformedPoint[0] >= (*it1).GetPosition()[0] && transformedPoint[0] <= (*it2).GetPosition()[0])
          {
          return true;
          }
        }
      else
        {
        if (transformedPoint[1] ==
            (int) ((((*it2).GetPosition()[1] -
                     (*it1).GetPosition()[1]) /
                    ((*it1).GetPosition()[0] -
                     (*it2).GetPosition()[0])) *
                   ((*it1).GetPosition()[0] - transformedPoint[0]) + (*it1).GetPosition()[1]) &&
            transformedPoint[0] >= (*it2).GetPosition()[0] && transformedPoint[0] <= (*it1).GetPosition()[0])
          {
          return true;
          }
        }
      if ((*it1).GetPosition()[1] < (*it2).GetPosition()[1])
        {
        if (transformedPoint[0] ==
            (int) ((((*it2).GetPosition()[0] -
                     (*it1).GetPosition()[0]) /
                    ((*it2).GetPosition()[1] -
                     (*it1).GetPosition()[1])) *
                   (transformedPoint[1] - (*it1).GetPosition()[1]) + (*it1).GetPosition()[0]) &&
            transformedPoint[1] >= (*it1).GetPosition()[1] && transformedPoint[1] <= (*it2).GetPosition()[1])
          {
          return true;
          }
        }
      else
        {
        if (transformedPoint[0] ==
            (int) ((((*it2).GetPosition()[0] -
                     (*it1).GetPosition()[0]) /
                    ((*it1).GetPosition()[1] -
                     (*it2).GetPosition()[1])) *
                   ((*it1).GetPosition()[1] - transformedPoint[1]) + (*it1).GetPosition()[0]) &&
            transformedPoint[1] >= (*it2).GetPosition()[1] && transformedPoint[1] <= (*it1).GetPosition()[1])
          {
          return true;
          }
        }
      it1++;
      ++it2;
      }

    }
  return false;
}

/** Check if a given point is inside a line
*  return True only if the point is in the point list */
template<unsigned int VDimension>
bool
LineSpatialObject<VDimension>
::IsInside(const PointType& point, unsigned int depth, char * name) const
{
  // otbMsgDevMacro( "Checking the point [" << point << "] is on the Line" );

  if (name == ITK_NULLPTR)
    {
    if (IsInside(point))
      {
      return true;
      }
    }
  else if (strstr(typeid(Self).name(), name))
    {
    if (IsInside(point))
      {
      return true;
      }
    }
  return Superclass::IsInside(point, depth, name);
}

/** Returns true if the line is evaluable at the requested point,
*  false otherwise. */
template<unsigned int VDimension>
bool
LineSpatialObject<VDimension>
::IsEvaluableAt(const PointType& point, unsigned int depth, char * name) const
{
  // otbMsgDevMacro( "Checking if the tube is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Returns the value of the line at that point.
* Currently this function returns a binary value,
* but it might want to return a degree of membership
* in case of fuzzy Lines. */
template<unsigned int VDimension>
bool
LineSpatialObject<VDimension>
::ValueAt(const PointType& point, double& value, unsigned int depth,
          char * name) const
{
  // otbMsgDevMacro( "Getting the value of the tube at " << point );

  if (IsInside(point, 0, name))
    {
    value = this->GetDefaultInsideValue();
    return true;
    }
  else
    {
    if (Superclass::IsEvaluableAt(point, depth, name))
      {
      Superclass::ValueAt(point, value, depth, name);
      return true;
      }
    else
      {
      value = this->GetDefaultOutsideValue();
      return false;
      }
    }
  return false;
}
} // end namespace otb

#endif
