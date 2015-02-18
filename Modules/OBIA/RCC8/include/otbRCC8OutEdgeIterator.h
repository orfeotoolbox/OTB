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
#ifndef __otbRCC8OutEdgeIterator_h
#define __otbRCC8OutEdgeIterator_h

#include "otbRCC8Graph.h"

namespace otb
{
/**
 * \class RCC8OutEdgeIterator
 * \brief Iterates on the graph out edges from a vertex.
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
#include "otbRCC8OutEdgeIterator.txx"
#endif

#endif
