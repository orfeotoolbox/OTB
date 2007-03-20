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
/* #include <iostream> */
/* #include <stdio.h> */
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
template <unsigned int VDimension = 3>
class ITK_EXPORT DXFToSpatialObjectGroupFilter 
  : public DL_CreationAdapter, public SpatialObjectSource< itk::GroupSpatialObject<VDimension> >
{
  public:
  /** Standard typedefs */
  typedef DXFToSpatialObjectGroupFilter                              Self;
  typedef SpatialObjectSource< itk::GroupSpatialObject<VDimension> > Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  	
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectReader,SpatialObjectSource);

  /** Spatial objects typedefs */
  typedef itk::GroupSpatialObject<VDimension>       GroupSpatialObjectType;
  typedef typename GroupSpatialObjectType::Pointer  GroupPointer;		
  typedef itk::SpatialObjectPoint<VDimension>       PointType;
  typedef itk::LandmarkSpatialObject<VDimension>    LandmarkType;
  typedef typename LandmarkType::Pointer            LandmarkPointer;
  typedef typename  LandmarkType::PointListType     LandmarkListType;
  typedef LineSpatialObject<VDimension>             LineType;
  typedef typename LineType::Pointer                LinePointer;
  typedef itk::LineSpatialObjectPoint<VDimension>   LinePointType;
  typedef typename LineType::PointListType          LineListType;
  typedef otb::ArcSpatialObject<VDimension>         ArcType;
  typedef typename ArcType::Pointer                 ArcPointer;
  typedef itk::EllipseSpatialObject<VDimension>     EllipseType;
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
  
   /** Main computation method */
  void GenerateData(void);

  protected:
  /** Constructor */
  DXFToSpatialObjectGroupFilter();
  /** Destructor */
  virtual ~DXFToSpatialObjectGroupFilter() {}  
 
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
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbDXFToSpatialObjectGroupFilter.txx"
#endif

#endif

