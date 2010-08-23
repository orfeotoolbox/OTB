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
#ifndef __otbVectorDataModel_txx
#define __otbVectorDataModel_txx

#include "otbVectorDataModel.h"


namespace otb
{




template <typename TImage>
void
VectorDataModel::AddVectorData( VectorDataPointer vData, TImage * image  )
{
  this->EndGeometry();
  DataTreeType::Pointer tree = vData->GetDataTree();
  TreeNodeType * root = const_cast<TreeNodeType *>(tree->GetRoot());
  this->AddNode( root, image );
  this->Update();
}


template <typename TImage>
void
VectorDataModel::AddNode( TreeNodeType * node, TImage * image )
{
  // From VEctorDataGlComponent
  // Render the current node
  switch (node->Get()->GetNodeType())
    {
    case FEATURE_POINT:
      {
        m_CurrentNodeType = FEATURE_POINT;
        typename TImage::IndexType id;
 	const PointType point = node->Get()->GetPoint();
        image->TransformPhysicalPointToIndex(point, id);
	   
 	VertexType vertex;
	vertex[0] = id[0];
	vertex[1] = id[1];
	this->AddPointToGeometry(vertex, false);

	this->EndGeometry();
	break;
      }
    case FEATURE_LINE:
      {
	m_CurrentNodeType = FEATURE_LINE;
	const LineType * line = node->Get()->GetLine();
	LineType::VertexListType::ConstIterator vIt = line->GetVertexList()->Begin();
	
	while (vIt != line->GetVertexList()->End())
	  {
            typename TImage::IndexType id;
            const PointType point = vIt.Value();
            image->TransformPhysicalPointToIndex(point, id);
	   
 	    VertexType vertex;
	    vertex[0] = id[0];
	    vertex[1] = id[1];
	    this->AddPointToGeometry(vertex, false );
	  }
	this->EndGeometry();
	break;
      }
    case FEATURE_POLYGON:
      {
  	m_CurrentNodeType = FEATURE_POLYGON;
        const PolygonType *     extRing = node->Get()->GetPolygonExteriorRing();
        PolygonType::VertexListType::ConstIterator vIt = extRing->GetVertexList()->Begin();

        while (vIt != extRing->GetVertexList()->End())
	  {
	    typename TImage::IndexType id;
	    const PointType point = vIt.Value();
	    image->TransformPhysicalPointToIndex(point, id);
 
	    VertexType vertex;
	    vertex[0] = id[0];
	    vertex[1] = id[1];
	    this->AddPointToGeometry(vertex, false);
	    vIt++;
	  }
	this->EndGeometry();
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
      this->AddNode(*it, image);
      ++it;
    }
}

}

#endif
