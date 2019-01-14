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

#ifndef otbVectorDataToRightAngleVectorDataFilter_h
#define otbVectorDataToRightAngleVectorDataFilter_h


#include "otbVectorDataToVectorDataFilter.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMath.h"

namespace otb
{

/** \class VectorDataToRightAngleVectorDataFilter
 *  \brief Right angle detector
 *
 *  This filter aims at detecting right angle formed by the segments
 *  included in the input VectorData. It outputs a vector data
 *  containing right angle localizations symbolized by points.
 *
 *  The entire input vector data will be processed considering the two
 *  firsts vertex of each line features as a segment.
 *
 *
 *
 * \ingroup OTBCorner
 */

template <class TVectorData>
class ITK_EXPORT VectorDataToRightAngleVectorDataFilter
  : public otb::VectorDataToVectorDataFilter<TVectorData, TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToRightAngleVectorDataFilter  Self;
  typedef VectorDataToVectorDataFilter
    <TVectorData, TVectorData>                    Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToRightAngleVectorDataFilter, VectorDataToVectorDataFilter);

  /** Typedef support for ProcessObject*/
  typedef itk::ProcessObject ProcessObjectType;

  /** Typdedef support for the VectroData*/
  typedef TVectorData                            VectorDataType;
  typedef typename VectorDataType::DataNodeType  DataNodeType;
  typedef typename VectorDataType::LineType      LineType;
  typedef typename VectorDataType::PointType     PointType;
  typedef typename LineType::VertexType          VertexType;
  typedef typename LineType::VertexListType      VertexListType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType>
                                                 TreeIteratorType;

  /** Set/Get the thresholds*/
  itkGetMacro(DistanceThreshold, double);
  itkSetMacro(DistanceThreshold, double);

  itkGetMacro(AngleThreshold, double);
  itkSetMacro(AngleThreshold, double);

protected:
  /** Constructor.*/
  VectorDataToRightAngleVectorDataFilter();
  /**Destructor.*/
  ~VectorDataToRightAngleVectorDataFilter() override{}
  /** Standard PrintSelf method.*/
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**Main computation method.*/
  void  GenerateData() override;
  /**Angle computation.*/
  virtual double ComputeAngleFormedBySegments(LineType * lineDst, LineType * lineSrc);
  /** When we find a right angle, one compute the coordinate of the segments intersection.*/
  virtual PointType ComputeRightAngleCoordinate(LineType * lineDst, LineType * lineSrc);
  /**Compute the orientation of a segment*/
  virtual double ComputeOrientation(LineType * line);
  /**Distance From a point rAngle to a segment line.*/
  virtual double ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line);

private:
  VectorDataToRightAngleVectorDataFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  double m_DistanceThreshold;
  double m_AngleThreshold;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRightAngleVectorDataFilter.hxx"
#endif

#endif
