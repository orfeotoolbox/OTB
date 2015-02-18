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
//  m_Transform = GenericTransformType::New();
  // with ITK v4 you can't instanciate a transform (virtual). We do NOT want to
  // use the otb::Transform or we loose the capability of using all the existing
  // itk transform, so we just keep it as NULL and you have to be careful not
  // to burn yourself when using it.
  m_Transform = NULL;
}

/**
* Convert point
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PointType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ProcessPoint(PointType pointCoord) const
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
::ProcessLine(LinePointerType line) const
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
    ++it;
    }

  return newLine;
}

/**
 * Convert polygon
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PolygonPointerType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ProcessPolygon(PolygonPointerType polygon) const
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
    ++it;
    }
  return newPolygon;
}

/**
* Convert polygon list
 */
template <class TInputVectorData, class TOutputVectorData >
typename VectorDataTransformFilter<TInputVectorData, TOutputVectorData>::PolygonListPointerType
VectorDataTransformFilter<TInputVectorData, TOutputVectorData>
::ProcessPolygonList(PolygonListPointerType polygonList) const
{

  PolygonListPointerType newPolygonList = PolygonListType::New();
  for (typename PolygonListType::ConstIterator it = polygonList->Begin();
       it != polygonList->End(); ++it)
    {
    newPolygonList->PushBack(this->ProcessPolygon(it.Get()));
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
  this->ProcessNode(inputRoot, outputRoot);
  chrono.Stop();
}

} // end namespace otb

#endif
