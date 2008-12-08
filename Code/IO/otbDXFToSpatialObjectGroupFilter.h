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
#ifndef __otbDXFToSpatialObjectGroupFilter_h
#define __otbDXFToSpatialObjectGroupFilter_h

#include "dl_creationadapter.h"
#include "itkGroupSpatialObject.h"
#include "otbSpatialObjectSource.h"

#include "itkEllipseSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkSpatialObjectPoint.h"
#include "itkLandmarkSpatialObject.h"
#include "otbLineSpatialObject.h"
#include "itkLineSpatialObjectPoint.h"
#include "otbArcSpatialObject.h"


namespace otb
{
  /** \class DXFToSpatialObjectGroupFilter
   *  \brief
   *
   */
template <class TSpatialObject>
class ITK_EXPORT DXFToSpatialObjectGroupFilter
  : public DL_CreationAdapter, public SpatialObjectSource<TSpatialObject>
{
  public:
  /** Standard typedefs */
  typedef DXFToSpatialObjectGroupFilter                              Self;
  typedef SpatialObjectSource< TSpatialObject > Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectReader,SpatialObjectSource);

  /** Spatial objects typedefs */
  typedef TSpatialObject                            GroupSpatialObjectType;
  typedef typename GroupSpatialObjectType::Pointer  GroupPointer;
  typedef itk::SpatialObjectPoint<TSpatialObject::ObjectDimension>       PointType;
  typedef itk::LandmarkSpatialObject<TSpatialObject::ObjectDimension>    LandmarkType;
  typedef typename LandmarkType::Pointer            LandmarkPointer;
  typedef typename  LandmarkType::PointListType     LandmarkListType;
  typedef LineSpatialObject<TSpatialObject::ObjectDimension>             LineType;
  typedef typename LineType::Pointer                LinePointer;
  typedef itk::LineSpatialObjectPoint<TSpatialObject::ObjectDimension>   LinePointType;
  typedef typename LineType::PointListType          LineListType;
  typedef otb::ArcSpatialObject<TSpatialObject::ObjectDimension>         ArcType;
  typedef typename ArcType::Pointer                 ArcPointer;
  typedef itk::EllipseSpatialObject<TSpatialObject::ObjectDimension>     EllipseType;
  typedef typename EllipseType::Pointer             EllipsePointer;
  typedef std::vector< LinePointType >              PointListType;


  virtual void addLayer(const DL_LayerData& data);
  virtual void addPoint(const DL_PointData& data);
  virtual void addLine(const DL_LineData& data);
  virtual void add3DFace(const DL_3DFaceData& data);
  virtual void addArc(const DL_ArcData& data);
  virtual void addCircle(const DL_CircleData& data);
  virtual void addPolyline(const DL_PolylineData& data);
  virtual void addVertex(const DL_VertexData& data);
  virtual void SetLayer(const char* layer);

  /** Workaround while waiting for the SpatialObject bug to be fixed */
  virtual void Update(void);

  protected:
  /** Constructor */
  DXFToSpatialObjectGroupFilter();
  /** Destructor */
  virtual ~DXFToSpatialObjectGroupFilter() {}

  /** Main computation method */
  virtual void GenerateData(void);

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private:
  DXFToSpatialObjectGroupFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  int m_CurrentObjectType;
  LineListType m_PointList;
  LinePointType m_PremierPoint;
  int m_PolylineClosed; //(=1 if the polyline is closed,else =0 );
  const char* m_layer;
  bool layerspecified;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDXFToSpatialObjectGroupFilter.txx"
#endif

#endif

