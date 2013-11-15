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
#include "otbVectorDataModel.h"

namespace otb
{

VectorDataModel::VectorDataModel() :
  m_VectorData(), m_CurrentNodeType(FEATURE_POLYGON), m_CurrentRootNode(),
  m_CurrentGeometry(), m_Origin(), m_Spacing()    //, m_UpperModel()
{
  m_Origin.Fill(0.);
  m_Spacing.Fill(1.);

  // Initialize vectordata
  this->ResetVectorData();
}

void VectorDataModel::ResetVectorData(void)
{
  m_VectorData = VectorDataType::New();
  m_CurrentRootNode = DataNodeType::New();
  m_CurrentRootNode->SetNodeId("DOCUMENT");
  m_CurrentRootNode->SetNodeType(DOCUMENT);
  m_VectorData->GetDataTree()->Add(
    m_CurrentRootNode,
    m_VectorData->GetDataTree()->GetRoot()->Get());
}

void VectorDataModel::Update(void)
{
  this->NotifyAll();
}

void VectorDataModel::AddPointToGeometry(VertexType& vertex, bool callUpdate)
{
  VertexType newPoint;
  newPoint[0] = m_Origin[0] + vertex[0] / m_Spacing[0];
  newPoint[1] = m_Origin[1] + vertex[1] / m_Spacing[1];

  // Check if current node type changed, and if so, close current geometry first
  if (m_CurrentGeometry.IsNotNull() && m_CurrentNodeType != m_CurrentGeometry->GetNodeType())
    {
    this->EndGeometry();
    }

  if  (m_CurrentNodeType == FEATURE_POINT)
    {
    otbMsgDevMacro(<< "VectorDataModel::AddPointToGeometry: Creating and adding new point");
    m_CurrentGeometry = NULL;
    if (m_CurrentGeometry.IsNull())
      {
      m_CurrentGeometry = DataNodeType::New();
      m_CurrentGeometry->SetNodeId("FEATURE_POINT");
      m_CurrentGeometry->SetNodeType(FEATURE_POINT);
      m_CurrentGeometry->SetPoint(newPoint);
      m_VectorData->GetDataTree()->Add(m_CurrentGeometry, m_CurrentRootNode);
      this->Modified();
      }
    else
      {
      itkExceptionMacro(<< "No partially created geometry should be up when adding FEATURE_POINT");
      }
    }
  else if (m_CurrentNodeType == FEATURE_POLYGON)
    {
    if (m_CurrentGeometry.IsNull())
      {
      otbMsgDevMacro(<< "VectorDataModel::AddPointToGeometry: Creating new polygon");
      m_CurrentGeometry = DataNodeType::New();
      m_CurrentGeometry->SetNodeId("FEATURE_POLYGON");
      m_CurrentGeometry->SetNodeType(FEATURE_POLYGON);
      PolygonType::Pointer exteriorRing = PolygonType::New();
      m_CurrentGeometry->SetPolygonExteriorRing(exteriorRing);
      m_VectorData->GetDataTree()->Add(m_CurrentGeometry, m_CurrentRootNode);
      }
    otbMsgDevMacro(<< "VectorDataModel::AddPoint: Adding point " << newPoint);
    m_CurrentGeometry->GetPolygonExteriorRing()->AddVertex(newPoint);
    }
  else if (m_CurrentNodeType == FEATURE_LINE)
    {
    if (m_CurrentGeometry.IsNull())
      {
      otbMsgDevMacro(<< "VectorDataModel::AddPointToGeometry: Creating new line");
      m_CurrentGeometry = DataNodeType::New();
      m_CurrentGeometry->SetNodeId("FEATURE_LINE");
      m_CurrentGeometry->SetNodeType(FEATURE_LINE);
      LineType::Pointer line = LineType::New();
      m_CurrentGeometry->SetLine(line);
      m_VectorData->GetDataTree()->Add(m_CurrentGeometry, m_CurrentRootNode);
      }
    otbMsgDevMacro(<< "VectorDataModel::AddPoint: Adding point " << newPoint);
    m_CurrentGeometry->GetLine()->AddVertex(newPoint);
    }
  else
    {
    itkExceptionMacro(<< "Node type not (yet) supported: " << m_CurrentNodeType);
    }

  if(callUpdate == true)
    {
      this->Update();
    }
}

void VectorDataModel::EndGeometry(bool callUpdate)
{
  // Avoid multiple endings
  if (!m_CurrentGeometry)
    {
    return;
    }

  if  (m_CurrentGeometry->GetNodeType() == FEATURE_POINT)
    {
    otbMsgDevMacro(<< "VectorDataModel::EndGeometry: Point don't need the geometry to be ended");
    m_CurrentGeometry = NULL;
    }
  else if (m_CurrentGeometry->GetNodeType() == FEATURE_POLYGON)
    {
    if (m_CurrentGeometry->GetPolygonExteriorRing()->GetVertexList()->Size()
        <= 2)
      {
      itkExceptionMacro(<< "Polygon must have at least 3 points");
      }
    otbMsgDevMacro(<< "VectorDataModel::EndGeometry: Ending polygon and adding to vector data");
    VertexListConstIteratorType it = m_CurrentGeometry->GetPolygonExteriorRing()->GetVertexList()->Begin();
    VertexType firstVertex = it.Value();
    m_CurrentGeometry->GetPolygonExteriorRing()->AddVertex(firstVertex);
    m_CurrentGeometry = NULL;
    }
  else if (m_CurrentGeometry->GetNodeType() == FEATURE_LINE)
    {
    if (m_CurrentGeometry->GetLine()->GetVertexList()->Size()
        < 2)
      {
      itkExceptionMacro(<< "Line must have at least 2 points");
      }
    otbMsgDevMacro(<< "VectorDataModel::EndGeometry: Ending line and adding to vector data");

    m_CurrentGeometry = NULL;
    }
  else
    {
    itkExceptionMacro(<< "Node type not (yet) supported: " << m_CurrentNodeType);
    }
  this->Modified();
  if(callUpdate == true)
    {
      this->Update();
    }
}

void VectorDataModel::DeleteGeometry(void)
{
  //delete selected geometry if there is one
  if (m_SelectedGeometry.IsNotNull())
    {
    VectorDataType::DataTreeType::PreOrderIteratorType it(m_VectorData->GetDataTree(),
                                                          m_VectorData->GetDataTree()->GetRoot());
    it.GoToBegin();
    while ( !it.IsAtEnd()
            && it.GetNode()->Get() != m_SelectedGeometry )
      {
      ++it;
      }
    if (it.IsAtEnd())
      {
      itkExceptionMacro(<< "The selected geometry is not in the tree (and it should)");
      }
    it.Remove();
    m_SelectedGeometry = NULL;
    }
  else
    {
    //No selected geometry found, delete the last one
    otbMsgDevMacro(<< "VectorDataModel::DeleteGeometry: Removing last geometry");

    //FIXME in need of a cleaner interface for the vector data
    VectorDataType::DataTreeType::TreeNodeType
    * currentRootNode =
      const_cast<VectorDataType::DataTreeType::TreeNodeType*> (m_VectorData->GetDataTree()->GetNode(
                                                                 m_CurrentRootNode));

    if (currentRootNode->GetChildrenList().size() > 0)
      {
      otbMsgDevMacro(<< "Current number of elements (before deletion)" << currentRootNode->GetChildrenList().size());

      currentRootNode->GetChildrenList().pop_back();
      }
    // Clear current geometry as well
    m_CurrentGeometry = NULL;
    }
  this->Modified();
  this->Update();

}

VectorDataModel::DataNodeType::Pointer VectorDataModel::GetNthDataNode(int n)
{
  VectorDataType::DataTreeType::PreOrderIteratorType it(m_VectorData->GetDataTree(),
                                                        m_VectorData->GetDataTree()->GetRoot());
  it.GoToBegin();
  int i = 0;
  while (!it.IsAtEnd() &&
         !(
           ((it.GetNode())->Get()->IsPointFeature())
           || (it.GetNode())->Get()->IsLineFeature()
           ||  (it.GetNode())->Get()->IsPolygonFeature()
           ))
    {
    ++it;
    }
  while (i < n && !it.IsAtEnd())
    {
    if ((it.GetNode())->Get()->IsPointFeature() || (it.GetNode())->Get()->IsLineFeature() ||
        (it.GetNode())->Get()->IsPolygonFeature())
      {
      ++i;
      }
    ++it;
    }
  if (it.IsAtEnd())
    {
//    itkExceptionMacro(<<"VectorDataModel::GetNthDataNode: requesting for inaccessible node: " << n);
    otbMsgDevMacro(<< "VectorDataModel::GetNthDataNode: requesting for inaccessible node: " << n);
    return NULL;
    }
  return (it.GetNode())->Get();
}

void VectorDataModel::SetSelectedGeometry(int n)
{
  m_SelectedGeometry = GetNthDataNode(n);
}


void
VectorDataModel::AddVectorData( VectorDataPointer vData, bool update)
{
  this->EndGeometry(false);
  DataTreeType::Pointer tree = vData->GetDataTree();
  TreeNodeType * root = const_cast<TreeNodeType *>(tree->GetRoot());
  this->AddNode( root );
  if (update)
    {
    this->Update();
    }
}

void
VectorDataModel::AddNode( TreeNodeType * node )
{
  switch (node->Get()->GetNodeType())
    {
    case FEATURE_POINT:
      {
  m_CurrentNodeType = FEATURE_POINT;
  PointType point = node->Get()->GetPoint();
  VertexType vertex;
  vertex[0] = point[0];
  vertex[1] = point[1];
  this->AddPointToGeometry(vertex, false);

  //Add Other Fields
  CopyFields( node );

  this->EndGeometry(false);
  break;
      }
    case FEATURE_LINE:
      {
  m_CurrentNodeType = FEATURE_LINE;
  const LineType * line = node->Get()->GetLine();
  LineType::VertexListType::ConstIterator vIt = line->GetVertexList()->Begin();
  while (vIt != line->GetVertexList()->End())
    {
      PointType point = vIt.Value();
      VertexType vertex;
      vertex[0] = point[0];
      vertex[1] = point[1];
      this->AddPointToGeometry(vertex, false);
      vIt++;
    }
        //Add Other Fields
        CopyFields( node );

  this->EndGeometry(false);
  break;
      }
    case FEATURE_POLYGON:
      {
    m_CurrentNodeType = FEATURE_POLYGON;
        const PolygonType *     extRing = node->Get()->GetPolygonExteriorRing();
        PolygonType::VertexListType::ConstIterator vIt = extRing->GetVertexList()->Begin();

        while (vIt != extRing->GetVertexList()->End())
    {
      PointType point = vIt.Value();
      VertexType vertex;
      vertex[0] = point[0];
      vertex[1] = point[1];
      this->AddPointToGeometry(vertex, false);
      vIt++;
    }

        //Add Other Fields
        CopyFields( node );

  this->EndGeometry(false);
  break;
      }
    default:
      {
  // discard
  break;
      }
    }

  // Get the children list from the input node
  ChildrenListType children = node->GetChildrenList();

  // Render each child
  ChildrenListType::iterator it = children.begin();
  while ( it != children.end() )
    {
      this->AddNode(*it);
      ++it;
    }
}

void
VectorDataModel::CopyFields( TreeNodeType * node )
{
  std::vector< std::string > fieldList = node->Get()->GetFieldList();
  unsigned int i;
  for (i=0; i < fieldList.size(); ++i)
    {
    m_CurrentGeometry->SetFieldAsString(fieldList.at(i), node->Get()->GetFieldAsString(fieldList.at(i)));
    }
}

}
