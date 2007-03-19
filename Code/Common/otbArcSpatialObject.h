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
#ifndef _otbArcSpatialObject_h
#define _otbArcSpatialObject_h

#include "itkSpatialObject.h"
#include "itkAffineTransform.h"
#include "itkFixedArray.h"

namespace otb
{
/** \class ArcSpatialObject
 *  \brief 
 *
 * \ingroup 
 * \ingroup
 */
template <unsigned int VDimension = 2>
class ITK_EXPORT ArcSpatialObject
  : public itk::SpatialObject<VDimension>
{
public:
  /** Standard typedefs */
  typedef ArcSpatialObject            Self;
  typedef itk::SpatialObject<VDimension> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ArcSpatialObject, SpatialObject);

  /** Get the number of Dimensions */
  itkStaticConstMacro(NumberOfDimension, unsigned int,VDimension);

  /** Superclass typedefs */
  typedef typename Superclass::PointType       PointType;
  typedef typename Superclass::TransformType   TransformType;
  typedef typename Superclass::BoundingBoxType BoundingBoxType;

  /** Additional typedefs */
  typedef itk::VectorContainer<unsigned long,PointType> PointContainerType;
  typedef itk::SmartPointer<PointContainerType>         PointContainerPointer;
  /** 
   *  Set all radii to the same radius value.  Each radius is
   *  half the length of one axis of the ellipse.  
   */
  itkSetMacro(Radius,double);
  itkGetConstReferenceMacro(Radius,double);
   /// Set the Start Angle
  itkSetMacro(Angle1,double);
  itkGetMacro(Angle1,double);
 
  //Set the End Angle
  itkSetMacro(Angle2,double);
  itkGetMacro(Angle2,double);
  /** 
   *  That's useful for fuzzy objects.
   * \return a degree of membership to the object. 
   */ 
  virtual bool ValueAt( const PointType & point, double & value, 
                        unsigned int depth=0,
                        char * name=NULL) const;
  /** 
   * \return true if the object provides a method to evaluate the value 
   * at the specified point, false otherwise.
   */
  virtual bool IsEvaluableAt( const PointType & point, 
                              unsigned int depth=0,
                              char * name=NULL) const;
  /** 
   * Test whether a point is inside or outside the object 
   */ 
  virtual bool IsInside( const PointType & point,
                         unsigned int depth,
                         char *) const;
  /** 
   *  Test whether a point is inside or outside the object 
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth 
   */ 
  virtual bool IsInside( const PointType & point) const;
  /** 
   * Get the boundaries of a specific object.  This function needs to
   * be called every time one of the object's components is
   * changed. 
   */ 
  virtual bool ComputeLocalBoundingBox() const;
  /** 
   * Copy the information from another SpatialObject 
   */
  void CopyInformation(const itk::DataObject *data);

protected:
  /** Constructor */
  ArcSpatialObject();
  /** Destructor */
  virtual ~ArcSpatialObject() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ArcSpatialObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_Radius;
  double m_Angle1;
  double m_Angle2;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbArcSpatialObject.txx"
#endif

#endif
