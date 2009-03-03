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
#ifndef __otbVectorDataExtractROI_txx
#define __otbVectorDataExtractROI_txx

#include "otbVectorDataExtractROI.h"
#include "itkPreOrderTreeIterator.h"

#include "otbGenericMapProjection.h"
#include "itkIdentityTransform.h"

#include "otbObjectList.h"
#include "otbMacro.h"

namespace otb
{

/**
 *
 */
template <class TVectorData>
VectorDataExtractROI<TVectorData>
::VectorDataExtractROI()
{
  m_ProjectionNeeded = false;
}

/**
 *
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

/**
 *
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::GenerateData(void)
{
  /** Get The input and the outptut*/
  typename VectorDataType::ConstPointer   input  = this->GetInput();
  typename VectorDataType::Pointer        output = this->GetOutput();

  /** put this here*/
  output->SetProjectionRef(input->GetProjectionRef());

  if(!input)
    std::cout << " Probleme avec la recuperation du input"<<std::endl;

  /** Need to check if it is necessary to project the roi*/
  this->CompareInputAndRegionProjection();

  
  /** If Projection of the region is needed, we project on the vectorData coordinate axis*/
  if(m_ProjectionNeeded)
    this->ProjectRegionToInputVectorProjection();
  
  
  /** Loop in the vectorData file
    * If There is a non null intersection between Bounding Boxes
    * and ROI, keep this feature.
    */
  /** Fill the document and the root of the vector data*/
  typename DataNodeType::Pointer                  document   = DataNodeType::New();
  typename DataNodeType::Pointer                  root       = input->GetDataTree()->GetRoot()->Get();
  typename VectorDataType::DataTreeType::Pointer  tree       = output->GetDataTree();
  
  DataNodePointerType                                         currentContainer;

  /** Walking trough the input vector data */
  typedef itk::PreOrderTreeIterator<DataTreeType>                 TreeIteratorType;
  TreeIteratorType                                                it(input->GetDataTree());
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
      DataNodePointerType  dataNode         =   it.Get();
      DataNodePointerType  newDataNode = DataNodeType::New();
      newDataNode->SetNodeType(dataNode->GetNodeType());
      newDataNode->SetNodeId(dataNode->GetNodeId());
      switch (dataNode->GetNodeType())
	{
	case ROOT:
	  {
	    tree->SetRoot(newDataNode);
	    currentContainer = newDataNode;
	    break;
	  }
	case DOCUMENT:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FOLDER:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FEATURE_POINT:
	  {
	    newDataNode->SetPoint(dataNode->GetPoint());
	    tree->Add(newDataNode,currentContainer);
	    break;
	  }
	case FEATURE_LINE:
	  {
	    if(this->IsLineIntersectionNotNull(dataNode->GetLine()))
	      {
		newDataNode->SetLine(dataNode->GetLine());
		tree->Add(newDataNode,currentContainer);
	      }
	    break;
	  }
	case FEATURE_POLYGON:
	  {
	    if(this->IsPolygonIntersectionNotNull(dataNode->GetPolygonExteriorRing()))
	      {
		newDataNode->SetPolygonExteriorRing(dataNode->GetPolygonExteriorRing());
		newDataNode->SetPolygonInteriorRings(dataNode->GetPolygonInteriorRings());
		tree->Add(newDataNode,currentContainer);
	      }
	    else
	      std::cout << " OUTSIDE The region" <<std::endl;
	    
	    break;
	  }
	case FEATURE_MULTIPOINT:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FEATURE_MULTILINE:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FEATURE_MULTIPOLYGON:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FEATURE_COLLECTION:
	  {
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	}
      
      ++it;
    }
  
}/*End GenerateData()*/

/**
 *
 */
template <class TVectorData>
bool
VectorDataExtractROI<TVectorData>
::IsPolygonIntersectionNotNull(PolygonPointerType polygon)
{
  RegionType region = ComputeVertexListBoudingRegion(polygon->GetVertexList());
  return region.Crop(m_GeoROI);
}

/**
 *
 */
template <class TVectorData>
bool
VectorDataExtractROI<TVectorData>
::IsLineIntersectionNotNull(LinePointerType line)
{
  RegionType region = ComputeVertexListBoudingRegion(line->GetVertexList());
  return region.Crop(m_GeoROI);
}

/**
 * CompareInputAndRegionProjection
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::CompareInputAndRegionProjection()
{
  /**Traces*/
  std::string regionProjection = m_ROI.GetRegionProjection();
  std::string inputVectorProjection = this->GetInput()->GetProjectionRef();
    
  if(regionProjection == inputVectorProjection)
    m_ProjectionNeeded = false;
  else
    m_ProjectionNeeded = true;
}

