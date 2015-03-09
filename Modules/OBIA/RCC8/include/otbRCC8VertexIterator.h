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
#ifndef __otbRCC8VertexIterator_h
#define __otbRCC8VertexIterator_h

#include "otbRCC8Graph.h"

namespace otb
{
/**
 * \class RCC8VertexIterator
 * \brief Iterates on the graph vertices
 *
 * \ingroup OTBRCC8
 */
template <class TGraph>
class ITK_EXPORT RCC8VertexIterator
{
public:
  /** self typedef */
  typedef RCC8VertexIterator Self;

  /** Graph typedef */
  typedef TGraph                                   GraphType;
  typedef typename GraphType::InternalGraphType    InternalGraphType;
  typedef typename GraphType::Pointer              GraphPointerType;
  typedef typename GraphType::VertexPointerType    VertexPointerType;
  typedef typename GraphType::VertexDescriptorType VertexDescriptorType;
  /** typedef of the internal iterator */
  typedef typename boost::graph_traits<InternalGraphType>::vertex_iterator InternalIteratorType;
  /** Typedef of the index map */
  typedef typename boost::property_map<InternalGraphType, boost::vertex_index_t>::type IndexMapType;
  /** Constructor */
  RCC8VertexIterator();
  /** Copy constructor */
  RCC8VertexIterator(const Self &iter);
  /**  Constructor with input graph */
  RCC8VertexIterator(TGraph * graph);
  /**
   * Get the current vertex.
   * \return The current vertex pointed by the iterator.
   */
  VertexPointerType Get(void);
  /**
   * Set the current vertex.
   * \param vertex The vertex to set.
   */
  void Set(VertexPointerType vertex);
  /**
   * Get the current vertex index.
   * \return The current vertex index.
   */
  VertexDescriptorType GetIndex(void);
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
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexIterator.txx"
#endif

#endif
