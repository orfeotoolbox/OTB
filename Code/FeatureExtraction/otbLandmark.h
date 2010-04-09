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
#ifndef __otbLandmark_h
#define __otbLandmark_h

#include "itkDataObject.h"
#include "otbMacro.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class Landmark
 *  \brief This class represent point and point data binary matching.
 *
 * It is used to represent match between keypoint like SIFT keypoint for instance.
 *
 * The class TLandmarkData can be used to store any information on the matching.
 */
template <class TPoint, class TPointData, class TLandmarkData = TPointData>
class ITK_EXPORT Landmark
  : public itk::DataObject
{
public:
  /// standard class typedefs
  typedef Landmark                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /// Standard macros
  itkNewMacro(Self);
  itkTypeMacro(Landmark, DataObject);

  /// template typedefs
  typedef TPoint        PointType;
  typedef TPointData    PointDataType;
  typedef TLandmarkData LandmarkDataType;

  /// Accessors
  itkSetMacro(Point1, PointType);
  itkGetConstReferenceMacro(Point1, PointType);
  itkSetMacro(PointData1, PointDataType);
  itkGetConstReferenceMacro(PointData1, PointDataType);
  itkSetMacro(Point2, PointType);
  itkGetConstReferenceMacro(Point2, PointType);
  itkSetMacro(PointData2, PointDataType);
  itkGetConstReferenceMacro(PointData2, PointDataType);
  itkSetMacro(LandmarkData, LandmarkDataType);
  itkGetConstReferenceMacro(LandmarkData, LandmarkDataType);

protected:
  /// Constructor
  Landmark() {}
  /// Destructor
  virtual ~Landmark() {}
  /// PrintSelf method
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Landmark: P1= " << m_Point1 << " P2= " << m_Point2 << std::endl;
  }

private:
  Landmark(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /// First landmark point
  PointType m_Point1;
  /// Second landmark point
  PointType m_Point2;
  /// First landmark point data
  PointDataType m_PointData1;
  /// Second landmark point data
  PointDataType m_PointData2;
  /// Landmark data
  LandmarkDataType m_LandmarkData;
};
} // end namespace otb
#endif
