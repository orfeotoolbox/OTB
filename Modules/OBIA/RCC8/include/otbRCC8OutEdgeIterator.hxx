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

#ifndef otbRCC8OutEdgeIterator_hxx
#define otbRCC8OutEdgeIterator_hxx

#include "otbRCC8OutEdgeIterator.h"

namespace otb
{
/** Constructor */
template <class TGraph>
RCC8OutEdgeIterator<TGraph>
::RCC8OutEdgeIterator()
{
  m_Graph = GraphType::New();
  tie(m_Iter, m_End) = out_edges(0, *m_Graph->GetGraph());
}
/**
 * Copy operator.
 */
template <class TGraph>
RCC8OutEdgeIterator<TGraph>
::RCC8OutEdgeIterator(const Self& iter)
{
  m_Iter = iter.m_Iter;
  m_Graph = iter.m_Graph;
  m_VertexIndex = iter.m_VertexIndex;
  m_End = iter.m_End;
}
/**
 *Constructor with input graph
 */
template <class TGraph>
RCC8OutEdgeIterator<TGraph>
::RCC8OutEdgeIterator(VertexDescriptorType vertex, TGraph * graph)
{
  m_Graph = graph;
  m_VertexIndex = vertex;
  tie(m_Iter, m_End) = out_edges(vertex, *m_Graph->GetGraph());
}
/**
 * Get the current edge value.
 * \return The value of the current edge pointed by the iterator.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::RCC8ValueType
RCC8OutEdgeIterator<TGraph>
::GetValue(void)
{
  return (*m_Graph->GetGraph())[*m_Iter]->GetValue();
}
/**
 * Return the target vertex of the current edge.
 * \return The target vertex.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::VertexPointerType
RCC8OutEdgeIterator<TGraph>
::GetTargetVertex(void)
{
  return (*m_Graph->GetGraph())[target(*m_Iter, (*m_Graph->GetGraph()))];
}
/**
 * Return the target vertex index of the current edge.
 * \return The target vertex index.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::VertexDescriptorType
RCC8OutEdgeIterator<TGraph>
::GetTargetIndex(void)
{
  IndexMapType index = get(boost::vertex_index, (*m_Graph->GetGraph()));
  return index[target(*m_Iter, (*m_Graph->GetGraph()))];
}
/**
 * Return true if the iterator is at the end.
 * \return True if the iterator is at the end.
 */
template <class TGraph>
bool
RCC8OutEdgeIterator<TGraph>
::IsAtEnd(void)
{
  return (m_Iter == m_End);
}
/**
 * Go to the beginning.
 */
template <class TGraph>
void
RCC8OutEdgeIterator<TGraph>
::GoToBegin(void)
{
  tie(m_Iter, m_End) = out_edges(m_VertexIndex, *m_Graph->GetGraph());
}
/**
 * Increment.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::Self&
RCC8OutEdgeIterator<TGraph>
::operator++ ()
  {
  ++m_Iter;
  return *this;
  }
/**
 * Decrement.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::Self&
RCC8OutEdgeIterator<TGraph>
::operator-- ()
  {
  --m_Iter;
  return *this;
  }
/**
 * Add
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::Self&
RCC8OutEdgeIterator<TGraph>
::operator + (int i)
  {
  m_Iter = m_Iter + i;
  return *this;
  }
/**
 * Remove
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::Self&
RCC8OutEdgeIterator<TGraph>
::operator - (int i)
  {
  m_Iter = m_Iter - i;
  return *this;
  }
/**
 * Difference comparison operator.
 */
template <class TGraph>
bool
RCC8OutEdgeIterator<TGraph>
::operator != (const Self &iter)
  {
  return (m_Iter != iter.m_Iter);
  }
/**
 * Equality comparison operator.
 */
template <class TGraph>
bool
RCC8OutEdgeIterator<TGraph>
::operator == (const Self &iter)
  {
  return (m_Iter == iter.m_Iter);
  }
/**
 * Instantiation operator.
 */
template <class TGraph>
typename RCC8OutEdgeIterator<TGraph>
::Self&
RCC8OutEdgeIterator<TGraph>
::operator = (const Self &iter)
  {
  m_Iter = iter.m_Iter;
  return *this;
  }
} // End namespace otb
#endif
