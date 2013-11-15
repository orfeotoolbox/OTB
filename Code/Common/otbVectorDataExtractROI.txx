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
::VectorDataExtractROI() :
  m_ProjectionNeeded(false),
  m_ROI(),
  m_Kept(0)
{
}

/**
 *
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
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
  if (!inputPtr->GetProjectionRef().empty()) outputPtr->SetProjectionRef(inputPtr->GetProjectionRef());

  /** Need to check if it is necessary to project the roi*/
  this->CompareInputAndRegionProjection();

  /** If Projection of the region is needed, we project on the vectorData coordinate axis*/
  if (m_ProjectionNeeded)
    {
    otbMsgDevMacro(<< "Reprojecting region in vector data projection");
    this->ProjectRegionToInputVectorProjection();
    }
  else
    {
    otbMsgDevMacro(<< "Region and vector data projection are similar");
    m_GeoROI = m_ROI;
    }

  otbMsgDevMacro(<< "ROI: " << this->m_ROI);
  otbMsgDevMacro(<< "GeoROI: " << this->m_GeoROI);

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
  ProcessNode(inputRoot, outputRoot);
  chrono.Stop();
  otbMsgDevMacro(
    << "VectorDataExtractROI: " << m_Kept << " Features processed in " << chrono.GetMean() << " seconds.");
} /*End GenerateData()*/

template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::ProcessNode(InternalTreeNodeType * source, InternalTreeNodeType * destination)
{
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();
  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    typename InternalTreeNodeType::Pointer newContainer;

    DataNodePointerType dataNode = (*it)->Get();
    DataNodePointerType newDataNode   = DataNodeType::New();
    newDataNode->SetNodeType(dataNode->GetNodeType());
    newDataNode->SetNodeId(dataNode->GetNodeId());
    newDataNode->SetMetaDataDictionary(dataNode->GetMetaDataDictionary());

    switch (dataNode->GetNodeType())
      {
      case ROOT:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ProcessNode((*it), newContainer);
        ++m_Kept;
        break;
        }
      case DOCUMENT:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
        break;
        }
      case FOLDER:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_POINT:
        {
        if (m_GeoROI.IsInside(this->PointToContinuousIndex(dataNode->GetPoint())))
          {
          newDataNode->SetPoint(dataNode->GetPoint());
          newContainer = InternalTreeNodeType::New();
          newContainer->Set(newDataNode);
          destination->AddChild(newContainer);
          ++m_Kept;
          }
        break;
        }
      case FEATURE_LINE:
        {
        if (this->IsLineIntersectionNotNull(dataNode->GetLine()))
          {
          newDataNode->SetLine(dataNode->GetLine());
          newContainer = InternalTreeNodeType::New();
          newContainer->Set(newDataNode);
          destination->AddChild(newContainer);
          ++m_Kept;
          }
        break;
        }
      case FEATURE_POLYGON:
        {
        if (this->IsPolygonIntersectionNotNull(dataNode->GetPolygonExteriorRing()))
          {
          newDataNode->SetPolygonExteriorRing(dataNode->GetPolygonExteriorRing());
          newDataNode->SetPolygonInteriorRings(dataNode->GetPolygonInteriorRings());
          newContainer = InternalTreeNodeType::New();
          newContainer->Set(newDataNode);
          destination->AddChild(newContainer);
          ++m_Kept;
          }
        break;
        }
      case FEATURE_MULTIPOINT:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_MULTILINE:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
        break;
        }
      case FEATURE_COLLECTION:
        {
        newContainer = InternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        ++m_Kept;
        ProcessNode((*it), newContainer);
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
  // Get the VertexList
  // -2 cause we don't want the last point
  // wich is the same as the first one (closed Ring)
  for (unsigned int i = 0; i<polygon->GetVertexList()->Size() - 2; ++i )
    {
    // Get the components of the polygon 2 by 2
    VertexType firstVertex;
    VertexType secondVertex;
    firstVertex = polygon->GetVertexList()->GetElement(i);
    secondVertex = polygon->GetVertexList()->GetElement(i+1);
    
    // Build a line with each two vertex
    typename LineType::Pointer  line =  LineType::New();
    line->AddVertex(firstVertex);
    line->AddVertex(secondVertex);

    if (this->IsLineIntersectionNotNull(line))
      return true;
    }
  return false;
}

/**
 *
 */
