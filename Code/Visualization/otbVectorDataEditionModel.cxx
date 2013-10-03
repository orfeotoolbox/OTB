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
#include "otbVectorDataEditionModel.h"

#include "itkTimeProbe.h"


namespace otb
{

VectorDataEditionModel::VectorDataEditionModel() :
  m_SearchIndex(-1), m_Threshold(10)
{

}

void
VectorDataEditionModel::UpdatePoint( PointType srcPoint, PointType dstPoint)
{
  srcPoint[0] = this->GetOrigin()[0] + srcPoint[0] / this->GetSpacing()[0];
  srcPoint[1] = this->GetOrigin()[1] + srcPoint[1] / this->GetSpacing()[1];

  if ( m_SearchIndex != -1 )
    {
    // Set the geometry the srcPoint belongs to, as the SelectedGeometry
    this->SetSelectedGeometry(m_SearchIndex);

    // const_cast the vertexList to allow updating
    VertexListType* newVertexList;

    // Get the VertexList depending on the Geometry Type;
    switch (GetSelectedGeometry()->GetNodeType())
    {
      case FEATURE_LINE:
        {
        newVertexList = const_cast< VertexListType*>(GetSelectedGeometry()->GetLine()->GetVertexList());
        }
        break;
      case FEATURE_POLYGON:
        {
        newVertexList = const_cast< VertexListType*>(GetSelectedGeometry()->GetPolygonExteriorRing()->GetVertexList());
        }
        break;
      case FEATURE_POINT:
        {
        std::cout <<"WIP" << std::endl;
        }
        break;
      default:
        {
        itkExceptionMacro(<<"Type is not supported for vectorData Edition");
        }
    }

    // needed variables
    /* -Wunused-variable
    *  int size = newVertexList->Size();
    *  unsigned int vidx = 0;
    */
    bool found = false;

    // Try with VertexIterator
    VertexListType::Iterator itVertexList = newVertexList->Begin();

    while(itVertexList != newVertexList->End() && !found)
      {
      VertexType currentPoint= itVertexList.Value();
      double dist = currentPoint.EuclideanDistanceTo(srcPoint);

      if (dist < m_Threshold)
       {
       currentPoint[0] = dstPoint[0];
       currentPoint[1] = dstPoint[1];

       // Update the point
       itVertexList.Value() = currentPoint;

       found = true;
       }
      ++itVertexList;
      }
    }
}


void
VectorDataEditionModel::UpdateGeometryPosition(double tx, double ty)
{
  if (m_SearchIndex != -1)
    {
    itk::TimeProbe chrono;
    chrono.Start();

    // Set the current Geometry the geometry which the srcPoint
    // belongs to
    this->SetSelectedGeometry(m_SearchIndex);

    // const_cast the vertexList to allow updating
    VertexListType* newVertexList;

    // Get the VertexList depending on the Geometry Type;
    switch (GetSelectedGeometry()->GetNodeType())
      {
      case FEATURE_LINE:
      {
      newVertexList = const_cast< VertexListType*>(GetSelectedGeometry()->GetLine()->GetVertexList());

      this->ProcessVertexListTranslation(newVertexList, tx, ty);
      }
      break;
      case FEATURE_POLYGON:
      {
      newVertexList
        = const_cast< VertexListType*>(GetSelectedGeometry()->GetPolygonExteriorRing()->GetVertexList());

      this->ProcessVertexListTranslation(newVertexList, tx, ty);
      }
      break;
      case FEATURE_POINT:
      {
      std::cout <<"WIP" << std::endl;
      }
      break;
      default:
      {
      itkExceptionMacro(<<"Type is not supported for vectorData Edition");
      }
      }

    chrono.Stop();
    // std::cout<< "\tVectorDataEditionModel::UpdateGeometyPosition index "
    //          << m_SearchIndex  <<" with tx: " << tx << " ty: "<< ty
    //          <<" took " << chrono.GetMeanTime() << " seconds."
    //          <<std::endl;
    }
  else
    {
    std::cout <<"Geometry to displace was not found" << std::endl;
    }
}

void
VectorDataEditionModel::SearchDataNodeIndexFromPoint( PointType srcPoint)
{
  itk::TimeProbe chrono;
  chrono.Start();

  // Initialize the search index
  m_SearchIndex = -1;

  // Search wich geometry the src point belong to
  itk::PreOrderTreeIterator<VectorDataType::DataTreeType> it(this->GetVectorData()->GetDataTree());
  it.GoToBegin();
  int count = 0;

  while (!it.IsAtEnd() )
    {
    if (it.Get()->IsPointFeature() || it.Get()->IsLineFeature() || it.Get()->IsPolygonFeature())
      {
      double dist = it.Get()->EuclideanDistance(srcPoint);

      if (dist < m_Threshold )
        {
        m_SearchIndex = count;
        break;
        }
      count++;
      }
    ++it;
    }

  chrono.Stop();
  //std::cout<< "\tVectorDataEditionModel::SearchDataNodeIndexFromPoint "
  //<< " -> m_SearchIndex "<< m_SearchIndex <<" found in " << chrono.GetMeanTime() << " seconds."
  //<<std::endl;
}

void VectorDataEditionModel::ProcessVertexListTranslation(VertexListType* vertexList, double tx, double ty)
{
  VertexListType::Iterator it = vertexList->Begin();
  while ( it != vertexList->End())
    {
    // Get a reference to the current vertex
    VertexType currentPoint= it.Value();
    currentPoint[0] += tx;
    currentPoint[1] += ty;
    it.Value() = currentPoint;
    ++it;
    }
}

void VectorDataEditionModel::DeleteSelectedGeometry()
{
  // Set the selected geometry using the index found
  if (m_SearchIndex != -1)
    {
    // Set the current Geometry
    this->SetSelectedGeometry(m_SearchIndex);

    // Delete the geometry
    this->DeleteGeometry();
    }
}

void VectorDataEditionModel
::DeleteSelectedGeometriesInROI(itk::ImageRegion<2> rsRegion)
{
  itk::TimeProbe chrono;
  chrono.Start();

  // From RemoteSensingRegion To Datanode (polygon)
  DataNodeType::Pointer  regionNode = DataNodeType::New();
  PolygonType::Pointer regionPolygon = PolygonType::New();
  // Set the type of the datanode
  regionNode->SetNodeId("FEATURE_POLYGON");
  regionNode->SetNodeType(otb::FEATURE_POLYGON);
  regionNode->SetPolygonExteriorRing(regionPolygon);

  // Add the verticies to the polygon
  VertexType p;
  VertexType ul,ur, ll, lr;
  ul[0] = rsRegion.GetIndex()[0];
  ul[1] = rsRegion.GetIndex()[1];
  regionPolygon->AddVertex(ul);

  ur[0] = rsRegion.GetIndex()[0] + rsRegion.GetSize()[0];
  ur[1] = rsRegion.GetIndex()[1];
  regionPolygon->AddVertex(ur);

  ll[0] = rsRegion.GetIndex()[0] + rsRegion.GetSize()[0];
  ll[1] = rsRegion.GetIndex()[1] + rsRegion.GetSize()[1];
  regionPolygon->AddVertex(ll);

  lr[0] = rsRegion.GetIndex()[0];
  lr[1] = rsRegion.GetIndex()[1] + rsRegion.GetSize()[1];
  regionPolygon->AddVertex(lr);

  // Search the geometries within or intersecting the bounding region
  // and delete them
  itk::PreOrderTreeIterator<VectorDataType::DataTreeType> it(this->GetVectorData()->GetDataTree());
  it.GoToBegin();
  //int count = 0;
  //int nbGeomDeleted = 0;

  while (!it.IsAtEnd() )
    {
    if (it.Get()->IsPointFeature() || it.Get()->IsLineFeature() || it.Get()->IsPolygonFeature())
      {
      // check if the geometry intersects or is within the bounding box
      if ( it.Get()->Intersects(regionNode) || it.Get()->Within(regionNode))
        {
        // Remove the node
        it.Remove();

        // must go the to begin, because the iterator seems lost when
        // removing a node from its tree
        it.GoToBegin();

        //nbGeomDeleted++;
        }
      //count++;
      }
    ++it;
    }
  // chrono.Stop();
  // std::cout<< "\tVectorDataEditionModel::DeleteWithinROI -> "<<  nbGeomDeleted << " geometries deleted / "
  //          << count <<" in "  << chrono.GetMeanTime() << " seconds." <<std::endl;
}

}
