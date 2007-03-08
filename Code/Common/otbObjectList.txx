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
#ifndef _otbObjectList_txx
#define _otbObjectList_txx

#include "otbObjectList.h"

namespace otb 
{
  /**
   * Constructor
   */
  template <class TObject>
  ObjectList<TObject>
  ::ObjectList()
  {}
  /**
   * Set the minimum capacity of the vector.
   * \param size Size of the vector to reserve.
   */
  template <class TObject>
  void 
  ObjectList<TObject>
  ::Reserve(unsigned int size)
  {
    m_InternalContainer.reserve(size);
  }
  /**
   * Get the capacity of the vector.
   * \return The capacity of the vector. 
   */
  template <class TObject>
  unsigned int 
  ObjectList<TObject>
  ::Capacity(void)
  {
    return m_InternalContainer.capacity();
  }
  /**
   * Get the number of elements in the vector.
   * \return The number of elements in the vector.
   */
  template <class TObject>
  unsigned int 
  ObjectList<TObject>
  ::Size(void)
  {
    return m_InternalContainer.size();
  }
  /**
   * Resize the maximal list capacity.
   * \param size The new maximal size of the list.
   */
  template <class TObject>
  void 
  ObjectList<TObject>
  ::Resize(unsigned int size)
  {
    m_InternalContainer.resize(size);
  }
  /**
   * Append an element to the list.
   * \param element Pointer to the element to append.
   */
  template <class TObject>
  void 
  ObjectList<TObject>
  ::PushBack(ObjectPointerType element)
  {
    m_InternalContainer.push_back(element);
  }
  /**
   * Set the nth element of the list.
   * \param index The index where to put the element.
   * \param element Pointer to the element to set.
   */
  template <class TObject>
  void 
  ObjectList<TObject>
  ::SetNthElement(unsigned int index,ObjectPointerType element)
  {
    m_InternalContainer[index]=element;
  }
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  template <class TObject>
  typename ObjectList<TObject>::ObjectPointerType 
  ObjectList<TObject>
  ::GetNthElement(unsigned int index)
  {
    return m_InternalContainer[index];
  }
/**
   * Return the first element of the list.
   * \return The first element of the list.
   */

  template <class TObject>
  typename ObjectList<TObject>::ObjectPointerType 
  ObjectList<TObject>
  ::Front(void)
  {
    return m_InternalContainer.front();
  }
  /**
   * Return the last element of the list.
   * \return The last element of the list.
   */
  template <class TObject>
  typename ObjectList<TObject>::ObjectPointerType 
  ObjectList<TObject>
  ::Back(void)
  {
    return m_InternalContainer.back();
  }
  /**
   * Erase the nth element in the list.
   * \param index The index of the element to erase.
   */
  template <class TObject>
  void
  ObjectList<TObject>
  ::Erase(unsigned int index)
  {
    m_InternalContainer.erase(m_InternalContainer.begin()+index);
  }
  /**
   * Clear the object list.
   */
  template <class TObject>
  void
  ObjectList<TObject>
  ::Clear(void)
  {
    m_InternalContainer.clear();
  }



  /**
   * Get an Iterator that points to the beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::Iterator 
  ObjectList<TObject>
  ::Begin(void)
  {
    Iterator iter(m_InternalContainer.begin());
    return iter;
  }
  /**
   * Get a ConstIterator that points to the beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ConstIterator
  ObjectList<TObject>
  ::Begin(void) const
  {
    ConstIterator iter(m_InternalContainer.begin());
    return iter;
  }
  /**
   * Get a ReverseIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ReverseIterator
  ObjectList<TObject>
  :: ReverseBegin(void)
  {
    ReverseIterator iter(m_InternalContainer.rbegin());
    return iter;
  }
  /**
   * Get a ReverseConstIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ReverseConstIterator 
  ObjectList<TObject>
  ::ReverseBegin(void) const
  {
    ReverseConstIterator iter(m_InternalContainer.rbegin());
    return iter;
  }
  /**
   * Get an Iterator that points past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::Iterator
  ObjectList<TObject>
  :: End(void)
  { 
    Iterator iter(m_InternalContainer.end());
    return iter;
  }
  /**
   * Get a ConstIterator that points past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ConstIterator 
  ObjectList<TObject>
  ::End(void) const
  {
    ConstIterator iter(m_InternalContainer.end());
    return iter;
  }
  /**
   * Get a ReverseIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ReverseIterator 
  ObjectList<TObject>
  ::ReverseEnd(void)
  {
    ReverseIterator iter(m_InternalContainer.rend());
    return iter;
  }
  /**
   * Get a ReverseConstIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename ObjectList<TObject>::ReverseConstIterator 
  ObjectList<TObject>
  ::ReverseEnd(void) const
  {
    ReverseConstIterator iter(m_InternalContainer.rend());
    return iter;
  } 
  /**
   * Erase elements from begin to last.
   * \param begin Iterator pointing on first object to erase.
   * \param end Iterator pointing past the last object to erase.
   */
  template <class TObject>
  void
  ObjectList<TObject>
  ::Erase(Iterator begin, Iterator end)
  {
    m_InternalContainer.erase(begin.GetIter(),end.GetIter());
  }
  /**PrintSelf method */
  template <class TObject>
  void 
  ObjectList<TObject>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Size: " << m_InternalContainer.size() << std::endl;
  }
} // end namespace otb

#endif
