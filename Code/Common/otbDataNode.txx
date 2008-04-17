#ifndef _otb_DataNode_txx
#define _otb_DataNode_txx

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
::SetLine(LinePointerType line)
{
  m_NodeType = FEATURE_LINE;
  m_Data.line = line;
  m_Data.valid = true;
}
template <class TPrecision, unsigned int VDimension> 
 void 
DataNode<TPrecision,VDimension>
::SetPolygon(PolygonPointerType polygon)
{
  m_NodeType = FEATURE_POLYGON;
  m_Data.polygon = polygon;
  m_Data.valid = true;
}
template <class TPrecision, unsigned int VDimension>  
typename DataNode<TPrecision,VDimension>
::PointType &        
DataNode<TPrecision,VDimension>
::GetPoint()
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
::GetLine()
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
::GetPolygon()
{
   if(!IsPolygonFeature())
    {
      itkGenericExceptionMacro(<<"Node "<<m_NodeId<<" is not a polygon.");
    }
   if(!m_Data.valid)
     {
       itkGenericExceptionMacro(<<"Invalid polygon node.");
     }
   return m_Data.line;
}
template <class TPrecision, unsigned int VDimension>  
void
DataNode<TPrecision,VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os<<indent;
  switch(m_NodeType)
    {
    case ROOT:
      {
	os<<"Root";
	break;
      }
    case DOCUMENT:
      {
	os<<"Document";
	break;
      }
    case FOLDER:
      {
	os<<"Folder";
	break;
      }
    case FEATURE_POINT:
      {
	os<<"Point Feature";
	break;
      }
    case FEATURE_LINE:
      {
	os<<"Line Feature";
	break;
      }
    case FEATURE_POLYGON:
      {
	os<<"Polygon Feature";
	break;
      }
    }
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
::GetField(std::string key)
{
  if(HasField(key))
    {
      return m_FieldMap[key];
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
::HasField(std::string key)
{
  return (m_FieldMap.find(key)!=m_FieldMap.end());
}
template <class TPrecision, unsigned int VDimension>
typename DataNode<TPrecision,VDimension>
::FieldType
DataNode<TPrecision,VDimension>
::GetNthField(unsigned int index)
{
  if(index<GetNumberOfFields())
    {
      FieldMapType::iterator it = m_FieldMap.begin() + index;
      return (*it);
    }
  return FieldType("No key","No value");
}
template <class TPrecision, unsigned int VDimension>
  unsigned int 
DataNode<TPrecision,VDimension>
::GetNumberOfFields()
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
::IsDocument()
{
  return m_NodeType==DOCUMENT;
}
template <class TPrecision, unsigned int VDimension>
  bool 
DataNode<TPrecision,VDimension>
::IsRoot()
{
  return m_NodeType = ROOT;
}
template <class TPrecision, unsigned int VDimension>
  bool 
DataNode<TPrecision,VDimension>
::IsFolder()
{
  return m_NodeType == FOLDER;
}
template <class TPrecision, unsigned int VDimension>
  bool 
DataNode<TPrecision,VDimension>
::IsPointFeature()
{
  return m_NodeType == FEATURE_POINT;
}
template <class TPrecision, unsigned int VDimension>
  bool 
DataNode<TPrecision,VDimension>
::IsLineFeature()
{
  return m_NodeType == FEATURE_LINE;
}
template <class TPrecision, unsigned int VDimension>
  bool 
DataNode<TPrecision,VDimension>
::IsPolygonFeature()
{
  return m_NodeType = FEATURE_POLYGON;
}
} // end namespace otb



#endif
