/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRectangle_h
#define otbRectangle_h

#include "itkImageRegion.h"

#include "itkVectorContainer.h"

namespace otb
{
/** \class Rectangle
 *  \brief This class represent a Rectangle.
 *
 *  A rectangle is defined by the median of the width, an orientation and a width
 *
 *
 * \ingroup OTBCommon
 */
template <class TValue = double>
class ITK_EXPORT Rectangle : public itk::Object
{
public:
  /** Standard typedefs */
  typedef Rectangle                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TValue ValueType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(Rectangle, itk::Object);

  /** Derived typedefs */
  typedef itk::ContinuousIndex<ValueType, 2> ContinuousIndexType;
  typedef ContinuousIndexType VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::Pointer       VertexListPointerType;
  typedef typename VertexListType::ConstIterator VertexListConstIteratorType;

  /*       typedef typename Superclass::VertexType                    VertexType; */
  /*       typedef typename Superclass::VertexListType                VertexListType; */
  /*       typedef typename VertexListType::Pointer                   VertexListPointerType; */
  /*       typedef typename Superclass::ContinuousIndexType           ContinuousIndexType; */
  /*       typedef typename Superclass::VertexListConstIteratorType   VertexListConstIteratorType; */

  typedef itk::ImageRegion<2> RegionType;

  /**
   * Set/Get the parameters the a rectangle
   *  - width and orientation
   */

  itkGetMacro(Width, ValueType);
  itkSetMacro(Width, ValueType);

  itkSetMacro(Orientation, ValueType);
  itkGetMacro(Orientation, ValueType);

  /**
   * Check whether point is strictly inside the rectangle.
   * \param point The point to check.
   * \return True if the point is inside the polygon.
   */
  bool IsInside(VertexType point) const;

  virtual void AddVertex(const ContinuousIndexType& vertex);

  /** GetBounding region*/
  virtual RegionType GetBoundingRegion() const;

protected:
  /** Constructor */
  Rectangle()
  {
    m_VertexList = VertexListType::New();
  };

  /** Destructor */
  ~Rectangle() override
  {
  }

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** */
  virtual double ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const;

  /** */

private:
  Rectangle(const Self&) = delete;
  void operator=(const Self&) = delete;

  ValueType m_Orientation;
  ValueType m_Width;

  VertexListPointerType m_VertexList;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRectangle.hxx"
#endif
#endif
