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
#ifndef __otbVectorDataTransformFilter_txx
#define __otbVectorDataTransformFilter_txx

#include "otbVectorDataTransformFilter.h"
#include "itkProgressReporter.h"
#include <itkContinuousIndex.h>
#include "itkTimeProbe.h"

namespace otb
{

/**
   * Constructor
 */
template <class TInputVectorData, class TOutputVectorData >
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::VectorDataTransformFilter()
{
  m_Transform = GenericTransformType::New();
}

/**
* Convert point
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PointType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ReprojectPoint(PointType pointCoord) const
{
  itk::Point<double, 2> point;
  point = m_Transform->TransformPoint(pointCoord);
  return point;
}


/**
 * Convert line
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::LinePointerType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ReprojectLine(LinePointerType line) const
{
  typedef typename LineType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename LineType::VertexListConstIteratorType VertexListConstIteratorType;
  VertexListConstPointerType  vertexList = line->GetVertexList();
  VertexListConstIteratorType it = vertexList->Begin();
  typename LineType::Pointer newLine = LineType::New();
  while ( it != vertexList->End())
    {
    itk::Point<double, 2> point;
    itk::ContinuousIndex<double, 2> index;
    typename LineType::VertexType pointCoord = it.Value();
    point = m_Transform->TransformPoint(pointCoord);
    index[0]=point[0];
    index[1]=point[1];
    if (!vnl_math_isnan(index[0]) &&  !vnl_math_isnan(index[1]))
      newLine->AddVertex(index);
    it++;
    }

  return newLine;
}

/**
 * Convert polygon
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PolygonPointerType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ReprojectPolygon(PolygonPointerType polygon) const
{
  typedef typename PolygonType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename PolygonType::VertexListConstIteratorType VertexListConstIteratorType;
  VertexListConstPointerType  vertexList = polygon->GetVertexList();
  VertexListConstIteratorType it = vertexList->Begin();
  typename PolygonType::Pointer newPolygon = PolygonType::New();
  while ( it != vertexList->End())
    {
    itk::Point<double, 2> point;
    itk::ContinuousIndex<double, 2> index;
    typename PolygonType::VertexType pointCoord = it.Value();
    point = m_Transform->TransformPoint(pointCoord);
    index[0]=point[0];
    index[1]=point[1];
    if( !vnl_math_isnan(index[0]) &&  !vnl_math_isnan(index[1]) )
      newPolygon->AddVertex(index);
    it++;
    }
  return newPolygon;
}

/**
* Convert polygon list
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PolygonListPointerType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ReprojectPolygonList(PolygonListPointerType polygonList) const
{

  PolygonListPointerType newPolygonList = PolygonListType::New();
  for (typename PolygonListType::ConstIterator it = polygonList->Begin();
       it != polygonList->End(); ++it)
    {
    newPolygonList->PushBack(this->ReprojectPolygon(it.Get()));
    }
  return newPolygonList;
}


/**
   * GenerateData Performs the coordinate convertion for each element in the tree
 */
template <class TInputVectorData, class TOutputVectorData >
void
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::GenerateData(void)
{
  Superclass::GenerateOutputInformation();
  this->AllocateOutputs();

  InputVectorDataPointer inputPtr = this->GetInput();
  OutputVectorDataPointer outputPtr = this->GetOutput();

  outputPtr->SetProjectionRef(inputPtr->GetProjectionRef());
  OutputDataTreePointerType tree = outputPtr->GetDataTree();

  // Get the input tree root
  InputInternalTreeNodeType * inputRoot = const_cast<InputInternalTreeNodeType *>(inputPtr->GetDataTree()->GetRoot());

  // Create the output tree root
  OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
  newDataNode->SetNodeType(inputRoot->Get()->GetNodeType());
  newDataNode->SetNodeId(inputRoot->Get()->GetNodeId());
  typename OutputInternalTreeNodeType::Pointer outputRoot = OutputInternalTreeNodeType::New();
  outputRoot->Set(newDataNode);
  tree->SetRoot(outputRoot);

  // Start recursive processing
  itk::TimeProbe chrono;
  chrono.Start();
  ProcessNode(inputRoot, outputRoot);
  chrono.Stop();
}


template <class TInputVectorData, class TOutputVectorData >
void
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ProcessNode(InputInternalTreeNodeType * source, OutputInternalTreeNodeType * destination)
{
  // Get the children list from the input node
  InputChildrenListType children = source->GetChildrenList();

  // For each child
  for(typename InputChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    typename OutputInternalTreeNodeType::Pointer newContainer;

    // Copy input DataNode info
    InputDataNodePointerType dataNode = (*it)->Get();
    OutputDataNodePointerType newDataNode   = OutputDataNodeType::New();
    newDataNode->SetNodeType(dataNode->GetNodeType());
    newDataNode->SetNodeId(dataNode->GetNodeId());
    newDataNode->CopyFieldList(dataNode);

    switch(dataNode->GetNodeType())
      {
      case ROOT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case DOCUMENT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case FOLDER:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case FEATURE_POINT:
      {
      PointType point = this->ReprojectPoint(dataNode->GetPoint());
      if (!vnl_math_isnan(point[0]) && !vnl_math_isnan(point[1]) )
        {
        newDataNode->SetPoint(this->ReprojectPoint(dataNode->GetPoint()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        }
      break;
      }
      case FEATURE_LINE:
      {
      LinePointerType line = this->ReprojectLine(dataNode->GetLine());
      if( line->GetVertexList()->Size() != 0 )
        {
        newDataNode->SetLine(this->ReprojectLine(dataNode->GetLine()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        }
      break;
      }
      case FEATURE_POLYGON:
      {
      PolygonPointerType polyExt = this->ReprojectPolygon(dataNode->GetPolygonExteriorRing());
      PolygonListPointerType polyInt = this->ReprojectPolygonList(dataNode->GetPolygonInteriorRings());
      bool ok=true;
      for (typename PolygonListType::ConstIterator it = polyInt->Begin();
           it != polyInt->End(); ++it)
        {
        if( it.Get()->GetVertexList()->Size() == 0 )
          ok = false;
        }
      if(polyExt->GetVertexList()->Size() != 0 && ok == true)
        {
        newDataNode->SetPolygonExteriorRing(this->ReprojectPolygon(dataNode->GetPolygonExteriorRing()));
        newDataNode->SetPolygonInteriorRings(this->ReprojectPolygonList(dataNode->GetPolygonInteriorRings()));
        newContainer = OutputInternalTreeNodeType::New();
        newContainer->Set(newDataNode);
        destination->AddChild(newContainer);
        }
      break;
      }
      case FEATURE_MULTIPOINT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case FEATURE_MULTILINE:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case FEATURE_MULTIPOLYGON:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      case FEATURE_COLLECTION:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      }
    }
}


} // end namespace otb

#endif
