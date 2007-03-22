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
#ifndef __otbDXFToSpatialObjectGroupFilter_txx
#define __otbDXFToSpatialObjectGroupFilter_txx

#include "otbDXFToSpatialObjectGroupFilter.h"

namespace otb
{
  /**
   * Constructor.
   */
  template <class TSpatialObject>
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::DXFToSpatialObjectGroupFilter()
  {
    m_CurrentObjectType=0;
    m_PointList.clear();
    m_layer="";
    layerspecified=false;
    this->SetNumberOfRequiredOutputs(0);
  }
  /**
   * Set the layer to read
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::SetLayer(const char* layer) {
    m_layer=layer;
    layerspecified=true;
  }
  /**
   * Sample implementation of the method which handles layers.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addLayer(const DL_LayerData& data) 
  {  
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp(attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=0;
    otbMsgDebugMacro(<<"LAYER: "<< data.name <<" flags: "<<data.flags);
    
  }
  /**
   * Sample implementation of the method which handles point entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addPoint(const DL_PointData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp( attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=1;
    LandmarkPointer landmark=LandmarkType::New();
    PointType point;
    LandmarkListType list;
    point.SetPosition(data.x, data.y, data.z );
    list.push_back(point);
    landmark->SetPoints(list);
    if(layerspecified)
      {
        if (!strcmp(attributes.getLayer().c_str(),m_layer))
	  {
	    grpPtr->AddSpatialObject(landmark);
	  }
      }
    else    
      grpPtr->AddSpatialObject(landmark);
    otbMsgDebugMacro(<<"POINT ("<<data.x<<", "<<data.y<<", "<<data.z<<") ( nb objects "<<grpPtr->GetNumberOfChildren()<<")");
    
  }
  /**
   * Sample implementation of the method which handles line entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addLine(const DL_LineData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp( attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=2;
    LinePointer line=LineType::New();
    LinePointType point1,point2;
    LineListType list,pointlist;
    pointlist.clear();
    point1.SetPosition(data.x1, data.y1, data.z1);
    list.push_back(point1);
    point2.SetPosition(data.x2, data.y2, data.z2);
    list.push_back(point2);
    line->SetPoints(list);
    if(layerspecified)
      {
        if (!strcmp(attributes.getLayer().c_str(),m_layer))
	  {
	    grpPtr->AddSpatialObject(line);
	  }
      }
    else    
      grpPtr->AddSpatialObject(line);
    otbMsgDebugMacro(<<"LINE "<<"("<<data.x1<<", "<<data.y1<<", "<<data.z1<<") ("
		     <<data.x2<<", "<<data.y2<<", "<<data.z2 <<grpPtr->GetNumberOfChildren()<<")");
    
  }
  /**
   * Sample implementation of the method which handles 3DFace entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::add3DFace(const DL_3DFaceData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp(attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=7;
    LinePointer line=LineType::New();
    LinePointType point1,point2,point3,point4;
    LineListType list,pointlist;
    pointlist.clear();
    point1.SetPosition(data.x1, data.y1, data.z1);
    list.push_back(point1);
    point2.SetPosition(data.x2, data.y2, data.z2);
    list.push_back(point2);
    point3.SetPosition(data.x3, data.y3, data.z3);
    list.push_back(point3);
    point4.SetPosition(data.x4, data.y4, data.z4);
    list.push_back(point4);
    list.push_back(point1);
    line->SetPoints(list);
    if(layerspecified)
      {
        if (!strcmp(attributes.getLayer().c_str(),m_layer))
	  {
	    grpPtr->AddSpatialObject(line);
	  }
      }
    else 
      grpPtr->AddSpatialObject(line);
    otbMsgDebugMacro(<<"3D Face "<<"("<<data.x1<<", "<<data.y1<<", "<<data.z1<<") ("
		     <<data.x2<<", "<<data.y2<<", "<<data.z2<<") ("
		     <<data.x3<<", "<<data.y3<<", "<<data.z3<<") ("
		     <<data.x4<<", "<<data.y4<<", "<<data.z4<<") ("
		     <<grpPtr->GetNumberOfChildren()<<")");
    
  }
  /**
   * Sample implementation of the method which handles arc entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addArc(const DL_ArcData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp(attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    otbMsgDebugMacro(<<"ARC ("<<data.cx<<", "<<data.cy<<", "<<data.cz<<") "
		     <<data.radius<<", "<<data.angle1<<", "<<data.angle2
		     <<" (nb objects "<<grpPtr->GetNumberOfChildren()<<")");
    
  }

  /**
   * Sample implementation of the method which handles circle entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addCircle(const DL_CircleData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp(attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=4;
    EllipsePointer ellipse=EllipseType::New();
    ellipse->SetRadius(data.radius);
    double offset[TSpatialObject::ObjectDimension];
    offset[ 0 ] = data.cx;
    offset[ 1 ] = data.cy;
    if (TSpatialObject::ObjectDimension==3)
      {
        offset[ 2 ] = data.cz;
      }
    ellipse->GetObjectToParentTransform()->SetOffset(offset);
    ellipse->ComputeObjectToWorldTransform();
    if(layerspecified)
      {
        if (!strcmp(attributes.getLayer().c_str(),m_layer))
	  {
	    grpPtr->AddSpatialObject(ellipse);
	  }
      }
    else    
      grpPtr->AddSpatialObject(ellipse);

    otbMsgDebugMacro(<<"CIRCLE ("<<data.cx<<", "<<data.cy<<", "<<data.cz<<") "
		     <<data.radius
		     <<" (nb objects "<<grpPtr->GetNumberOfChildren()<<")");
    
  }
  /**
   * Sample implementation of the method which handles polyline entities.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addPolyline(const DL_PolylineData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp( attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
    m_CurrentObjectType=5;

    m_PointList.clear();//reset list

    if ((int)data.flags==1)
      {
        m_PolylineClosed=1;
      }
    else 
      m_PolylineClosed=0;
    otbMsgDebugMacro(<<"POLYLINE ");
    otbMsgDebugMacro(<<"flags: "<< data.flags <<",nombre de sommets: "<< data.number);
    
  }


  /**
   * Sample implementation of the method which handles vertices.
   */
  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::addVertex(const DL_VertexData& data) 
  {
    GroupPointer grpPtr = this->GetOutput();
    m_CurrentObjectType=6;
    LinePointType point;
    point.SetPosition(data.x, data.y, data.z);
    m_PointList.push_back(point);
    m_PremierPoint=m_PointList[0];
    otbMsgDebugMacro(<<"VERTEX ("<<data.x<<", "<<data.y<<", "<<data.z<<") "
		     <<data.bulge
		     <<" (nb objects "<<grpPtr->GetNumberOfChildren()<<")");
    
  }

  template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::GenerateData() 
  {
    GroupPointer grpPtr = this->GetOutput();
    //If the previous object is a vertex we create the polyline:
    if (m_CurrentObjectType==6)
      {
	//If the polyline is closed then we add the first point at the end of the list:
	if (m_PolylineClosed==1)
	  {
	    m_PointList.push_back(m_PremierPoint);
	  }
	LinePointer m_CurrentPolyLine=LineType::New();
	m_CurrentPolyLine->SetPoints(m_PointList);
	if(layerspecified)
	  {
	    if (!strcmp( attributes.getLayer().c_str(),m_layer))
	      {
		grpPtr->AddSpatialObject(m_CurrentPolyLine);
	      }
	  }
	else 
	  grpPtr->AddSpatialObject(m_CurrentPolyLine);
      }
  }

template <class TSpatialObject>
  void
  DXFToSpatialObjectGroupFilter<TSpatialObject>
  ::Update() 
  {
    this->GenerateData();
  }

template <class TSpatialObject>
void
DXFToSpatialObjectGroupFilter<TSpatialObject>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} //namespace otb

#endif
