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
#include "otbGenericRSTransform.h"

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
  if(!input->GetProjectionRef().empty())
    output->SetProjectionRef(input->GetProjectionRef());

  if(!input)
    return;

  /** Need to check if it is necessary to project the roi*/
  this->CompareInputAndRegionProjection();

  
  /** If Projection of the region is needed, we project on the vectorData coordinate axis*/
  if(m_ProjectionNeeded)
    this->ProjectRegionToInputVectorProjection();
  else
    m_GeoROI = m_ROI;
  
  
  /** Loop in the vectorData file
    * If There is a non null intersection between Bounding Boxes
    * and ROI, keep this feature.
    */
  /** Fill the document and the root of the vector data*/
  typename DataNodeType::Pointer                  document   = DataNodeType::New();
  typename DataNodeType::Pointer                  root       = input->GetDataTree()->GetRoot()->Get();
  typename VectorDataType::DataTreeType::Pointer  tree       = output->GetDataTree();
  
  DataNodePointerType  currentContainer;
  DataNodePointerType  newDataNodeFolder                      = DataNodeType::New();
  DataNodePointerType  newDataNodeMultiPolygon                = DataNodeType::New();
  DataNodePointerType  newDataNodeMultiLine                   = DataNodeType::New();
  DataNodePointerType  newDataNodeMultiFeature                = DataNodeType::New();

  
  /** Walking trough the input vector data */
  typedef itk::PreOrderTreeIterator<DataTreeType>                 TreeIteratorType;
  TreeIteratorType                                                it(input->GetDataTree());
  it.GoToBegin();
  bool newFolder       = false ;
  bool newMultiFeature = false;
  
  

  while (!it.IsAtEnd())
    {
      DataNodePointerType  dataNode               =   it.Get();
      DataNodePointerType  newDataNode            = DataNodeType::New();


      switch (dataNode->GetNodeType())
	{
	case ROOT:
	  {
	    newDataNode->SetNodeType(dataNode->GetNodeType());
	    newDataNode->SetNodeId(dataNode->GetNodeId());
	    tree->SetRoot(newDataNode);
	    currentContainer = newDataNode;
	    break;
	  }
	case DOCUMENT:
	  {
	    newDataNode->SetNodeType(dataNode->GetNodeType());
	    newDataNode->SetNodeId(dataNode->GetNodeId());
	    tree->Add(newDataNode,currentContainer);
	    currentContainer = newDataNode;
	    break;
	  }
	case FOLDER:
	  {
	    newDataNodeFolder->SetNodeType(dataNode->GetNodeType());
	    newDataNodeFolder->SetNodeId(dataNode->GetNodeId());
	    newFolder = true;
	    break;
	  }
	case FEATURE_POINT:
	  {
	    if(m_GeoROI.IsInside(this->PointToContinuousIndex(dataNode->GetPoint())))
	      {
		if(newFolder)
		  {
		    tree->Add(newDataNodeFolder,currentContainer);
		    currentContainer = newDataNodeFolder;
		    newFolder = false;
		  }
		if(newMultiFeature)
		  {
		    tree->Add(newDataNodeMultiFeature,currentContainer);
		    currentContainer =  newDataNodeMultiFeature; 
		    newMultiFeature = false;
		  }
		newDataNode->SetNodeType(dataNode->GetNodeType());
		newDataNode->SetNodeId(dataNode->GetNodeId());
		newDataNode->SetPoint(dataNode->GetPoint());
		tree->Add(newDataNode,currentContainer);
	      }
	   
	    break;
	  }
	case FEATURE_LINE:
	  {
	    if(this->IsLineIntersectionNotNull(dataNode->GetLine()))
	      {
		if(newFolder)
		  {
		    tree->Add(newDataNodeFolder,currentContainer);
		    currentContainer = newDataNodeFolder;
		    newFolder = false;
		  }
		if(newMultiFeature)
		  {
		    tree->Add(newDataNodeMultiFeature ,currentContainer);
		    currentContainer =  newDataNodeMultiFeature ;
		    newMultiFeature = false;
		  }
		newDataNode->SetNodeType(dataNode->GetNodeType());
		newDataNode->SetNodeId(dataNode->GetNodeId());
		newDataNode->SetLine(dataNode->GetLine());
		tree->Add(newDataNode,currentContainer);
	      }
	    break;
	  }
	case FEATURE_POLYGON:
	  {
	    if(this->IsPolygonIntersectionNotNull(dataNode->GetPolygonExteriorRing()))
	      {
		if(newFolder)
		  {
		    tree->Add(newDataNodeFolder,currentContainer);
		    currentContainer = newDataNodeFolder;
		    newFolder = false;
		  }
		
		if(newMultiFeature)
		  {
		    tree->Add(newDataNodeMultiFeature,currentContainer);
		    currentContainer =  newDataNodeMultiFeature ;
		    newMultiFeature = false;
		  }
		
		newDataNode->SetNodeType(dataNode->GetNodeType());
		newDataNode->SetNodeId(dataNode->GetNodeId());
		newDataNode->SetPolygonExteriorRing(dataNode->GetPolygonExteriorRing());
		newDataNode->SetPolygonInteriorRings(dataNode->GetPolygonInteriorRings());
		tree->Add(newDataNode,currentContainer);
	      }
	    break;
	  }
	case FEATURE_MULTIPOINT:
	  {
	    newDataNodeMultiFeature->SetNodeType(dataNode->GetNodeType());
	    newDataNodeMultiFeature ->SetNodeId(dataNode->GetNodeId());
	    newMultiFeature = true;
	    

	    break;
	  }
	case FEATURE_MULTILINE:
	  {
	    newDataNodeMultiFeature ->SetNodeType(dataNode->GetNodeType());
	    newDataNodeMultiFeature ->SetNodeId(dataNode->GetNodeId());
	    newMultiFeature = true;
	    break;
	  }
	case FEATURE_MULTIPOLYGON:
	  {
	    newDataNodeMultiFeature  ->SetNodeType(dataNode->GetNodeType());
	    newDataNodeMultiFeature->SetNodeId(dataNode->GetNodeId());
	    newMultiFeature = true;
	    break;
	  }
	case FEATURE_COLLECTION:
	  {
	    newDataNode->SetNodeType(dataNode->GetNodeType());
	    newDataNode->SetNodeId(dataNode->GetNodeId());	    
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
  /* Use the RS Generic projection */
  typedef otb::GenericRSTransform<>                   GenericRSTransformType;
  typename GenericRSTransformType::Pointer genericTransform =  GenericRSTransformType::New();

  /** We need get the transformation*/
  const itk::MetaDataDictionary &inputDict = this->GetInput()->GetMetaDataDictionary();
  genericTransform->SetInputDictionary(inputDict);
  genericTransform->SetInputProjectionRef( m_ROI.GetRegionProjection());
  genericTransform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef() ); 
  genericTransform->InstanciateTransform();
  
  
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

  /** Fill the vertex List : First Convert Point To*/
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(genericTransform->TransformPoint(point1)));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(genericTransform->TransformPoint(point2)));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(genericTransform->TransformPoint(point3)));
  regionCorners->InsertElement(regionCorners->Size(),this->PointToContinuousIndex(genericTransform->TransformPoint(point4)));

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

  index.Fill(0.);
  maxId.Fill(0.);
  size.Fill(0.);

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
