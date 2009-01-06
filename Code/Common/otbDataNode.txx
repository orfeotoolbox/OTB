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
template <class TPrecision, unsigned int VDimension>
DataNode<TPrecision,VDimension>
::DataNode()
{
  m_NodeType = ROOT;
  m_NodeId = "";
  m_Data.valid = false;
}

template <class TPrecision, unsigned int VDimension>
void
DataNode<TPrecision,VDimension>
::SetNodeType(NodeType type)
{
  m_NodeType = type;
  m_Data.valid = false;
}

template <class TPrecision, unsigned int VDimension>
void
DataNode<TPrecision,VDimension>
::SetPoint(PointType point)
{
  m_NodeType = FEATURE_POINT;
  m_Data.point = point;
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension>
  void DataNode<TPrecision,VDimension>
::SetLine(LineType* line)
{
  m_NodeType = FEATURE_LINE;
  m_Data.line = line;
  m_Data.valid = true;
}
template <class TPrecision, unsigned int VDimension>
 void
DataNode<TPrecision,VDimension>
::SetPolygonExteriorRing(PolygonType* polygon)
{
  m_NodeType = FEATURE_POLYGON;
  m_Data.exteriorRing = polygon;
  if(!m_Data.interiorRings)
    {
      m_Data.interiorRings = PolygonListType::New();
    }
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension>
 void
DataNode<TPrecision,VDimension>
  ::SetPolygonInteriorRings(PolygonListType* polygonList)
{
  m_NodeType = FEATURE_POLYGON;
  m_Data.interiorRings = polygonList;
  if(!m_Data.exteriorRing)
    {
      m_Data.exteriorRing = PolygonType::New();
    }
  m_Data.valid = true;
}

template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::PointType
DataNode<TPrecision,VDimension>
::GetPoint() const
{
  if(!IsPointFeature())
    {
      itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a point.");
    }
  if(!m_Data.valid)
     {
       itkGenericExceptionMacro(<<"Invalid point node.");
     }
   return m_Data.point;
}
template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::LinePointerType
DataNode<TPrecision,VDimension>
::GetLine() const
{
  if(!IsLineFeature())
    {
      itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a line.");
    }
  if(!m_Data.valid)
     {
       itkGenericExceptionMacro(<<"Invalid line node.");
     }
   return m_Data.line;
}

template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::PolygonPointerType
DataNode<TPrecision,VDimension>
::GetPolygonExteriorRing() const
{
   if(!IsPolygonFeature())
    {
      itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a polygon.");
    }
   if(!m_Data.valid || !m_Data.exteriorRing )
     {
       itkGenericExceptionMacro(<<"Invalid polygon node.");
     }
   return m_Data.exteriorRing;
}

template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::PolygonListPointerType
DataNode<TPrecision,VDimension>
::GetPolygonInteriorRings() const
{
   if(!IsPolygonFeature())
    {
      itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a polygon.");
    }
   if(!m_Data.valid || !m_Data.interiorRings)
     {
       itkGenericExceptionMacro(<<"Invalid polygon node.");
     }
   return m_Data.interiorRings;
}

template <class TPrecision, unsigned int VDimension>
void
DataNode<TPrecision,VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os<<indent<<this->GetNodeTypeAsString();
}


template <class TPrecision, unsigned int VDimension>
std::string
DataNode<TPrecision,VDimension>
::GetNodeTypeAsString() const
{
  itk::OStringStream oss;
  switch(m_NodeType)
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

template <class TPrecision, unsigned int VDimension>
void
DataNode<TPrecision,VDimension>
::SetField(std::string key, std::string value)
{
  m_FieldMap[key] = value;
}
template <class TPrecision, unsigned int VDimension>
  std::string
DataNode<TPrecision,VDimension>
::GetField(std::string key) const
{
  if(HasField(key))
    {
      return (*m_FieldMap.find(key)).second;
    }
  else
    {
      return "Unknown Key";
    }
}
template <class TPrecision, unsigned int VDimension>
  void
DataNode<TPrecision,VDimension>
::RemoveField(std::string key)
{
  m_FieldMap.erase(key);
}
template <class TPrecision, unsigned int VDimension>
bool
DataNode<TPrecision,VDimension>
::HasField(std::string key) const
{
  return (m_FieldMap.find(key)!=m_FieldMap.end());
}
template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::FieldType
DataNode<TPrecision,VDimension>
::GetNthField(unsigned int index) const
{
  if(index<GetNumberOfFields())
    {
      FieldMapType::iterator it = m_FieldMap.begin();

      for(unsigned int i = 0;i<index;++i)
	{
	  ++it;
	}
      return (*it);
    }
  return FieldType("No key","No value");
}
template <class TPrecision, unsigned int VDimension>
  unsigned int
DataNode<TPrecision,VDimension>
::GetNumberOfFields() const
{
  return m_FieldMap.size();
}
template <class TPrecision, unsigned int VDimension>
  void
DataNode<TPrecision,VDimension>
::ClearFields()
{
  m_FieldMap.clear();
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsDocument() const
{
  return m_NodeType == DOCUMENT;
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsRoot() const
{
  return m_NodeType == ROOT;
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsFolder() const
{
  return m_NodeType == FOLDER;
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsPointFeature() const
{
  return m_NodeType == FEATURE_POINT;
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsLineFeature() const
{
  return m_NodeType == FEATURE_LINE;
}
template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsPolygonFeature() const
{
  return m_NodeType == FEATURE_POLYGON;
}

template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsMultiPointFeature() const
{
  return m_NodeType == FEATURE_MULTIPOINT;
}

template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsMultiLineFeature() const
{
  return m_NodeType == FEATURE_MULTILINE;
}

template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsMultiPolygonFeature() const
{
  return m_NodeType = FEATURE_MULTIPOLYGON;
}

template <class TPrecision, unsigned int VDimension>
  bool
DataNode<TPrecision,VDimension>
::IsCollectionFeature() const
{
  return m_NodeType == FEATURE_COLLECTION;
}
} // end namespace otb

#endif
