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
#ifndef otbRCC8InEdgeIterator_h
#define otbRCC8InEdgeIterator_h

#include "otbRCC8Graph.h"

namespace otb
{
/**
 * \class RCC8InEdgeIterator
 * \brief Iterates on the graph in edges from a vertex.
 *
 * \ingroup OTBRCC8
 */
template <class TGraph>
class ITK_EXPORT RCC8InEdgeIterator
{
public:
  /** self typedef */
  typedef RCC8InEdgeIterator Self;

  /** Graph typedef */
  typedef TGraph                                   GraphType;
  typedef typename GraphType::InternalGraphType    InternalGraphType;
  typedef typename GraphType::Pointer              GraphPointerType;
  typedef typename GraphType::EdgePointerType      EdgePointerType;
  typedef typename GraphType::VertexPointerType    VertexPointerType;
  typedef typename GraphType::VertexDescriptorType VertexDescriptorType;
  typedef typename GraphType::RCC8ValueType        RCC8ValueType;
  /** typedef of the internal iterator */
  typedef typename boost::graph_traits<InternalGraphType>::in_edge_iterator InternalIteratorType;
  /** Typedef of the index map */
  typedef typename boost::property_map<InternalGraphType, boost::vertex_index_t>::type IndexMapType;

  /** Constructor */
  RCC8InEdgeIterator();
  /** Copy constructor */
  RCC8InEdgeIterator(const Self &iter);
  /**  Constructor with input graph */
  RCC8InEdgeIterator(VertexDescriptorType vertex, GraphType * graph);
  /**
   * Get the current edge value.
   * \return The value of the current edge pointed by the iterator.
   */
  RCC8ValueType GetValue(void);
  /**
    * Return the source vertex of the current edge.
    * \return The source edge.
    */
  VertexPointerType GetSourceVertex(void);
  /**
   * Return the source vertex index of the current edge.
   * \return The souce vertex index.
   */
  VertexDescriptorType GetSourceIndex(void);
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
  // Target vertex index
  VertexDescriptorType m_VertexIndex;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8InEdgeIterator.txx"
#endif

#endif
