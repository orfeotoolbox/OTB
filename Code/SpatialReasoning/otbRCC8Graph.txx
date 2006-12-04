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
#ifndef _otbRCC8Graph_txx
#define _otbRCC8Graph_txx

namespace otb
{
  /** 
   * Constructor 
   */
  template <class TVertex>
  RCC8Graph<TVertex>
  ::RCC8Graph()
  {};
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
    for(int i = 0; i<m_NumberOfVertices;i++)
      {
	boost::add_vertex(m_Graph);
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
    VertexDescriptorType v = *boost::vertices(m_Graph).first;
    m_Graph[v+index]= vertex;
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
    return m_Graph[v+index];
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
    EdgeDescriptorType e = boost::add_edge(index1,index2,m_Graph).first;
    EdgeType::Pointer edge = EdgeType::New();
    edge->SetValue(r);
    m_Graph[e]=edge;
  }
  /**
   * PrintSelf method
   */
  template <class TVertex>
  void
  RCC8Graph<TVertex>
  ::PrintSelf( std::ostream& os,itk::Indent indent ) const
  {
    Superclass::PrintSelf(os,indent);
  }
}// end namespace otb
#endif
