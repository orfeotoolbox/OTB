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
#ifndef __otbDataNode_txx
#define __otbDataNode_txx

#include "otbDataNode.h"

namespace otb
{
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
DataNode<TPrecision,VDimension,TValuePrecision>
::DataNode()
{
  m_NodeType = ROOT;
  m_NodeId = "";
  m_Data.valid = false;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::SetNodeType(NodeType type)
{
  m_NodeType = type;
  m_Data.valid = false;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::SetPoint(PointType point)
{
  m_NodeType = FEATURE_POINT;
  m_Data.point = point;
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void DataNode<TPrecision,VDimension,TValuePrecision>
::SetLine(LineType* line)
{
  m_NodeType = FEATURE_LINE;
  m_Data.line = line;
  m_Data.valid = true;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::SetPolygonExteriorRing(PolygonType* polygon)
{
  m_NodeType = FEATURE_POLYGON;
  m_Data.exteriorRing = polygon;
  if (!m_Data.interiorRings)
  {
    m_Data.interiorRings = PolygonListType::New();
  }
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::SetPolygonInteriorRings(PolygonListType* polygonList)
{
  m_NodeType = FEATURE_POLYGON;
  m_Data.interiorRings = polygonList;
  if (!m_Data.exteriorRing)
  {
    m_Data.exteriorRing = PolygonType::New();
  }
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
typename DataNode<TPrecision,VDimension,TValuePrecision>
::PointType
DataNode<TPrecision,VDimension,TValuePrecision>
::GetPoint() const
{
  if (!IsPointFeature())
  {
    itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a point.");
  }
  if (!m_Data.valid)
  {
    itkGenericExceptionMacro(<<"Invalid point node.");
  }
  return m_Data.point;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
typename DataNode<TPrecision,VDimension,TValuePrecision>
::LinePointerType
DataNode<TPrecision,VDimension,TValuePrecision>
::GetLine() const
{
  if (!IsLineFeature())
  {
    itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a line.");
  }
  if (!m_Data.valid)
  {
    itkGenericExceptionMacro(<<"Invalid line node.");
  }
  return m_Data.line;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
typename DataNode<TPrecision,VDimension,TValuePrecision>
::PolygonPointerType
DataNode<TPrecision,VDimension,TValuePrecision>
::GetPolygonExteriorRing() const
{
  if (!IsPolygonFeature())
  {
    itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a polygon.");
  }
  if (!m_Data.valid || !m_Data.exteriorRing )
  {
    itkGenericExceptionMacro(<<"Invalid polygon node.");
  }
  return m_Data.exteriorRing;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
typename DataNode<TPrecision,VDimension,TValuePrecision>
::PolygonListPointerType
DataNode<TPrecision,VDimension,TValuePrecision>
::GetPolygonInteriorRings() const
{
  if (!IsPolygonFeature())
  {
    itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a polygon.");
  }
  if (!m_Data.valid || !m_Data.interiorRings)
  {
    itkGenericExceptionMacro(<<"Invalid polygon node.");
  }
  return m_Data.interiorRings;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os<<indent<<this->GetNodeTypeAsString();
}


template <class TPrecision, unsigned int VDimension, class TValuePrecision>
std::string
DataNode<TPrecision,VDimension,TValuePrecision>
::GetNodeTypeAsString() const
{
  itk::OStringStream oss;
  switch (m_NodeType)
  {
  case ROOT:
  {
    oss<<"Root ("<<m_NodeId<<")";
    break;
  }
  case DOCUMENT:
  {
    oss<<"Document ("<<m_NodeId<<")";
    break;
  }
  case FOLDER:
  {
    oss<<"Folder ("<<m_NodeId<<")";
    break;
  }
  case FEATURE_POINT:
  {
    oss<<"Point ("<<m_NodeId<<") "<<m_Data.point;
    break;
  }
  case FEATURE_LINE:
  {
    oss<<"Line ("<<m_NodeId<<") "<<m_Data.line->GetVertexList()->Size()<<" points";
    break;
  }
  case FEATURE_POLYGON:
  {
    oss<<"Polygon ("<<m_NodeId<<") "<<this->GetPolygonExteriorRing()->GetVertexList()->Size()<<" points, "<<this->GetPolygonInteriorRings()->Size()<<" interior rings";
    break;
  }
  case FEATURE_MULTIPOINT:
  {
    oss<<"MultiPoint ("<<m_NodeId<<")";
    break;
  }
  case FEATURE_MULTILINE:
  {
    oss<<"MultiLine ("<<m_NodeId<<")";
    break;
  }
  case FEATURE_MULTIPOLYGON:
  {
    oss<<"MultiPolygon ("<<m_NodeId<<")";
    break;
  }
  case FEATURE_COLLECTION:
  {
    oss<<"Collection ("<<m_NodeId<<")";
    break;
  }
  }
  return oss.str();
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::SetField(std::string key, std::string value)
{
  m_FieldMap[key] = value;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
std::string
DataNode<TPrecision,VDimension,TValuePrecision>
::GetField(std::string key) const
{
  if (HasField(key))
  {
    return (*m_FieldMap.find(key)).second;
  }
  else
  {
    return "Unknown Key";
  }
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::RemoveField(std::string key)
{
  m_FieldMap.erase(key);
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::HasField(std::string key) const
{
  return (m_FieldMap.find(key)!=m_FieldMap.end());
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
typename DataNode<TPrecision,VDimension,TValuePrecision>
::FieldType
DataNode<TPrecision,VDimension,TValuePrecision>
::GetNthField(unsigned int index) const
{
  if (index<GetNumberOfFields())
  {
    FieldMapType::iterator it = m_FieldMap.begin();

    for (unsigned int i = 0;i<index;++i)
    {
      ++it;
    }
    return (*it);
  }
  return FieldType("No key","No value");
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
unsigned int
DataNode<TPrecision,VDimension,TValuePrecision>
::GetNumberOfFields() const
{
  return m_FieldMap.size();
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
void
DataNode<TPrecision,VDimension,TValuePrecision>
::ClearFields()
{
  m_FieldMap.clear();
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsDocument() const
{
  return m_NodeType == DOCUMENT;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsRoot() const
{
  return m_NodeType == ROOT;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsFolder() const
{
  return m_NodeType == FOLDER;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsPointFeature() const
{
  return m_NodeType == FEATURE_POINT;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsLineFeature() const
{
  return m_NodeType == FEATURE_LINE;
}
template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsPolygonFeature() const
{
  return m_NodeType == FEATURE_POLYGON;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsMultiPointFeature() const
{
  return m_NodeType == FEATURE_MULTIPOINT;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsMultiLineFeature() const
{
  return m_NodeType == FEATURE_MULTILINE;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsMultiPolygonFeature() const
{
  return m_NodeType = FEATURE_MULTIPOLYGON;
}

template <class TPrecision, unsigned int VDimension, class TValuePrecision>
bool
DataNode<TPrecision,VDimension,TValuePrecision>
::IsCollectionFeature() const
{
  return m_NodeType == FEATURE_COLLECTION;
}
} // end namespace otb

#endif