/**
 * CompareInputAndRegionProjection
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::ProjectRegionToInputVectorProjection()
{
  typedef otb::GenericMapProjection<otb::FORWARD>     ForwardMapProjectionType;
  ForwardMapProjectionType::Pointer mapTransform =    ForwardMapProjectionType::New();
  mapTransform->SetWkt(m_ROI.GetRegionProjection());
  
  /*FORWARD ; From RegionProjection To long/lat Projection */
  typename VertexListType::Pointer  regionCorners = VertexListType::New();
  ProjPointType                          point1, point2 , point3, point4;
  
  /** Compute the extremities of the region*/
  point1[0] = m_ROI.GetOrigin()[0];
  point1[1] = m_ROI.GetOrigin()[1];

  point2[0] = m_ROI.GetOrigin()[0]+ m_ROI.GetSize()[0];
  point2[1] = m_ROI.GetOrigin()[1];
  
  point3[0] = m_ROI.GetOrigin()[0]+ m_ROI.GetSize()[0];
  point3[1] = m_ROI.GetOrigin()[1]+ m_ROI.GetSize()[0];
  
  point4[0] = m_ROI.GetOrigin()[0];
  point4[1] = m_ROI.GetOrigin()[1]+ m_ROI.GetSize()[1];
  
  /** Aplly the transformation*/
  ProjPointType pGeo1 = mapTransform->TransformPoint(point1);
  ProjPointType pGeo2 = mapTransform->TransformPoint(point2);
  ProjPointType pGeo3 = mapTransform->TransformPoint(point3);
  ProjPointType pGeo4 = mapTransform->TransformPoint(point4);
  
  /** Inverse : From long/lat to InputVectorData projection*/
  typedef otb::GenericMapProjection<otb::INVERSE>     InverseMapProjectionType;
  InverseMapProjectionType::Pointer mapInverseTransform =    InverseMapProjectionType::New();
  if(this->GetInput()->GetProjectionRef().empty())
    {
      std::string inputProjectionRef = "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]";
      mapInverseTransform->SetWkt(inputProjectionRef);
    }
  else
    mapInverseTransform->SetWkt(this->GetInput()->GetProjectionRef());
  
  /** Finally  Project the four corners in the InputDataVector Projection*/
  ProjPointType pCarto1 = mapInverseTransform->TransformPoint(pGeo1);
  ProjPointType pCarto2 = mapInverseTransform->TransformPoint(pGeo2);
  ProjPointType pCarto3 = mapInverseTransform->TransformPoint(pGeo3);
  ProjPointType pCarto4 = mapInverseTransform->TransformPoint(pGeo4);
  
  /** Fill the vertex List : First Convert Point To*/
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(pCarto1));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(pCarto2));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(pCarto3));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(pCarto4));
  
  /** Due to The projection : the Projected ROI can be rotated */
  m_GeoROI = this->ComputeVertexListBoudingRegion(regionCorners.GetPointer());
    
}
/**
 * itk::Point to ContinuousIndex
 */
template <class TVectorData>
typename VectorDataExtractROI<TVectorData>
::VertexType
VectorDataExtractROI<TVectorData>
::PointToContinuousIndex(ProjPointType  point)
{
  VertexType vertex;
  
  vertex[0] = point[0];
  vertex[1] = point[1];
  
  return vertex;
}

/**
 * CompareInputAndRegionProjection
 */
template <class TVectorData>
typename VectorDataExtractROI<TVectorData>
::RegionType
VectorDataExtractROI<TVectorData>
::ComputeVertexListBoudingRegion(typename VertexListType::ConstPointer vertexlist)
{
  double x = 0.,y = 0.;
  IndexType       index;
  IndexType       maxId;
  SizeType        size;

  typename VertexListType::ConstIterator it = vertexlist->Begin();
  
  if (vertexlist->Size() > 0 )
    {
      x = static_cast<double>(it.Value()[0]);
      y = static_cast<double>(it.Value()[1]);
      index[0] = x;
      index[1] = y;

      ++it;
      while (it != vertexlist->End())
	{
	  x = static_cast<double>(it.Value()[0]);
	  y = static_cast<double>(it.Value()[1]);

	  // Index search
	  if ( x < index[0] )
	    {
	      index[0] = x;
	    }
	  if ( y < index[1] )
	    {
	      index[1] = y;
	    }
	  // Max Id search for size computation
	  if ( x > maxId[0] )
	    {
	      maxId[0] = x;
	    }
	  if ( y > maxId[1] )
	    {
	      maxId[1] = y;
	    }

	  ++it;
	}

      size[0] = maxId[0] - index[0];
      size[1] = maxId[1] - index[1];
   }
  
  RegionType                      region;
  region.SetSize(size);
  region.SetOrigin(index);
  
  return region;
}



} // end namespace otb

#endif
