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

  /** Returns a reference to the list of the Line points.*/
  PointListType& GetPoints(void);

  /** Set the list of line points. */
  void SetPoints(PointListType& newPoints);

  /** Return a point in the list given the index */
  const SpatialObjectPointType* GetPoint(unsigned long id) const ITK_OVERRIDE
  {
    return &(m_Points[id]);
  }

  /** Return a point in the list given the index */
  SpatialObjectPointType* GetPoint(unsigned long id) ITK_OVERRIDE
  {
    return &(m_Points[id]);
  }

  /** Return the number of points in the list */
  unsigned long GetNumberOfPoints(void) const ITK_OVERRIDE
  {
    return m_Points.size();
  }

  /** Returns true if the line is evaluable at the requested point,
   *  false otherwise. */
  bool IsEvaluableAt(const PointType& point,
                     unsigned int depth = 0, char * name = ITK_NULLPTR) const ITK_OVERRIDE;

  /** Returns the value of the line at that point.
   * Currently this function returns a binary value,
   * but it might want to return a degree of membership
   * in case of fuzzy Lines. */
  bool ValueAt(const PointType& point, double& value,
               unsigned int depth = 0, char * name = ITK_NULLPTR) const ITK_OVERRIDE;

  /** Returns true if the point is inside the line, false otherwise. */
  bool IsInside(const PointType& point,
                unsigned int depth, char * name) const ITK_OVERRIDE;

  /** Test whether a point is inside or outside the object
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */
  virtual bool IsInside(const PointType& point) const;

  /** Compute the boundaries of the line.*/
  bool ComputeLocalBoundingBox() const ITK_OVERRIDE;

protected:
  /** Constructor */
  LineSpatialObject();
  /** Destructor */
  ~LineSpatialObject() ITK_OVERRIDE;
  /** Method to print the object. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  LineSpatialObject(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Point list */
  PointListType m_Points;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSpatialObject.txx"
#endif

#endif
