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
#ifndef __otbRectangle_h
#define __otbRectangle_h

#include "itkImageRegion.h"

#include "itkVectorContainer.h"

namespace otb
{
/** \class Rectangle
 *  \brief This class represent a Rectangle.
 *
 *  A rectangle is defined by the median of the width, an orientation and a width
 *
 */
template<class TValue = double>
class ITK_EXPORT Rectangle
  : public itk::Object
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
  typedef itk::ContinuousIndex<ValueType, 2>         ContinuousIndexType;
  typedef ContinuousIndexType                        VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef typename VertexListType::Pointer           VertexListPointerType;
  typedef typename VertexListType::ConstIterator     VertexListConstIteratorType;

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
   * Check wether point is strictly inside the rectangle.
   * \param point The point to check.
   * \return True if the point is inside the polygon.
   */
  bool IsInside(VertexType point) const;

  virtual void  AddVertex(const ContinuousIndexType& vertex);

  /** GetBounding region*/
  virtual RegionType GetBoundingRegion() const;

protected:
  /** Constructor */
  Rectangle()
    {
    m_VertexList = VertexListType::New();
    };

  /** Destructor */
  virtual ~Rectangle() {}

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** */
  virtual double ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const;

  /** */

private:
  Rectangle(const Self &);    //purposely not implemented
  void operator =(const Self&);    //purposely not implemented

  ValueType m_Orientation;
  ValueType m_Width;

  VertexListPointerType m_VertexList;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRectangle.txx"
#endif
#endif
