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
#ifndef __otbVectorDataExtractROI_h
#define __otbVectorDataExtractROI_h

#include "otbVectorDataToVectorDataFilter.h"
#include "otbRemoteSensingRegion.h"
#include "otbDEMHandler.h"
#include "itkMacro.h"

namespace otb
{

/** \class VectorDataExtractROI
 * \brief Extract a subset of a Vector Data based on location
 *
 * The region to extract is specified by a otb::RemoteSensingRegion
 * object. This region contains its own projection reference that may be
 * different from the projection of the vector data.
 *
 * The projection of the vector data will not be modified by this filter
 * if you need to change the projection, refer to otb::VectorDataProjectionFilter
 *
 * \note Parameter to this class for input and outputs are vectorData
 *
 * \sa RemoteSensingRegion
 * \sa VectorDataProjectionFilter
 *
 * \ingroup VectorDataFilter
 *
 * \example Projections/VectorDataExtractROIExample.cxx
 */
template <class TVectorData>
class ITK_EXPORT VectorDataExtractROI :
  public VectorDataToVectorDataFilter<TVectorData, TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataExtractROI                                   Self;
  typedef VectorDataToVectorDataFilter<TVectorData, TVectorData> Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataExtractROI, VectorDataToVectorDataFilter);

  /** Image type information*/
  typedef TVectorData                               VectorDataType;
  typedef typename VectorDataType::DataNodeType     DataNodeType;
  typedef typename DataNodeType::Pointer            DataNodePointerType;
  typedef typename VectorDataType::DataTreeType     DataTreeType;
  typedef typename DataNodeType::PolygonPointerType PolygonPointerType;
  typedef typename DataNodeType::PolygonType        PolygonType;
  typedef typename DataNodeType::LineType           LineType;
  typedef typename DataNodeType::LinePointerType    LinePointerType;
  typedef typename DataNodeType::PointType          PointType;

  /** Need Vertex and Vertex List Type*/
  typedef typename PolygonType::VertexType        VertexType;
  typedef typename PolygonType::VertexListType    VertexListType;
  typedef typename PolygonType::VertexListPointer VertexListPointer;

  /** TODO : automatize the dimension of the region*/
  typedef otb::RemoteSensingRegion<typename VertexType::CoordRepType> RegionType;
  typedef typename  RegionType::IndexType                             IndexType;
  typedef typename  RegionType::SizeType                              SizeType;

  typedef itk::Point<typename VertexType::CoordRepType, IndexType::IndexDimension> ProjPointType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> InputTreeIteratorType;
  typedef typename VectorDataType::DataTreeType::TreeNodeType              InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType                  ChildrenListType;

  /** Method to Set/Get the Region of intereset*/
  void SetRegion(const RegionType&  region)
  {
    m_ROI = region;
  }

  const RegionType& GetRegion()
  {return m_ROI; }

protected:
  VectorDataExtractROI();
  virtual ~VectorDataExtractROI() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Method to compare the projection embedded in the cartoRegion And the the InputVectorData*/
  virtual void CompareInputAndRegionProjection();

  /** Method to project from carto reference to Geo axis*/
  virtual void ProjectRegionToInputVectorProjection();

  /** Improve the Projected roi*/
  virtual RegionType ComputeVertexListBoundingRegion(typename VertexListType::ConstPointer vertexlist);

  /** Method to transform itk::Point to itk::ContinuousIndex*/
  virtual VertexType  PointToContinuousIndex(ProjPointType point);

  /** Prototype of the generate data method*/
  virtual void GenerateData(void);

  /** Method to check if the polygon Bounding Box ha ve a non-null intersection with the ROI*/
  virtual bool IsPolygonIntersectionNotNull(PolygonPointerType polygon);

  /** Method to check if the line Bounding Box ha ve a non-null intersection with the ROI*/
  virtual bool IsLineIntersectionNotNull(LinePointerType line);

  virtual void ProcessNode(InternalTreeNodeType * source, InternalTreeNodeType * destination);

private:
  VectorDataExtractROI(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int CounterClockWise(PointType firstPoint, PointType secondPoint, PointType thirdPoint);
  bool IsSegementIntersectSegment(LinePointerType segmentLineAB, LinePointerType segmentLineCD);

  bool        m_ProjectionNeeded;
  RegionType  m_ROI;
  RegionType  m_GeoROI;

  unsigned int m_Kept;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataExtractROI.txx"
#endif

#endif
