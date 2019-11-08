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

#ifndef otbLandmark_h
#define otbLandmark_h

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
 *
 * \ingroup OTBDescriptors
 */
template <class TPoint, class TPointData, class TLandmarkData = TPointData>
class ITK_EXPORT Landmark : public itk::DataObject
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
  Landmark()
  {
  }
  /// Destructor
  ~Landmark() override
  {
  }
  /// PrintSelf method
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Landmark: P1= " << m_Point1 << " P2= " << m_Point2 << std::endl;
  }

private:
  Landmark(const Self&) = delete;
  void operator=(const Self&) = delete;

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
