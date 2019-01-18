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

#ifndef otbLineSpatialObjectListToRightAnglePointSetFilter_h
#define otbLineSpatialObjectListToRightAnglePointSetFilter_h

#include "itkPointSet.h"
#include "otbLineSpatialObjectListToPointSetFilter.h"
#include "otbMath.h"
#include "itkVariableSizeMatrix.h"

namespace otb
{

/** \class LineSpatialObjectListToRightAnglePointSetFilter
 * \brief TODO
 *
 * \ingroup OTBCorner
 */

template <class TImage, class TLinesList, class TPointSet>
class ITK_EXPORT LineSpatialObjectListToRightAnglePointSetFilter
  : public otb::LineSpatialObjectListToPointSetFilter <TLinesList, TPointSet>
{

public:

  /** Standard class typedefs. */
  typedef LineSpatialObjectListToRightAnglePointSetFilter               Self;
  typedef LineSpatialObjectListToPointSetFilter <TLinesList, TPointSet> Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineSpatialObjectListToRightAnglePointSetFilter, LineSpatialObjectListToPointSetFilter);

  /** Typedef support for ProcessObject*/
  typedef itk::ProcessObject ProcessObjectType;

  /** Template parameters typedefs*/
  typedef TLinesList                            InputLinesListType;
  typedef typename InputLinesListType::LineType LineType;
  typedef std::vector<LineType*>                VectorLineType;
  //typedef typename LineType::PointListType            InputPointListType;
  typedef typename InputLinesListType::const_iterator InputLinesListTypeIterator;

  /**  Typdedef for The input image  :
   *   (ONLY USED FOR THE LINE ITERATOR -> To check if the index is inside the region)
   */
  typedef TImage                             InputImageType;
  typedef typename InputImageType::IndexType InputIndexType;

  /** Typedef support for output PointSet*/
  typedef TPointSet                              OutputPointSetType;
  typedef typename OutputPointSetType::PointType PointType;
  typedef typename OutputPointSetType::Pointer   OutputPointSetPointerType;

  /** Typedef Support  for Checking couple of segments used*/
  typedef itk::VariableSizeMatrix<unsigned int> checkMatrixType;

  /**
   *  Public Methods :
   *       Get the Input Image & Get The input Image
   */
  virtual InputImageType* GetInputImage();

  virtual void SetInputImage(InputImageType *);

  /** Set/Get the thresholds*/
  itkGetMacro(ThresholdDistance, double);
  itkSetMacro(ThresholdDistance, double);

  itkGetMacro(ThresholdAngle, double);
  itkSetMacro(ThresholdAngle, double);

protected:

  /**
   * Without the GenerateOutputInformation below it does not work
   * Because the ProcessObject class do a static_cast to allow memory and copy
   * the output (here a pointset). It works fine with images but with pointsets
   * no size is specified ...
   * the aim of the GenerateOutputInformation below is to avoid the guilty static_cast
   *
   */

  void GenerateOutputInformation() override{}

  /**
   * Constructor.
   */
  LineSpatialObjectListToRightAnglePointSetFilter();
  /**
   * Destructor.
   */
  ~LineSpatialObjectListToRightAnglePointSetFilter() override{}
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Main computation method.
   */
  void  GenerateData() override;
  /**
   * Angle computation
   */
  virtual double ComputeAngleFormedBySegments(LineType * lineDst, LineType * lineSrc);
  /**
   *  When we find a right angle, one compute the coordinate of the segments intersection
   */
  virtual PointType ComputeAngleRightCoordinate(LineType * lineDst, LineType * lineSrc);
  /**
   * AddRightAngleToPointSet
   */
  virtual void AddRightAngleToPointSet(PointType rAngle, LineType * LineDst, LineType* LineCur);
  /**
   * Compute the orienation of a segment
   */
  virtual double ComputeOrientation(LineType* line);
  /**
   * Distance From a point rAngle to a segment line
   */
  virtual double ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line);

private:

  LineSpatialObjectListToRightAnglePointSetFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Smart pointer on the output PointSet*/
  OutputPointSetPointerType m_OutputPointSet;

  double m_ThresholdDistance;
  double m_ThresholdAngle;

};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.hxx"
#endif

#endif
