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

#ifndef otbLineSpatialObject_h
#define otbLineSpatialObject_h

#include "itkPointBasedSpatialObject.h"
#include "itkLineSpatialObjectPoint.h"

namespace otb
{
/** \class LineSpatialObject
 *  \brief Representation of a Line based on the spatial object classes.
 *
 * The Line is basically defined by a set of points.
 *
 * \sa LineSpatialObjectPoint
 *
 * \ingroup OTBSpatialObjects
 */
template <unsigned int VDimension = 3>
class ITK_EXPORT LineSpatialObject
  : public itk::PointBasedSpatialObject<VDimension>
{
public:
  /** Standard typedefs */
  typedef LineSpatialObject                        Self;
  typedef itk::PointBasedSpatialObject<VDimension> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Method for creation through the object factory. */
  itkTypeMacro(LineSpatialObject, PointBasedSpatialObject);

  /** Superclass typedefs */
  typedef typename Superclass::SpatialObjectPointType SpatialObjectPointType;
  typedef typename Superclass::PointType              PointType;
  typedef typename Superclass::TransformType          TransformType;
  typedef typename Superclass::BoundingBoxType        BoundingBoxType;

  /** Additional typedefs */
  typedef double                                         ScalarType;
  typedef itk::LineSpatialObjectPoint<VDimension>        LinePointType;
  typedef std::vector<LinePointType>                     PointListType;
  typedef itk::VectorContainer<unsigned long, PointType> PointContainerType;
  typedef itk::SmartPointer<PointContainerType>          PointContainerPointer;
  typedef itk::SizeValueType SizeValueType;
  typedef itk::IdentifierType IdentifierType;

  /** Returns a reference to the list of the Line points.*/
  PointListType& GetPoints(void);

  /** Set the list of line points. */
  void SetPoints(PointListType& newPoints);

  /** Return a point in the list given the index */
  const SpatialObjectPointType* GetPoint(IdentifierType id) const override
  {
    return &(m_Points[id]);
  }

  /** Return a point in the list given the index */
  SpatialObjectPointType* GetPoint(IdentifierType id) override
  {
    return &(m_Points[id]);
  }

  /** Return the number of points in the list */
  SizeValueType GetNumberOfPoints(void) const override
  {
    return m_Points.size();
  }

  /** Returns true if the line is evaluable at the requested point,
   *  false otherwise. */
  bool IsEvaluableAt(const PointType& point,
                     unsigned int depth = 0, char * name = nullptr) const override;

  /** Returns the value of the line at that point.
   * Currently this function returns a binary value,
   * but it might want to return a degree of membership
   * in case of fuzzy Lines. */
  bool ValueAt(const PointType& point, double& value,
               unsigned int depth = 0, char * name = nullptr) const override;

  /** Returns true if the point is inside the line, false otherwise. */
  bool IsInside(const PointType& point,
                unsigned int depth, char * name) const override;

  /** Test whether a point is inside or outside the object
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */
  virtual bool IsInside(const PointType& point) const;

  /** Compute the boundaries of the line.*/
  bool ComputeLocalBoundingBox() const override;

protected:
  /** Constructor */
  LineSpatialObject();
  /** Destructor */
  ~LineSpatialObject() override;
  /** Method to print the object. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LineSpatialObject(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Point list */
  PointListType m_Points;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSpatialObject.hxx"
#endif

#endif
