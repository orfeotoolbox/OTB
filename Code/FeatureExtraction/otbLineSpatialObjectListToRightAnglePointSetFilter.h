/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLineSpatialObjectListToRightAnglePointSetFilter_h
#define __otbLineSpatialObjectListToRightAnglePointSetFilter_h

#include "itkPointSet.h"
#include "otbLineSpatialObjectListToPointSetFilter.h"
#include "otbMath.h"
#include "itkVariableSizeMatrix.h"

namespace otb
{

/** \class LineSpatialObjectListToRightAnglePointSetFilter
 * \brief TODO
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
   * Without the GenrateOutputInformation below it does not work
   * Because the ProcessObject class do a static_cast to allow memory and copy
   * the output (here a pointset). It works fine with images but with pointsets
   * no size is specified ...
   * the aim of the GenerateOutputInformation below is to avoid the guilty static_cast
   *
   */

  virtual void GenerateOutputInformation(){}

  /**
   * Constructor.
   */
  LineSpatialObjectListToRightAnglePointSetFilter();
  /**
   * Destructor.
   */
  virtual ~LineSpatialObjectListToRightAnglePointSetFilter(){}
  /**
   * Standard PrintSelf method.
   */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * Main computation method.
   */
  virtual void  GenerateData();
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

  LineSpatialObjectListToRightAnglePointSetFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Smart pointer on the output PointSet*/
  OutputPointSetPointerType m_OutputPointSet;

  double m_ThresholdDistance;
  double m_ThresholdAngle;

};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.txx"
#endif

#endif
