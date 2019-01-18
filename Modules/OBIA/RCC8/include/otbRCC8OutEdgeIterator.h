/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8OutEdgeIterator_h
#define otbRCC8OutEdgeIterator_h

#include "otbRCC8Graph.h"

namespace otb
{
/**
 * \class RCC8OutEdgeIterator
 * \brief Iterates on the graph out edges from a vertex.
 *
 * \ingroup OTBRCC8
 */
template <class TGraph>
class ITK_EXPORT RCC8OutEdgeIterator
{
public:
  /** self typedef */
  typedef RCC8OutEdgeIterator Self;

  /** Graph typedef */
  typedef TGraph                                   GraphType;
  typedef typename GraphType::InternalGraphType    InternalGraphType;
  typedef typename GraphType::Pointer              GraphPointerType;
  typedef typename GraphType::EdgePointerType      EdgePointerType;
  typedef typename GraphType::VertexPointerType    VertexPointerType;
  typedef typename GraphType::VertexDescriptorType VertexDescriptorType;
  typedef typename GraphType::RCC8ValueType        RCC8ValueType;
  /** typedef of the internal iterator */
  typedef typename boost::graph_traits<InternalGraphType>::out_edge_iterator InternalIteratorType;
  /** Typedef of the index map */
  typedef typename boost::property_map<InternalGraphType, boost::vertex_index_t>::type IndexMapType;

  /** Constructor */
  RCC8OutEdgeIterator();
  /** Copy constructor */
  RCC8OutEdgeIterator(const Self &iter);
  /**  Constructor with input graph */
  RCC8OutEdgeIterator(VertexDescriptorType vertex, TGraph * graph);
  /**
   * Get the current edge value.
   * \return The value of the current edge pointed by the iterator.
   */
  RCC8ValueType GetValue(void);
  /**
   * Return the target vertex of the current edge.
   * \return The target vertex.
   */
  VertexPointerType GetTargetVertex(void);
  /**
   * Return the target vertex index of the current edge.
   * \return The target vertex index.
   */
  VertexDescriptorType GetTargetIndex(void);

  /**
   * Return true if the iterator is at the end.
   * \return True if the iterator is at the end.
   */
  bool IsAtEnd(void);
  /**
   * Go to the beginning.
   */
  void GoToBegin(void);
  /**
   * Increment.
   */
  Self& operator ++();
  /**
   * Decrement.
   */
  Self& operator --();
  /**
   * Add
   */
  Self& operator +(int i);
  /**
   * Remove
   */
  Self& operator -(int i);
  /**
   * Difference comparison operator.
   */
  bool operator !=(const Self& it);
  /**
   * Equality comparison operator.
   */
  bool operator ==(const Self& it);
  /**
   * Instantiation operator.
   */
  Self& operator =(const Self& it);
private:
  // End
  InternalIteratorType m_End;
  // Internal iterator.
  InternalIteratorType m_Iter;
  // Input graph pointer
  GraphPointerType m_Graph;
  // source vertex descriptor
  VertexDescriptorType m_VertexIndex;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8OutEdgeIterator.hxx"
#endif

#endif
