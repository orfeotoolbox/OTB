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

#ifndef otbPolygon_h
#define otbPolygon_h

#include "otbPolyLineParametricPathWithValue.h"

namespace otb
{
/** \class Polygon
 *  \brief This class represent a 2D polygon.
 *
 *  It derives from otb::PolyLineParametricPathWithValue. The polygon is
 * considered to be the closed path represented by the
 * PolyLineParametricPathWithValue.
 *
 *  It implements some useful methods to work with polygons, such as surface
 * computation, as well as method useful in the spatial reasoning context, such
 * as IsInside, IsOnEdge, IsTouching, IsCrossing.
 *
 * \sa otb::PolyLineParametricPathWithValue
 *
 * \ingroup OTBVectorDataBase
 */
template <class TValue = double>
class ITK_EXPORT Polygon : public PolyLineParametricPathWithValue<TValue, 2>
{
public:
  /** Standard typedefs */
  typedef Polygon Self;
  typedef PolyLineParametricPathWithValue<TValue, 2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef TValue                        ValueType;
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(Polygon, PolyLineParametricPathWithValue);

  /** Derived typedefs */
  typedef typename Superclass::VertexType                  VertexType;
  typedef typename Superclass::VertexListType              VertexListType;
  typedef typename Superclass::ContinuousIndexType         ContinuousIndexType;
  typedef typename Superclass::VertexListConstIteratorType VertexListConstIteratorType;

  itkSetMacro(Epsilon, double);
  itkGetMacro(Epsilon, double);

  /**
   * Check whether point is strictly inside the polygon.
   * \param point The point to check.
   * \return True if the point is inside the polygon.
   */
  bool IsInside(VertexType point) const;

  /**
   * Check whether point is strictly on the edge of the polygon.
   * \param point The point to check.
   * \return True if the point is on the edge of the polygon.
   */
  bool IsOnEdge(VertexType point) const;

  /**
   * Returns the number of crossings of the polygon with a given segment.
   * \param a First point of the segment,
   * \param b Second point of the segment,
   * \return the number of strict crossings of segment [ab] with the polygon.
   */
  unsigned int NbCrossing(VertexType a, VertexType b) const;

  /**
   * Returns the number of touchings without crossing of the polygon with a given segment.
   * \param a First point of the segment,
   * \param b Second point of the segment,
   * \return the number of touchings without crossing of segment [ab] with the polygon.
   */
  unsigned int NbTouching(VertexType a, VertexType b) const;

  /**
   * Check whether two segments [a1a2] and [b1b2] are strictly crossing.
   * \param a1 First point of the first segment,
   * \param a1 Second point of the first segment,
   * \param a1 First point of the second segment,
   * \param a1 Second point of the second segment.
   * \return True if the two segments are strictly crossing.
   */
  bool IsCrossing(VertexType a1, VertexType a2, VertexType b1, VertexType b2) const;

  /**
   * Check whether two segments[a1a2] and [b1b2] are touching without crossing.
   * \param a1 First point of the first segment,
   * \param a1 Second point of the first segment,
   * \param a1 First point of the second segment,
   * \param a1 Second point of the second segment.
   * \return True if the two segments are touching without crossing.
   */
  bool IsTouching(VertexType a1, VertexType a2, VertexType b1, VertexType b2) const;

  /**
  * Return the polygon area.
  * \return The area.
  */
  virtual double GetArea() const;

  /**
  * Return the polygon length (perimeter).
  * \return The length.
  */
  double GetLength() const override;

  void AddVertex(const ContinuousIndexType& vertex) override;

protected:
  /** Constructor */
  Polygon()
  {
    m_Epsilon     = 0.000001;
    m_Area        = -1.0;
    m_AreaIsValid = false;
  };

  /** Destructor */
  ~Polygon() override
  {
  }

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  virtual void ComputeArea() const;
  void         Modified() const override;

private:
  Polygon(const Self&) = delete;
  void operator=(const Self&) = delete;

  double         m_Epsilon;
  mutable double m_Area;
  mutable bool   m_AreaIsValid;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolygon.hxx"
#endif
#endif
