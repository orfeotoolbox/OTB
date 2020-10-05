/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbVectorDataAdapterFilter_hxx
#define otbVectorDataAdapterFilter_hxx

#include "otbVectorDataAdapter.h"

namespace otb
{

/**
* Convert point
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataAdapter<TInputVectorData, TOutputVectorData>::OutputPointType
VectorDataAdapter<TInputVectorData, TOutputVectorData>::ProcessPoint(InputPointType pointCoord) const
{

  OutputPointType point;
  point[0] = pointCoord[0];
  point[1] = pointCoord[1];
  if (OutputPointType::PointDimension == 3)
  {
    point[2] = pointCoord[2];
  }
  return point;
}

/**
 * Convert line
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataAdapter<TInputVectorData, TOutputVectorData>::OutputLinePointerType
VectorDataAdapter<TInputVectorData, TOutputVectorData>::ProcessLine(InputLinePointerType line) const
{
  typedef typename InputLineType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename InputLineType::VertexListConstIteratorType  VertexListConstIteratorType;
  VertexListConstPointerType                                   vertexList = line->GetVertexList();
  VertexListConstIteratorType                                  it         = vertexList->Begin();
  typename OutputLineType::Pointer                             newLine    = OutputLineType::New();
  while (it != vertexList->End())
  {
    typename OutputLineType::VertexType index;
    typename InputLineType::VertexType  pointCoord = it.Value();
    index[0]                                       = pointCoord[0];
    index[1]                                       = pointCoord[1];

    // fixme handle 2.5D ?

    newLine->AddVertex(index);
    ++it;
  }

  return newLine;
}

/**
 * Convert polygon
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataAdapter<TInputVectorData, TOutputVectorData>::OutputPolygonPointerType
VectorDataAdapter<TInputVectorData, TOutputVectorData>::ProcessPolygon(InputPolygonPointerType polygon) const
{
  typedef typename InputPolygonType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename InputPolygonType::VertexListConstIteratorType  VertexListConstIteratorType;
  VertexListConstPointerType                                      vertexList = polygon->GetVertexList();
  VertexListConstIteratorType                                     it         = vertexList->Begin();
  typename OutputPolygonType::Pointer                             newPolygon = OutputPolygonType::New();
  while (it != vertexList->End())
  {
    typename OutputPolygonType::VertexType index;
    typename InputPolygonType::VertexType  pointCoord = it.Value();
    index[0]                                          = pointCoord[0];
    index[1]                                          = pointCoord[1];

    // fixme handle 2.5D ?

    newPolygon->AddVertex(index);
    ++it;
  }
  return newPolygon;
}

/**
* Convert polygon list
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataAdapter<TInputVectorData, TOutputVectorData>::OutputPolygonListPointerType
VectorDataAdapter<TInputVectorData, TOutputVectorData>::ProcessPolygonList(InputPolygonListPointerType polygonList) const
{

  OutputPolygonListPointerType newPolygonList = OutputPolygonListType::New();
  for (typename InputPolygonListType::ConstIterator it = polygonList->Begin(); it != polygonList->End(); ++it)
  {
    newPolygonList->PushBack(this->ProcessPolygon(it.Get()));
  }
  return newPolygonList;
}


} // end namespace otb

#endif