template <class TVectorData>
bool
VectorDataExtractROI<TVectorData>
::IsLineIntersectionNotNull(LinePointerType line)
{
  RegionType lineRegion(line->GetBoundingRegion());
  
  // if the line bounding box have a null
  // intersection with the geoROI
  // the line and the region do not intersect
  if (!lineRegion.Crop(m_GeoROI))
    {
    return false;
    }
  else
    {
    // Get the VertexList
    for (unsigned int i = 0; i<line->GetVertexList()->Size() -1; ++i )
      {
      // Get the components of the line 2 by 2
      VertexType firstVertex;
      VertexType secondVertex;
      firstVertex = line->GetVertexList()->GetElement(i);
      secondVertex = line->GetVertexList()->GetElement(i+1);
      
      //  -------------------
      // Case 1 : Check if one of the two points are in the region
      PointType firstPoint, secondPoint;
      firstPoint[0] = firstVertex[0];
      firstPoint[1] = firstVertex[1];

      secondPoint[0] = secondVertex[0];
      secondPoint[1] = secondVertex[1];
    
      if (m_GeoROI.IsInside(this->PointToContinuousIndex(firstPoint))
          || m_GeoROI.IsInside(this->PointToContinuousIndex(secondPoint)))
        {
        return true;
        }
      else
        {
        //  -------------------
        // Case 2 : If any of the point is in the region
        if (!m_GeoROI.IsInside(this->PointToContinuousIndex(firstPoint))
            && !m_GeoROI.IsInside(this->PointToContinuousIndex(secondPoint)))
          {
          // Build a line with each two vertex
          typename LineType::Pointer  tempLine =  LineType::New();
          tempLine->AddVertex(firstVertex);
          tempLine->AddVertex(secondVertex);
          
          // Check if the intersection is not null
          RegionType region(tempLine->GetBoundingRegion());
          if (region.Crop(m_GeoROI))
            return true;
          
          //  -------------------
          // TODO : check if the segment cut
          //        one of the region edges
          }
        }
      }
    }
  
  return false;
}


template <class TVectorData>
bool
VectorDataExtractROI<TVectorData>
::IsSegementIntersectSegment(LinePointerType segmentLineAB, LinePointerType segmentLineCD)
{

  PointType vertexA, vertexB, vertexC, vertexD;

  vertexA  = segmentLineAB->GetVertexList()->GetElement(0);
  vertexB = segmentLineAB->GetVertexList()->GetElement(1);
  vertexC  = segmentLineCD->GetVertexList()->GetElement(0);
  vertexD = segmentLineCD->GetVertexList()->GetElement(1);

  int CounterClockWiseValueWithC = CounterClockWise(vertexA, vertexB, vertexC);
  int CounterClockWiseValueWithD = CounterClockWise(vertexA, vertexB, vertexD);

  if(CounterClockWiseValueWithC == CounterClockWiseValueWithD)
    {
      return false;
    }
  int CounterClockWiseValueWithA = CounterClockWise(vertexC, vertexD, vertexA);
  int CounterClockWiseValueWithB = CounterClockWise(vertexC, vertexD, vertexB);

  if(CounterClockWiseValueWithA == CounterClockWiseValueWithB)
    {
      return false;
    }

  return true;

}

template <class TVectorData>
int
VectorDataExtractROI<TVectorData>
::CounterClockWise(PointType firstPoint, PointType secondPoint, PointType thirdPoint)
{
  PointType  SecondMinusFirstPoint;
  PointType  ThirdMinusFirstPoint;

  SecondMinusFirstPoint = secondPoint - firstPoint;
  ThirdMinusFirstPoint  = thirdPoint - firstPoint;

  double dX1dY2MinusdY1dX2;
  dX1dY2MinusdY1dX2 = static_cast<double>(  SecondMinusFirstPoint[0]*ThirdMinusFirstPoint[1]
                                          - SecondMinusFirstPoint[1]*ThirdMinusFirstPoint[0]);
  if( dX1dY2MinusdY1dX2 > 0.0)
    {
      return 1;
    }
  if( dX1dY2MinusdY1dX2 < 0.0)
    {
      return -1;
    }

  double dX1dX2;
  double dY1dY2;
  dX1dX2 = static_cast<double>(SecondMinusFirstPoint[0] * ThirdMinusFirstPoint[0]);
  dY1dY2 = static_cast<double>(SecondMinusFirstPoint[1] * ThirdMinusFirstPoint[1]);
  if( (dX1dX2 < 0.0) || (dY1dY2 < 0.0) )
    {
      return -1;
    }

  double dX1dX1, dX2dX2, dY1dY1, dY2dY2;
  dX1dX1 = static_cast<double>(SecondMinusFirstPoint[0] * SecondMinusFirstPoint[0]);
  dX2dX2 = static_cast<double>(ThirdMinusFirstPoint[0] * ThirdMinusFirstPoint[0]);
  dY1dY1 = static_cast<double>(SecondMinusFirstPoint[1] * SecondMinusFirstPoint[1]);
  dY2dY2 = static_cast<double>(ThirdMinusFirstPoint[1] * ThirdMinusFirstPoint[1]);

  if( (dX1dX1+dY1dY1) < (dX2dX2 + dY2dY2) )
    {
      return 1;
    }

  return 0;
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

  //FIXME: the string comparison is not sufficient to say that two
  //projections are different
  if (regionProjection == inputVectorProjection) m_ProjectionNeeded = false;
  else m_ProjectionNeeded = true;
}

