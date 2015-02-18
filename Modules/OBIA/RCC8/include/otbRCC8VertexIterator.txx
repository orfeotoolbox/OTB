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
#ifndef __otbRCC8VertexIterator_txx
#define __otbRCC8VertexIterator_txx

#include "otbRCC8VertexIterator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TGraph>
RCC8VertexIterator<TGraph>
::RCC8VertexIterator()
{
  m_Graph = GraphType::New();
  boost::tie(m_Iter, m_End) = boost::vertices(*(m_Graph->GetGraph()));
}
/**
 * Copy operator.
 */
template <class TGraph>
RCC8VertexIterator<TGraph>
::RCC8VertexIterator(const Self& iter)
{
  m_Iter = iter.m_Iter;
  m_Graph = iter.m_Graph;
  m_End = iter.m_End;
}
/**
 *Constructor with input graph
 */
template <class TGraph>
RCC8VertexIterator<TGraph>
::RCC8VertexIterator(TGraph * graph)
{
  m_Graph = graph;
  boost::tie(m_Iter, m_End) = boost::vertices(*(m_Graph->GetGraph()));
}
/**
 * Get the current object.
 * \return The current object pointed by the iterator.
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::VertexPointerType
RCC8VertexIterator<TGraph>
::Get(void)
{
  return (*(m_Graph->GetGraph()))[*m_Iter];
}
/**
 * Set the current vertex.
 * \param vertex The vertex to set.
 */
template <class TGraph>
void
RCC8VertexIterator<TGraph>
::Set(VertexPointerType vertex)
{
  (*(m_Graph->GetGraph()))[*m_Iter] = vertex;
}
/**
 * Get the current vertex index.
 * \return The current vertex index.
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::VertexDescriptorType
RCC8VertexIterator<TGraph>
::GetIndex(void)
{
  IndexMapType index = get(boost::vertex_index, (*m_Graph->GetGraph()));
  return index[*m_Iter];
}
/**
  * Return true if the iterator is at the end.
  * \return True if the iterator is at the end.
  */
template <class TGraph>
bool
RCC8VertexIterator<TGraph>
::IsAtEnd(void)
{
  return (m_Iter == m_End);
}
/**
 * Go to the beginning.
 */
template <class TGraph>
void
RCC8VertexIterator<TGraph>
::GoToBegin(void)
{
  boost::tie(m_Iter, m_End) = boost::vertices(*(m_Graph->GetGraph()));
}
/**
 * Increment.
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::Self&
RCC8VertexIterator<TGraph>
::operator++ ()
  {
  ++m_Iter;
  return *this;
  }
/**
 * Decrement.
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::Self&
RCC8VertexIterator<TGraph>
::operator-- ()
  {
  --m_Iter;
  return *this;
  }
/**
 * Add
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::Self&
RCC8VertexIterator<TGraph>
::operator + (int i)
  {
  m_Iter = m_Iter + i;
  return *this;
  }
/**
 * Remove
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::Self&
RCC8VertexIterator<TGraph>
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
RCC8VertexIterator<TGraph>
::operator != (const Self &iter)
  {
  return (m_Iter != iter.m_Iter);
  }
/**
 * Equality comparison operator.
 */
template <class TGraph>
bool
RCC8VertexIterator<TGraph>
::operator == (const Self &iter)
  {
  return (m_Iter == iter.m_Iter);
  }
/**
 * Instantiation operator.
 */
template <class TGraph>
typename RCC8VertexIterator<TGraph>
::Self&
RCC8VertexIterator<TGraph>
::operator = (const Self &iter)
  {
  m_Iter = iter.m_Iter;
  return *this;
  }
} // End namespace otb
#endif
