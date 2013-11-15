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
#ifndef __otbVectorDataToRightAngleVectorDataFilter_h
#define __otbVectorDataToRightAngleVectorDataFilter_h


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
 *  The entire input vector data will be processed concidering the two
 *  firsts vertex of each line features as a segment.
 *
 *
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
  virtual ~VectorDataToRightAngleVectorDataFilter(){}
  /** Standard PrintSelf method.*/
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**Main computation method.*/
  virtual void  GenerateData();
  /**Angle computation.*/
  virtual double ComputeAngleFormedBySegments(LineType * lineDst, LineType * lineSrc);
  /** When we find a right angle, one compute the coordinate of the segments intersection.*/
  virtual PointType ComputeRightAngleCoordinate(LineType * lineDst, LineType * lineSrc);
  /**Compute the orientation of a segment*/
  virtual double ComputeOrientation(LineType * line);
  /**Distance From a point rAngle to a segment line.*/
  virtual double ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line);

private:
  VectorDataToRightAngleVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  double m_DistanceThreshold;
  double m_AngleThreshold;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRightAngleVectorDataFilter.txx"
#endif

#endif