/**
 * ProjectRegionToInputVectorProjection
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::ProjectRegionToInputVectorProjection()
{
  /* Use the RS Generic projection */
  typedef otb::GenericRSTransform<> GenericRSTransformType;
  typename GenericRSTransformType::Pointer genericTransform =  GenericRSTransformType::New();

  /** Set up the projection */
  genericTransform->SetInputProjectionRef(m_ROI.GetRegionProjection());
  genericTransform->SetInputKeywordList(m_ROI.GetKeywordList());
  genericTransform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef());
  const itk::MetaDataDictionary& inputDict = this->GetInput()->GetMetaDataDictionary();
  genericTransform->SetOutputDictionary(inputDict);
  genericTransform->SetOutputOrigin(this->GetInput()->GetOrigin());
  genericTransform->SetOutputSpacing(this->GetInput()->GetSpacing());
  genericTransform->InstanciateTransform();

  otbMsgDevMacro(<< genericTransform);

  typename VertexListType::Pointer regionCorners = VertexListType::New();
  ProjPointType                    point1, point2, point3, point4;

  /** Compute the extremities of the region*/
  point1[0] = m_ROI.GetOrigin()[0];
  point1[1] = m_ROI.GetOrigin()[1];

  point2[0] = m_ROI.GetOrigin()[0] + m_ROI.GetSize()[0];
  point2[1] = m_ROI.GetOrigin()[1];

  point3[0] = m_ROI.GetOrigin()[0] + m_ROI.GetSize()[0];
  point3[1] = m_ROI.GetOrigin()[1] + m_ROI.GetSize()[1];

  point4[0] = m_ROI.GetOrigin()[0];
  point4[1] = m_ROI.GetOrigin()[1] + m_ROI.GetSize()[1];

  /** Fill the vertex List : First Convert Point To*/
  regionCorners->InsertElement(regionCorners->Size(),
                               this->PointToContinuousIndex(genericTransform->TransformPoint(point1)));
  regionCorners->InsertElement(regionCorners->Size(),
                               this->PointToContinuousIndex(genericTransform->TransformPoint(point2)));
  regionCorners->InsertElement(regionCorners->Size(),
                               this->PointToContinuousIndex(genericTransform->TransformPoint(point3)));
  regionCorners->InsertElement(regionCorners->Size(),
                               this->PointToContinuousIndex(genericTransform->TransformPoint(point4)));

  /** Due to The projection : the Projected ROI can be rotated */
  m_GeoROI = this->ComputeVertexListBoundingRegion(regionCorners.GetPointer());
  m_GeoROI.SetRegionProjection(this->GetInput()->GetProjectionRef());
}
/**
 * itk::Point to ContinuousIndex
 */
template <class TVectorData>
typename VectorDataExtractROI<TVectorData>
::VertexType
VectorDataExtractROI<TVectorData>
::PointToContinuousIndex(ProjPointType point)
{

  VertexType vertex;

  vertex[0] = point[0];
  vertex[1] = point[1];

  return vertex;
}

/**
 * ComputeVertexListBoundingRegion
 */
template <class TVectorData>
typename VectorDataExtractROI<TVectorData>
::RegionType
VectorDataExtractROI<TVectorData>
::ComputeVertexListBoundingRegion(typename VertexListType::ConstPointer vertexlist)
{
  double    x = 0., y = 0.;
  IndexType index;
  IndexType maxId;
  SizeType  size;

  index.Fill(0.);
  maxId.Fill(0.);
  size.Fill(0.);

  typename VertexListType::ConstIterator it = vertexlist->Begin();

  if (vertexlist->Size() > 0)
    {
    x = static_cast<double>(it.Value()[0]);
    y = static_cast<double>(it.Value()[1]);
    index[0] = x;
    index[1] = y;
    maxId[0] = x;
    maxId[1] = y;

    ++it;
    while (it != vertexlist->End())
      {
      x = static_cast<double>(it.Value()[0]);
      y = static_cast<double>(it.Value()[1]);

      // Index search
      if (x < index[0])
        {
        index[0] = x;
        }
      if (y > index[1])
        {
        index[1] = y;
        }
      // Max Id search for size computation
      if (x > maxId[0])
        {
        maxId[0] = x;
        }
      if (y < maxId[1])
        {
        maxId[1] = y;
        }

      ++it;
      }

    size[0] = maxId[0] - index[0];
    size[1] = maxId[1] - index[1];
    }

  RegionType region;
  region.SetSize(size);
  region.SetOrigin(index);

  return region;
}

} // end namespace otb

#endif
