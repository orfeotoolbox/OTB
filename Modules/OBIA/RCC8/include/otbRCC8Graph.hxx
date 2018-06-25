/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8Graph_hxx
#define otbRCC8Graph_hxx

#include "otbRCC8Graph.h"

namespace otb
{
/**
 * Constructor
 */
template <class TVertex>
RCC8Graph<TVertex>
::RCC8Graph()
{
  m_NumberOfVertices = 0;
}
/**
 * since the number of vertices is mandatory to instantiate the
 * internal boost representation, the build method has to be called
 * once this parameter is set.
 */
template <class TVertex>
void
RCC8Graph<TVertex>
::Build(void)
{
  this->InitializeGraph(m_NumberOfVertices - 1);
}
/**
 * Initialize a range of vertex.
 * \param num The index of the last vertices to initialize.
 */
template <class TVertex>
void
RCC8Graph<TVertex>
::InitializeGraph(unsigned int num)
{
  for (unsigned int i = boost::num_vertices(m_Graph); i <= num; ++i)
    {
    VertexDescriptorType id = boost::add_vertex(m_Graph);
    VertexPointerType    vertex = VertexType::New();
    m_Graph[id] = vertex;
    }
}
/**
 * Set a vertex.
 * \param index The index of the vertex in the graph.
 * \param vertex The vertex to set.
 */
template<class TVertex>
void
RCC8Graph<TVertex>
::SetVertex(unsigned int index, VertexPointerType vertex)
{
  if (index >= m_NumberOfVertices)
    {
    this->InitializeGraph(index);
    m_NumberOfVertices = index + 1;
    }
  VertexDescriptorType v = *boost::vertices(m_Graph).first;
  m_Graph[v + index] = vertex;
}
/**
 * Get a vertex.
 * \param index The index of the vertex in the graph
 * \return The vertex.
 */
template <class TVertex>
typename RCC8Graph<TVertex>
::VertexPointerType
RCC8Graph<TVertex>
::GetVertex(unsigned int index)
{
  VertexDescriptorType v = *boost::vertices(m_Graph).first;
  return m_Graph[v + index];
}
/**
 * Add an edge in the graph.
 * \param index1 The index of the source vertex.
 * \param index2 The index of the target vertex.
 * \param r The rcc8 value associated to the edge.
 */
template <class TVertex>
void
RCC8Graph<TVertex>
::AddEdge(unsigned int index1, unsigned int index2, RCC8ValueType r)
{
  EdgeDescriptorType e = boost::add_edge(index1, index2, m_Graph).first;
  EdgeType::Pointer  edge = EdgeType::New();
  edge->SetValue(r);
  m_Graph[e] = edge;
}
/**
  * Get number of edges
  * /return The number of edges.
  */
template <class TVertex>
unsigned int
RCC8Graph<TVertex>
::GetNumberOfEdges(void)
{
  return num_edges(m_Graph);
}
/**
 * PrintSelf method
 */
template <class TVertex>
void
RCC8Graph<TVertex>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
