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

#include "otbGenericMapProjection.h"
#include "itkIdentityTransform.h"
#include "otbGenericRSTransform.h"

#include "otbObjectList.h"
#include "otbMacro.h"

#include "itkProgressReporter.h"
#include "itkTimeProbe.h"


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
  m_DEMDirectory = "";
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
  this->AllocateOutputs();
  typename VectorDataType::ConstPointer inputPtr = this->GetInput();
  typename VectorDataType::Pointer      outputPtr = this->GetOutput();

  
  // Find out the projection needed
  if(!inputPtr->GetProjectionRef().empty())
    outputPtr->SetProjectionRef(inputPtr->GetProjectionRef());

  /** Need to check if it is necessary to project the roi*/
  this->CompareInputAndRegionProjection();

  /** If Projection of the region is needed, we project on the vectorData coordinate axis*/
  if(m_ProjectionNeeded)
    {
    this->ProjectRegionToInputVectorProjection();
    }
  else
    {
    m_GeoROI = m_ROI;
    }

  otbMsgDevMacro( << "ROI: " << this->m_ROI);
  otbMsgDevMacro( << "GeoROI: " << this->m_GeoROI);

  // Retrieve the output tree
  typename VectorDataType::DataTreePointerType tree = outputPtr->GetDataTree();

  // Get the input tree root
  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(inputPtr->GetDataTree()->GetRoot());

  // Create the output tree root
  DataNodePointerType newDataNode = DataNodeType::New();
  newDataNode->SetNodeType(inputRoot->Get()->GetNodeType());
  newDataNode->SetNodeId(inputRoot->Get()->GetNodeId());
  typename InternalTreeNodeType::Pointer outputRoot = InternalTreeNodeType::New();
  outputRoot->Set(newDataNode);
  tree->SetRoot(outputRoot);

  m_Kept  = 0;

  // Start recursive processing
  itk::TimeProbe chrono;
  chrono.Start();
  ProcessNode(inputRoot,outputRoot);
  chrono.Stop();
  std::cout<<"VectorDataExtractROI: "<<m_Kept<<" Features processed in "<<chrono.GetMeanTime()<<" seconds."<<std::endl;
}/*End GenerateData()*/



template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::ProcessNode(InternalTreeNodeType * source, InternalTreeNodeType * destination)
{
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();
  // For each child
  for(typename ChildrenListType::iterator it = children.begin(); it!=children.end();++it)
    {
    typename InternalTreeNodeType::Pointer newContainer;

    DataNodePointerType dataNode = (*it)->Get();
    DataNodePointerType newDataNode   = DataNodeType::New();
    newDataNode->SetNodeType(dataNode->GetNodeType());
    newDataNode->SetNodeId(dataNode->GetNodeId());

    switch(dataNode->GetNodeType())
    {
    case ROOT:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it),newContainer);
      ++m_Kept;
      break;
    }
    case DOCUMENT:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    case FOLDER:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    case FEATURE_POINT:
    {
    if(m_GeoROI.IsInside(this->PointToContinuousIndex(dataNode->GetPoint())))
      {
      newDataNode->SetPoint(dataNode->GetPoint());
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      }
      break;
    }
    case FEATURE_LINE:
    {
    if(this->IsLineIntersectionNotNull(dataNode->GetLine()))
      {
      newDataNode->SetLine(dataNode->GetLine());
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      }
      break;
    }
    case FEATURE_POLYGON:
    {
    if(this->IsPolygonIntersectionNotNull(dataNode->GetPolygonExteriorRing()))
      {
      newDataNode->SetPolygonExteriorRing(dataNode->GetPolygonExteriorRing());
      newDataNode->SetPolygonInteriorRings(dataNode->GetPolygonInteriorRings());
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      }
      break;
    }
    case FEATURE_MULTIPOINT:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    case FEATURE_MULTILINE:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    case FEATURE_MULTIPOLYGON:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    case FEATURE_COLLECTION:
    {
      newContainer = InternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ++m_Kept;
      ProcessNode((*it),newContainer);
      break;
    }
    }
    }
}

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
  typedef otb::GenericRSTransform<>                            GenericRSTransformType;
  typename GenericRSTransformType::Pointer genericTransform =  GenericRSTransformType::New();

  /** We need get the transformation*/
  genericTransform->SetInputProjectionRef( m_ROI.GetRegionProjection());

  genericTransform->SetInputKeywordList(m_ROI.GetKeywordList());
  genericTransform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef() );
  const itk::MetaDataDictionary &inputDict = this->GetInput()->GetMetaDataDictionary();
  genericTransform->SetOutputDictionary(inputDict);

  genericTransform->SetDEMDirectory(m_DEMDirectory);

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
  m_GeoROI.SetRegionProjection(this->GetInput()->GetProjectionRef());
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
